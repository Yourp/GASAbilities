// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASAbilitiesCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/DecalComponent.h"

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

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
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

//////////////////////////////////////////////////////////////////////////
// Input

void AGASAbilitiesCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AGASAbilitiesCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AGASAbilitiesCharacter::MoveRight);
}


void AGASAbilitiesCharacter::BroadcastHealth()
{
	OnHealthUpdateDelegate.Broadcast(7555, 10000);
}


void AGASAbilitiesCharacter::BroadcastEnergy()
{
	OnEnergyUpdateDelegate.Broadcast(456, 1000);
}


UAbilitySystemComponent* AGASAbilitiesCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
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
