// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASAbilitiesCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/DecalComponent.h"
#include "GameplayAbilitySystem/GASAbilitySystemComponent.h"
#include "GameplayAbilitySystem/GASAttributeSet.h"
#include "Net/UnrealNetwork.h"

#define RETURN_ATTRIBUTE_VALUE(AttributeClass, AttributeName)                                      \
	if (AbilitySystemComponent)                                                                    \
	{                                                                                              \
		if (const AttributeClass* AttributeSet = AbilitySystemComponent->GetSet<AttributeClass>()) \
		{                                                                                          \
			return AttributeSet->Get##AttributeName();                                             \
		}                                                                                          \
	}                                                                                              \
	return 0.f                                                                                            
//////////////////////////////////////////////////////////////////////////
// AGASAbilitiesCharacter

AGASAbilitiesCharacter::AGASAbilitiesCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	SelectDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectDecal"));
	SelectDecal->SetupAttachment(RootComponent);
	SelectDecal->SetVisibility(false);
	SelectDecal->SetActive(false);

	AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	bReplicates = true;
}

void AGASAbilitiesCharacter::OnSelect()
{
	if (SelectDecal)
	{
		SelectDecal->SetVisibility(true);
		SelectDecal->SetActive(true);
	}
}

void AGASAbilitiesCharacter::OnUnselect()
{
	if (SelectDecal)
	{
		SelectDecal->SetVisibility(false);
		SelectDecal->SetActive(false);
	}
}

void AGASAbilitiesCharacter::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle UpdateMovingTagTimer;
	GetWorldTimerManager().SetTimer(UpdateMovingTagTimer, this, &AGASAbilitiesCharacter::OnUpdateMovingTag, 0.05f, true);

	if (AbilitySystemComponent)
	{
		if (HasAuthority())
		{
			if (FireballAbility)
			{
				AbilitySystemComponent->GiveAbility(FireballAbility);
			}

			if (HealAbility)
			{
				AbilitySystemComponent->GiveAbility(HealAbility);
			}
		}
		else
		{
			for (UAttributeSet* AttributSet : AbilitySystemComponent->GetSpawnedAttributes())
			{
				if (UGASAttributeSet* GASAttributSet = Cast<UGASAttributeSet>(AttributSet))
				{
					GASAttributSet->OnHealthChangeDelegate.AddUniqueDynamic(this, &AGASAbilitiesCharacter::OnHealthChange);
					GASAttributSet->OnEnergyChangeDelegate.AddUniqueDynamic(this, &AGASAbilitiesCharacter::OnEnergyChange);
				}
			}
		}
	}
}

void AGASAbilitiesCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

void AGASAbilitiesCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

float AGASAbilitiesCharacter::GetCurrentHealth() const
{
	RETURN_ATTRIBUTE_VALUE(UGASAttributeSet, CurrentHealth);
}

float AGASAbilitiesCharacter::GetMaxHealth() const
{
	RETURN_ATTRIBUTE_VALUE(UGASAttributeSet, MaxHealth);
}

float AGASAbilitiesCharacter::GetCurrentEnergy() const
{
	RETURN_ATTRIBUTE_VALUE(UGASAttributeSet, CurrentEnergy);
}

float AGASAbilitiesCharacter::GetMaxEnergy() const
{
	RETURN_ATTRIBUTE_VALUE(UGASAttributeSet, MaxEnergy);
}

void AGASAbilitiesCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGASAbilitiesCharacter, AbilitySystemComponent);
}


float AGASAbilitiesCharacter::GetCastProgress() const
{
	return AbilitySystemComponent ? AbilitySystemComponent->GetCastingProgress() : -1.f;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGASAbilitiesCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("CastFireball", IE_Pressed, this, &AGASAbilitiesCharacter::CastFireball);
	PlayerInputComponent->BindAction("CastHeal",     IE_Pressed, this, &AGASAbilitiesCharacter::CastHeal);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AGASAbilitiesCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AGASAbilitiesCharacter::MoveRight);
}


void AGASAbilitiesCharacter::OnHealthChange(float CurrentHealth, float MaxHealth, float OldHealth, float OldMaxHealth)
{
	OnHealthUpdateDelegate.Broadcast(CurrentHealth, MaxHealth);
}

void AGASAbilitiesCharacter::OnEnergyChange(float CurrentEnergy, float MaxEnergy, float OldEnergy, float OldMaxEnergy)
{
	OnEnergyUpdateDelegate.Broadcast(CurrentEnergy, MaxEnergy);
}

void AGASAbilitiesCharacter::OnUpdateMovingTag()
{
	check(AbilitySystemComponent);

	if (!GetCharacterMovement()->Velocity.IsZero() && !AbilitySystemComponent->HasMatchingGameplayTag(MovingTag))
	{
		AbilitySystemComponent->AddLooseGameplayTag(MovingTag);
	}
	else if (GetCharacterMovement()->Velocity.IsZero() && AbilitySystemComponent->HasMatchingGameplayTag(MovingTag))
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(MovingTag);
	}
}

UAbilitySystemComponent* AGASAbilitiesCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGASAbilitiesCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AGASAbilitiesCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AGASAbilitiesCharacter::CastFireball()
{
	if (FireballAbility && AbilitySystemComponent)
	{
		AbilitySystemComponent->TryActivateAbilityByClass(FireballAbility);
	}
}

void AGASAbilitiesCharacter::CastHeal()
{
	if (HealAbility && AbilitySystemComponent)
	{
		AbilitySystemComponent->TryActivateAbilityByClass(HealAbility);
	}
}
#undef RETURN_ATTRIBUTE_VALUE
