// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "AuraFXGameplayCue.generated.h"

/**
 * 
 */
UCLASS()
class AAuraFXGameplayCue : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()
	
public:

	virtual bool WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual void HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Removing Method")
	bool bHideWhenRemove = true;

	UPROPERTY(EditDefaultsOnly, Category = "Removing Method")
	bool bDeactivateWhenRemove = false;
};
