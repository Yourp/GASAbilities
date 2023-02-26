// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GASAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class UGASAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	UFUNCTION(BlueprintPure)
	virtual UAbilitySystemComponent* GetSelectedTarget() const;

private:

	UPROPERTY(EditDefaultsOnly)
	bool bIsPositive = false;
};
