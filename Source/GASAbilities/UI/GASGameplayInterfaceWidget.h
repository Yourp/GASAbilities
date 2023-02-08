// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GASUserWidgetBase.h"
#include "GASGameplayInterfaceWidget.generated.h"

class UGASStatusBarWidget;
class IStatusBroadcaster;

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

	UFUNCTION()
	void OnUpdateTarget(const TScriptInterface<IStatusBroadcaster>& NewTarget);

	UFUNCTION()
	void OnUpdateHealth(float CurrentValue, float MaxValue);

	UFUNCTION()
	void OnUpdateEnergy(float CurrentValue, float MaxValue);

	UFUNCTION()
	void OnUpdateTargetHealth(float CurrentValue, float MaxValue);

	UFUNCTION()
	void OnUpdateTargetEnergy(float CurrentValue, float MaxValue);

	void SetEnergy(UGASStatusBarWidget* Bar, float CurrentValue, float MaxValue) const;
	void SetHealth(UGASStatusBarWidget* Bar, float CurrentValue, float MaxValue) const;

	UPROPERTY(meta = (BindWidget))
	UGASStatusBarWidget* SelfBar;

	UPROPERTY(meta = (BindWidget))
	UGASStatusBarWidget* TargetBar;

	UPROPERTY()
	TScriptInterface<IStatusBroadcaster> CurrentTarget;
};
