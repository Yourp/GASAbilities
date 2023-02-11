// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/StatusBroadcaster.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/HasSelectingEvent.h"
#include "GASAbilitiesCharacter.generated.h"

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

	virtual void BroadcastHealth() override;
	virtual void BroadcastEnergy() override;

	virtual void OnSelect() override;
	virtual void OnUnselect() override;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

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

	UPROPERTY(VisibleAnywhere)
	class UDecalComponent* SelectDecal = nullptr;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

};

