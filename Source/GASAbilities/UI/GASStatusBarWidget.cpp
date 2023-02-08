// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GASStatusBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UGASStatusBarWidget::SetHealth(float CurrentValue, float MaxValue)
{
	SetSpecificBar(HealthBar, HealthText, CurrentValue, MaxValue);
}

void UGASStatusBarWidget::SetEnergy(float CurrentValue, float MaxValue)
{
	SetSpecificBar(EnergyBar, EnergyText, CurrentValue, MaxValue);
}

void UGASStatusBarWidget::SetSpecificBar(UProgressBar* ProgressBar, UTextBlock* Text, float CurrentValue, float MaxValue) const
{
	if (!ProgressBar || !Text)
	{
		return;
	}

	ProgressBar->SetPercent(CurrentValue / MaxValue);
	Text->SetText(FText::FromString(TTypeToString<uint32>::ToString(CurrentValue)));
}
