// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FloatingCombatText.h"
#include "Game/CombatLog.h"
#include "GASHUD.generated.h"

/**
 * 
 */
UCLASS()
class AGASHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	AGASHUD();

	void CreateFloatingCombatText(FText const& NewText, FVector const& DrawPoint, EFloatingCombatTextType AnimationType);

	virtual void Tick(float DeltaSeconds) override;
	virtual void DrawHUD() override;

	void ConnectToCombatLogWhenItSpawned(AActor* NewActor);

	UFUNCTION()
	void CreateFloatingText(TArray<FCombatLogData> const& NewLogs);

private:

	UPROPERTY(EditAnywhere)
    FFloatingCombatTextManager FloatingTextManager;

	FDelegateHandle CombatLogSpawnedHandle;

protected:

	virtual void BeginPlay() override;

};
