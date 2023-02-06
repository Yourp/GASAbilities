// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GASPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AGASPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	
private:

	virtual void SetupInputComponent() override;

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

	/** Showing cursor and return to saved position (@See LastCursorPositionBeforeHide). Canceling capture things. */
	void ReleaseMouseLockAndCapture();

	/** Saved cursor location for capture implementation. */
	FVector2D LastCursorPositionBeforeHide;
};
