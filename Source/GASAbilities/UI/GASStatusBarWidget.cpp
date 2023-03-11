// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GASStatusBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UGASStatusBarWidget::SetHealth(float CurrentValue, float MaxValue, bool bWithAnimation/* = false*/)
{
	ActualHealth  = CurrentValue;
	MaxHealth     = MaxValue;

	HealthText->SetText(FText::FromString(TTypeToString<uint32>::ToString(CurrentValue)));

	if (!bWithAnimation)
	{
		VisualHealth = CurrentValue;
		HealthBar->SetPercent(VisualHealth / MaxHealth);
	}
}

void UGASStatusBarWidget::SetEnergy(float CurrentValue, float MaxValue, bool bWithAnimation/* = false*/)
{
	ActualEnergy  = CurrentValue;
	MaxEnergy     = MaxValue;

	EnergyText->SetText(FText::FromString(TTypeToString<uint32>::ToString(CurrentValue)));

	if (!bWithAnimation)
	{
		VisualEnergy = CurrentValue;
		EnergyBar->SetPercent(VisualEnergy / MaxEnergy);
	}
}

void UGASStatusBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ActualHealth != VisualHealth)
	{
		VisualHealth = FMath::FInterpTo(VisualHealth, ActualHealth, InDeltaTime, AnimationSpeed);
		HealthBar->SetPercent(VisualHealth / MaxHealth);
	}

	if (ActualEnergy != VisualEnergy)
	{
		VisualEnergy = FMath::FInterpTo(VisualEnergy, ActualEnergy, InDeltaTime, AnimationSpeed);
		EnergyBar->SetPercent(VisualEnergy / MaxEnergy);
	}
}
