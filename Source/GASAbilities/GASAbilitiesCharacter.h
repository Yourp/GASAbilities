// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/StatusBroadcaster.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/HasSelectingEvent.h"
#include "GameplayAbilitySpec.h"
#include "GASAbilitiesCharacter.generated.h"

class UGameplayAbility;

UCLASS(config=Game)
class AGASAbilitiesCharacter : 
	public ACharacter, 
	public IStatusBroadcaster, 
	public IAbilitySystemInterface,
	public IHasSelectingEvent
{
	GENERATED_BODY()

public:
	AGASAbilitiesCharacter();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void OnSelect() override;
	virtual void OnUnselect() override;

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Controller() override;

	virtual float GetCurrentHealth() const override;
	virtual float GetMaxHealth() const override;
	virtual float GetCurrentEnergy() const override;
	virtual float GetMaxEnergy() const override;
	virtual float GetCastProgress() const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void CastFireball();
	void CastHeal();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

private:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom = nullptr;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera = nullptr;

	UPROPERTY(EditAnywhere)
	class UDecalComponent* SelectDecal = nullptr;

	UPROPERTY(EditAnywhere, Replicated)
	class UGASAbilitySystemComponent* AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = Abilities)
	TSubclassOf<UGameplayAbility> FireballAbility;

	UPROPERTY(EditAnywhere, Category = Abilities)
	TSubclassOf<UGameplayAbility> HealAbility;

	UPROPERTY(EditAnywhere)
	FGameplayTag MovingTag;


	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION()
	void OnHealthChange(float CurrentHealth, float MaxHealth, float OldHealth, float OldMaxHealth);

	UFUNCTION()
	void OnEnergyChange(float CurrentEnergy, float MaxEnergy, float OldEnergy, float OldMaxEnergy);

	void OnUpdateMovingTag();
};

