// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GASUserWidgetBase.h"
#include "GASStatusBarWidget.generated.h"

class UProgressBar;
class UTextBlock;

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

	float ActualHealth = 0.f;
	float VisualHealth = 0.f;
	float MaxHealth = 0.f;

	float ActualEnergy = 0.f;
	float VisualEnergy = 0.f;
	float MaxEnergy = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float AnimationSpeed = 5.f;
};
