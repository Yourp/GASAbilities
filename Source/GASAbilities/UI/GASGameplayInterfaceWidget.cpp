// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GASGameplayInterfaceWidget.h"
#include "Interfaces/StatusBroadcaster.h"
#include "GASStatusBarWidget.h"
#include "Player/GASPlayerController.h"
#include "Components/ProgressBar.h"

void UGASGameplayInterfaceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StatusBroadcaster.SetObject(GetOwningPlayerPawn());
	StatusBroadcaster.SetInterface(Cast<IStatusBroadcaster>(GetOwningPlayerPawn()));

	if (StatusBroadcaster)
	{
		StatusBroadcaster->OnHealthUpdateDelegate.AddUniqueDynamic(this, &UGASGameplayInterfaceWidget::OnUpdateHealth);
		StatusBroadcaster->OnEnergyUpdateDelegate.AddUniqueDynamic(this, &UGASGameplayInterfaceWidget::OnUpdateEnergy);

		OnUpdateHealth(StatusBroadcaster->GetCurrentHealth(), StatusBroadcaster->GetMaxHealth());
		OnUpdateEnergy(StatusBroadcaster->GetCurrentEnergy(), StatusBroadcaster->GetMaxEnergy());
		
		if (CastBar)
		{
			CastBar->PercentDelegate.BindUFunction(this, "UpdateCastBarPercent");
			CastBar->VisibilityDelegate.BindUFunction(this, "UpdateCastBarVisibility");
			CastBar->SynchronizeProperties();
		}
	}

	if (AGASPlayerController* PlayerController = GetOwningPlayer<AGASPlayerController>())
	{
		PlayerController->OnSelectNewTargetDelegate.AddUniqueDynamic(this, &UGASGameplayInterfaceWidget::OnUpdateTarget);
	}
}

void UGASGameplayInterfaceWidget::SetTarget(const TScriptInterface<IStatusBroadcaster>& NewTarget)
{
	if (!TargetBar || NewTarget == CurrentTarget)
	{
		return;
	}

	if (CurrentTarget)
	{
		CurrentTarget->OnHealthUpdateDelegate.RemoveDynamic(this, &UGASGameplayInterfaceWidget::OnUpdateTargetHealth);
		CurrentTarget->OnEnergyUpdateDelegate.RemoveDynamic(this, &UGASGameplayInterfaceWidget::OnUpdateTargetEnergy);
		TargetBar->SetVisibility(ESlateVisibility::Collapsed);
	}

	CurrentTarget = NewTarget;

	if (CurrentTarget)
	{
		CurrentTarget->OnHealthUpdateDelegate.AddUniqueDynamic(this, &UGASGameplayInterfaceWidget::OnUpdateTargetHealth);
		CurrentTarget->OnEnergyUpdateDelegate.AddUniqueDynamic(this, &UGASGameplayInterfaceWidget::OnUpdateTargetEnergy);

		OnUpdateTargetHealth(CurrentTarget->GetCurrentHealth(), CurrentTarget->GetMaxHealth());
		OnUpdateTargetEnergy(CurrentTarget->GetCurrentEnergy(), CurrentTarget->GetMaxEnergy());

		TargetBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UGASGameplayInterfaceWidget::OnUpdateTarget(const TScriptInterface<IAbilitySystemInterface>& NewTarget)
{
	SetTarget(TScriptInterface<IStatusBroadcaster>(NewTarget.GetObject()));
}

void UGASGameplayInterfaceWidget::OnUpdateHealth(float CurrentValue, float MaxValue)
{
	SetHealth(SelfBar, CurrentValue, MaxValue);
}

void UGASGameplayInterfaceWidget::OnUpdateEnergy(float CurrentValue, float MaxValue)
{
	SetEnergy(SelfBar, CurrentValue, MaxValue);
}

void UGASGameplayInterfaceWidget::OnUpdateTargetHealth(float CurrentValue, float MaxValue)
{
	SetHealth(TargetBar, CurrentValue, MaxValue);
}

void UGASGameplayInterfaceWidget::OnUpdateTargetEnergy(float CurrentValue, float MaxValue)
{
	SetEnergy(TargetBar, CurrentValue, MaxValue);
}

float UGASGameplayInterfaceWidget::UpdateCastBarPercent() const
{
	const float CastBarProgress = StatusBroadcaster ? StatusBroadcaster->GetCastProgress() : 1.f;

	return FMath::IsWithin(CastBarProgress, 0.f, 1.f) ? CastBarProgress : 1.f;
}

ESlateVisibility UGASGameplayInterfaceWidget::UpdateCastBarVisibility() const
{
	const bool bIsVisible = StatusBroadcaster ? FMath::IsWithin(StatusBroadcaster->GetCastProgress(), 0.f, 1.f) : false;

	return bIsVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
}

void UGASGameplayInterfaceWidget::SetEnergy(UGASStatusBarWidget* Bar, float CurrentValue, float MaxValue) const
{
	if (!Bar)
	{
		return;
	}

	Bar->SetEnergy(CurrentValue, MaxValue);
}

void UGASGameplayInterfaceWidget::SetHealth(UGASStatusBarWidget* Bar, float CurrentValue, float MaxValue) const
{
	if (!Bar)
	{
		return;
	}

	Bar->SetHealth(CurrentValue, MaxValue);
}
