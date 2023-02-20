// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GASPlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Slate/Public/Widgets/SViewport.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/HasSelectingEvent.h"

bool AGASPlayerController::Server_SetUseControllerRotationYaw_Validate(bool Val) { return true; }
bool AGASPlayerController::Server_SetSelectedTarget_Validate(const TScriptInterface<IAbilitySystemInterface>& NewTarget) { return true; }

void AGASPlayerController::Server_SetSelectedTarget_Implementation(const TScriptInterface<IAbilitySystemInterface>& NewTarget)
{
	SelectedTarget = NewTarget;
}

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

void AGASPlayerController::SetSelectedTarget(IAbilitySystemInterface* NewTarget)
{
	if (NewTarget != GetSelectedTarget())
	{
		OnUnselectingTarget();
		SelectedTarget.SetObject(Cast<UObject>(NewTarget));
		SelectedTarget.SetInterface(NewTarget);
		Server_SetSelectedTarget(SelectedTarget);
		OnSelectingTarget();

		OnSelectNewTargetDelegate.Broadcast(SelectedTarget);
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

void AGASPlayerController::TrySelectTarget()
{
	FVector2D   MousePosition;
	FHitResult  HitResult;

	if (GetMousePosition(MousePosition.X, MousePosition.Y))
	{
		GetHitResultAtScreenPosition(MousePosition, ECC_WorldStatic, false, HitResult);
		SetSelectedTarget(Cast<IAbilitySystemInterface>(HitResult.GetActor()));
	}
}

void AGASPlayerController::OnUnselectingTarget()
{
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		if (IHasSelectingEvent* Target = Cast<IHasSelectingEvent>(SelectedTarget.GetObject()))
		{
			Target->OnUnselect();
		}
	}
}

void AGASPlayerController::OnSelectingTarget()
{
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		if (IHasSelectingEvent* Target = Cast<IHasSelectingEvent>(SelectedTarget.GetObject()))
		{
			Target->OnSelect();
		}
	}
}

void AGASPlayerController::RightClickPressed()
{
	SaveLastCursorPositionOnCapture();
	UpdateControllerRotationYaw();
}

void AGASPlayerController::RightClickReleased()
{
	if (ShouldShowMouseCursor())
	{
		TrySelectTarget();
	}
	else if (!IsInputKeyDown(EKeys::LeftMouseButton))
	{
		ReleaseMouseLockAndCapture();
	}

	UpdateControllerRotationYaw();
}

void AGASPlayerController::LeftClickPressed()
{
	SaveLastCursorPositionOnCapture();
}

void AGASPlayerController::LeftClickReleased()
{
	if (ShouldShowMouseCursor())
	{
		TrySelectTarget();
	}
	else if (!IsInputKeyDown(EKeys::RightMouseButton))
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

		if (GetPawn()->bUseControllerRotationYaw != bCanControllerRotationYaw)
		{
			GetPawn()->bUseControllerRotationYaw = bCanControllerRotationYaw;
			Server_SetUseControllerRotationYaw(bCanControllerRotationYaw);
		}
	}
}

void AGASPlayerController::Server_SetUseControllerRotationYaw_Implementation(bool Val)
{
	if (GetPawn())
	{
		GetPawn()->bUseControllerRotationYaw = Val;
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