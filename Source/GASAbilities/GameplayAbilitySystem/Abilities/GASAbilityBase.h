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

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void SetCapturedTarget(UAbilitySystemComponent* NewCapturedTarget);

	UFUNCTION(BlueprintPure)
	UAbilitySystemComponent* GetCapturedTarget() const;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

private:

	UPROPERTY(EditDefaultsOnly)
	bool bIsPositive = false;

	UPROPERTY()
	UAbilitySystemComponent* CapturedTarget;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> AdditionalAbilities;
};
