// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GASStatusBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UGASStatusBarWidget::SetHealth(float CurrentValue, float MaxValue, bool bWithAnimation/* = false*/)
{
	HealthData.SetValues(CurrentValue, MaxValue, bWithAnimation);
}

void UGASStatusBarWidget::SetEnergy(float CurrentValue, float MaxValue, bool bWithAnimation/* = false*/)
{
	EnergyData.SetValues(CurrentValue, MaxValue, bWithAnimation);
}

void UGASStatusBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	HealthData.Update(InDeltaTime);
	EnergyData.Update(InDeltaTime);
}

void UGASStatusBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HealthData.Initialize(HealthBar, HealthText, AnimationSpeed);
	EnergyData.Initialize(EnergyBar, EnergyText, AnimationSpeed);
}

void FStatusBarData::Initialize(UProgressBar* NewProgressBar, UTextBlock* NewTextBlock, float NewAnimationSpeed)
{
	ProgressBar     = NewProgressBar;
	TextBlock       = NewTextBlock;
	AnimationSpeed  = NewAnimationSpeed;
}

void FStatusBarData::SetValues(float NewCurrentValue, float NewMaxValue, bool bWithAnimation)
{
	if (!ProgressBar || !TextBlock)
	{
		return;
	}

	ActualValue  = NewCurrentValue;
	MaxValue     = NewMaxValue;

	TextBlock->SetText(FText::FromString(TTypeToString<uint32>::ToString(ActualValue)));

	if (!bWithAnimation)
	{
		VisualValue = ActualValue;
		ProgressBar->SetPercent(VisualValue / MaxValue);
	}
}

void FStatusBarData::Update(float Delay)
{
	if (!ProgressBar)
	{
		return;
	}

	if (ActualValue != VisualValue)
	{
		VisualValue = FMath::FInterpTo(VisualValue, ActualValue, Delay, AnimationSpeed);
		ProgressBar->SetPercent(VisualValue / MaxValue);
	}
}
