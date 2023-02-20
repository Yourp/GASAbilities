// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GASGameplayInterfaceWidget.h"
#include "Interfaces/StatusBroadcaster.h"
#include "GASStatusBarWidget.h"
#include "Player/GASPlayerController.h"

void UGASGameplayInterfaceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IStatusBroadcaster* StatusBroadcaster = GetOwningPlayerPawn<IStatusBroadcaster>())
	{
		StatusBroadcaster->OnHealthUpdateDelegate.AddUniqueDynamic(this, &UGASGameplayInterfaceWidget::OnUpdateHealth);
		StatusBroadcaster->OnEnergyUpdateDelegate.AddUniqueDynamic(this, &UGASGameplayInterfaceWidget::OnUpdateEnergy);

		OnUpdateHealth(StatusBroadcaster->GetCurrentHealth(), StatusBroadcaster->GetMaxHealth());
		OnUpdateEnergy(StatusBroadcaster->GetCurrentEnergy(), StatusBroadcaster->GetMaxEnergy());
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
