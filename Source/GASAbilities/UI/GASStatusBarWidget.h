// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GASUserWidgetBase.h"
#include "GASStatusBarWidget.generated.h"

class UProgressBar;
class UTextBlock;

USTRUCT()
struct FStatusBarData
{
	GENERATED_BODY()

private:

	UPROPERTY()
	UProgressBar* ProgressBar = nullptr;

	UPROPERTY()
	UTextBlock* TextBlock = nullptr;

	float ActualValue     = 0.f;
	float VisualValue     = 0.f;
	float MaxValue        = 0.f;
	float AnimationSpeed  = 5.f;

public:

	void Initialize(UProgressBar* NewProgressBar, UTextBlock* NewTextBlock, float NewAnimationSpeed);
	void SetValues(float NewCurrentValue, float NewMaxValue, bool bWithAnimation);
	void Update(float Delay);
};

/**
 * 
 */
UCLASS()
class UGASStatusBarWidget : public UGASUserWidgetBase
{
	GENERATED_BODY()
	
public:

	void SetHealth(float CurrentValue, float MaxValue, bool bWithAnimation = false);
	void SetEnergy(float CurrentValue, float MaxValue, bool bWithAnimation = false);

protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	virtual void NativeConstruct() override;

private:

	void SetSpecificBar(UProgressBar* ProgressBar, UTextBlock* Text, float CurrentValue, float MaxValue) const;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* EnergyBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EnergyText;

	FStatusBarData HealthData;
	FStatusBarData EnergyData;

	UPROPERTY(EditDefaultsOnly)
	float AnimationSpeed = 5.f;
};
