// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GASAbilitySystemComponent.generated.h"

class UAbilityTask_Casting;

/**
 * 
 */
UCLASS()
class UGASAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintPure)
	UAbilitySystemComponent* GetSelectedTarget() const;

	void SetCastingTask(UAbilityTask_Casting* NewCastingTask);

	UFUNCTION(BlueprintPure)
	float GetCastingProgress() const;

private:

	UPROPERTY()
	UAbilityTask_Casting* CastingTask = nullptr;

};
