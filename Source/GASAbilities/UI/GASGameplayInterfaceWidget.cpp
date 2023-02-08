// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GASGameplayInterfaceWidget.h"
#include "Interfaces/StatusBroadcaster.h"
#include "GASStatusBarWidget.h"

void UGASGameplayInterfaceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IStatusBroadcaster* StatusBroadcaster = GetOwningPlayerPawn<IStatusBroadcaster>())
	{
		StatusBroadcaster->OnHealthUpdateDelegate.AddUniqueDynamic(this, &UGASGameplayInterfaceWidget::OnUpdateHealth);
		StatusBroadcaster->OnEnergyUpdateDelegate.AddUniqueDynamic(this, &UGASGameplayInterfaceWidget::OnUpdateEnergy);
		StatusBroadcaster->OnTargetUpdateDelegate.AddUniqueDynamic(this, &UGASGameplayInterfaceWidget::OnUpdateTarget);

		StatusBroadcaster->BroadcastHealth();
		StatusBroadcaster->BroadcastEnergy();
		StatusBroadcaster->BroadcastTarget();
	}
}

void UGASGameplayInterfaceWidget::OnUpdateTarget(const TScriptInterface<IStatusBroadcaster>& NewTarget)
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

		CurrentTarget->BroadcastHealth();
		CurrentTarget->BroadcastEnergy();

		TargetBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
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
