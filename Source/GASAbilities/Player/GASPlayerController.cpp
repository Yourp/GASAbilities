// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GASPlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Slate/Public/Widgets/SViewport.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

void AGASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		PlayerCameraManager->bUseClientSideCameraUpdates = false;

		UGameViewportClient* GameViewportClient = GetWorld()->GetGameViewport();

		if (GameViewportClient)
		{
			GameViewportClient->SetMouseLockMode(EMouseLockMode::DoNotLock);
		}
	}
}

void AGASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction ( "RightClick",     IE_Pressed,  this, &AGASPlayerController::RightClickPressed  );
	InputComponent->BindAction ( "RightClick",     IE_Released, this, &AGASPlayerController::RightClickReleased );
	InputComponent->BindAction ( "LeftClick",      IE_Pressed,  this, &AGASPlayerController::LeftClickPressed   );
	InputComponent->BindAction ( "LeftClick",      IE_Released, this, &AGASPlayerController::LeftClickReleased  );
	InputComponent->BindAxis   ( "MousePressing",               this, &AGASPlayerController::MousePressing      );
	InputComponent->BindAxis   ( "Turn",                        this, &AGASPlayerController::AddYawInput        );
	InputComponent->BindAxis   ( "LookUp",                      this, &AGASPlayerController::AddPitchInput      );
}

void AGASPlayerController::RightClickPressed()
{
	SaveLastCursorPositionOnCapture();
	UpdateControllerRotationYaw();
	SetOrientRotationToMovement(false);
}

void AGASPlayerController::RightClickReleased()
{
	if (!ShouldShowMouseCursor() && !IsInputKeyDown(EKeys::LeftMouseButton))
	{
		ReleaseMouseLockAndCapture();
	}
	UpdateControllerRotationYaw();
	SetOrientRotationToMovement(true);
}

void AGASPlayerController::LeftClickPressed()
{
	SaveLastCursorPositionOnCapture();
}

void AGASPlayerController::LeftClickReleased()
{
	if (!ShouldShowMouseCursor() && !IsInputKeyDown(EKeys::RightMouseButton))
	{
		ReleaseMouseLockAndCapture();
	}
}

void AGASPlayerController::MousePressing(float Val)
{
	if (!Val || !ShouldShowMouseCursor())
	{
		return;
	}

	float X = 0.f;
	float Y = 0.f;

	if (GetMousePosition(X, Y))
	{
		/** Checking if we try to move cursor when left or right mouse button is pressed. */
		if (FMath::Abs(X - LastCursorPositionBeforeHide.X) < 2.f && FMath::Abs(Y - LastCursorPositionBeforeHide.Y) < 2.f)
		{
			/** If different is not to big we are don't need to capture. */
			return;
		}

		/** Do capture things. */

		SaveLastCursorPositionOnCapture(X, Y);

		ULocalPlayer* LP = Cast<ULocalPlayer>(Player);

		if (LP)
		{
			TSharedPtr<SViewport> ViewportWidgetPtr = LP->ViewportClient->GetGameViewportWidget();

			if (ViewportWidgetPtr.IsValid())
			{
				bShowMouseCursor = false;
				TSharedRef<SViewport> ViewportWidgetRef = ViewportWidgetPtr.ToSharedRef();

				LP->GetSlateOperations().CaptureMouse(ViewportWidgetRef)
					.LockMouseToWidget(ViewportWidgetRef)
					.UseHighPrecisionMouseMovement(ViewportWidgetRef);
			}
		}
	}

	UpdateControllerRotationYaw();
}

void AGASPlayerController::AddYawInput(float Val)
{
	if (Val && !ShouldShowMouseCursor())
	{
		Super::AddYawInput(Val);
	}
}

void AGASPlayerController::AddPitchInput(float Val)
{
	if (Val && !ShouldShowMouseCursor())
	{
		Super::AddPitchInput(Val);
	}
}

void AGASPlayerController::SaveLastCursorPositionOnCapture(float& X, float& Y)
{
	LastCursorPositionBeforeHide.X = X;
	LastCursorPositionBeforeHide.Y = Y;
}

void AGASPlayerController::SaveLastCursorPositionOnCapture()
{
	/** We don't need to save it when capture is working and mouse position already saved. */
	if (ShouldShowMouseCursor())
	{
		GetMousePosition(LastCursorPositionBeforeHide.X, LastCursorPositionBeforeHide.Y);
	}
}

void AGASPlayerController::UpdateControllerRotationYaw()
{
	if (GetPawn())
	{
		const bool bCanControllerRotationYaw  = !ShouldShowMouseCursor() && IsInputKeyDown(EKeys::RightMouseButton);
		GetPawn()->bUseControllerRotationYaw  = bCanControllerRotationYaw;
	}
}

void AGASPlayerController::SetOrientRotationToMovement(bool ShouldRotate)
{
	if (GetCharacter())
	{
		if (UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(GetCharacter()->GetMovementComponent()))
		{
			MovementComponent->bOrientRotationToMovement = ShouldRotate;
		}
	}
}

void AGASPlayerController::ReleaseMouseLockAndCapture()
{
	bShowMouseCursor = true;
	SetMouseLocation(LastCursorPositionBeforeHide.X, LastCursorPositionBeforeHide.Y);
	ULocalPlayer* LP = Cast<ULocalPlayer>(Player);

	if (LP)
	{
		LP->GetSlateOperations().ReleaseMouseCapture().ReleaseMouseLock();
	}
}