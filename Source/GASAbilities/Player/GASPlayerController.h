// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AbilitySystemInterface.h"
#include "GASPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectNewTarget, const TScriptInterface<IAbilitySystemInterface>&, NewTarget);

/**
 * 
 */
UCLASS()
class AGASPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	FORCEINLINE IAbilitySystemInterface* GetSelectedTarget() const { return SelectedTarget.GetInterface(); }
	void SetSelectedTarget(IAbilitySystemInterface* NewTarget);

	FOnSelectNewTarget OnSelectNewTargetDelegate;
	
private:

	virtual void SetupInputComponent() override;

	void TrySelectTarget();

	void OnUnselectingTarget();
	void OnSelectingTarget();

	void RightClickPressed();
	void RightClickReleased();
	void LeftClickPressed();
	void LeftClickReleased();

	void AddYawInput(float Val);
	void AddPitchInput(float Val);

	/** Left or Right button. */
	void MousePressing(float Val);

	/** Saving cursor position where X and Y. */
	void SaveLastCursorPositionOnCapture(float& X, float& Y);

	/** Saving current cursor position. */
	void SaveLastCursorPositionOnCapture();

	/** Set pawn ControllerRotationYaw = true if cursor is hidden and right mouse button is pressed. */
	void UpdateControllerRotationYaw();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetUseControllerRotationYaw(bool Val);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetSelectedTarget(const TScriptInterface<IAbilitySystemInterface>& NewTarget);

	/** Showing cursor and return to saved position (@See LastCursorPositionBeforeHide). Canceling capture things. */
	void ReleaseMouseLockAndCapture();

	/** Saved cursor location for capture implementation. */
	FVector2D LastCursorPositionBeforeHide;

	UPROPERTY()
	TScriptInterface<IAbilitySystemInterface> SelectedTarget;
};
