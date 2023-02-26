// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GASUserWidgetBase.h"
#include "GASGameplayInterfaceWidget.generated.h"

class UGASStatusBarWidget;
class IStatusBroadcaster;
class IAbilitySystemInterface;
class UProgressBar;

/**
 * 
 */
UCLASS()
class UGASGameplayInterfaceWidget : public UGASUserWidgetBase
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

private:

	void SetTarget(const TScriptInterface<IStatusBroadcaster>& NewTarget);

	UFUNCTION()
	void OnUpdateTarget(const TScriptInterface<IAbilitySystemInterface>& NewTarget);

	UFUNCTION()
	void OnUpdateHealth(float CurrentValue, float MaxValue);

	UFUNCTION()
	void OnUpdateEnergy(float CurrentValue, float MaxValue);

	UFUNCTION()
	void OnUpdateTargetHealth(float CurrentValue, float MaxValue);

	UFUNCTION()
	void OnUpdateTargetEnergy(float CurrentValue, float MaxValue);

	UFUNCTION()
	float UpdateCastBarPercent() const;

	UFUNCTION()
	ESlateVisibility UpdateCastBarVisibility() const;

	void SetEnergy(UGASStatusBarWidget* Bar, float CurrentValue, float MaxValue) const;
	void SetHealth(UGASStatusBarWidget* Bar, float CurrentValue, float MaxValue) const;

	UPROPERTY(meta = (BindWidget))
	UGASStatusBarWidget* SelfBar;

	UPROPERTY(meta = (BindWidget))
	UGASStatusBarWidget* TargetBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* CastBar;

	UPROPERTY()
	TScriptInterface<IStatusBroadcaster> CurrentTarget;

	UPROPERTY()
	TScriptInterface<IStatusBroadcaster> StatusBroadcaster;
};
