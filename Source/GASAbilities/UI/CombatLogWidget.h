// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/CombatLog.h"
#include "CombatLogWidget.generated.h"

class UScrollBox;
class ACombatLog;
class UTextBlock;

/**
 * 
 */
UCLASS()
class UCombatLogWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

	UPROPERTY(meta = (BindWidget))
	UScrollBox* CombatLogBox = nullptr;

	UPROPERTY(EditDefaultsOnly)
	uint32 MaxLogs = 100;

	UPROPERTY(EditDefaultsOnly)
	uint32 LogSize = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Log Color")
	FLinearColor SelfHealingColor;

	UPROPERTY(EditDefaultsOnly, Category = "Log Color")
	FLinearColor TakenDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Log Color")
	FLinearColor DefaultColor;

	FDelegateHandle CombatLogSpawnedHandle;

	UTextBlock* CreateLogText(FCombatLogData const& LogData);

	UFUNCTION()
	void UpdateLog(TArray<FCombatLogData> const& NewLogs);

protected:

	virtual void NativeConstruct() override;

public:

	void ConnectToCombatLogWhenItSpawned(AActor* NewActor);

};
