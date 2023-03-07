// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_Casting.generated.h"

USTRUCT()
struct FCancelTagData
{
	GENERATED_BODY()

	FCancelTagData() = default;
	FCancelTagData(FGameplayTag const& NewTag) : Tag(NewTag) {}

	FGameplayTag     Tag;
	FDelegateHandle  Delegate;
};

/**
 * 
 */
UCLASS()
class UAbilityTask_Casting : public UAbilityTask
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTaskOnSuccessDelegate);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTaskOnCancelDelegate);

public:

	UPROPERTY(BlueprintAssignable)
	FTaskOnSuccessDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FTaskOnCancelDelegate OnInterrupt;

	virtual void Activate() override;

	virtual void OnDestroy(bool AbilityIsEnding) override;

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_Casting* WaitCasting(UGameplayAbility* OwningAbility, float Time, FGameplayTagContainer InterruptTags, FGameplayTag CastingTag);

	float GetProgress() const;

private:

	void OnCastingSuccess();

	UFUNCTION()
	void GameplayTagCallback(const FGameplayTag Tag, int32 NewCount);

	float                   Time = 0.f;
	FTimerHandle            TimerHandle;
	FGameplayTag            CastingTag;
	TArray<FCancelTagData>  InterruptTagDelegates;
};
