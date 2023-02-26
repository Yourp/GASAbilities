// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Tasks/AbilityTask_Casting.h"
#include "AbilitySystemGlobals.h"
#include "GameplayAbilitySystem/GASAbilitySystemComponent.h"

void UAbilityTask_Casting::Activate()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (UGASAbilitySystemComponent* GASAbilityComponent = Cast<UGASAbilitySystemComponent>(AbilitySystemComponent))
	{
		GASAbilityComponent->SetCastingTask(this);
	}

	if (UWorld* World = GetWorld())
	{
		for (FCancelTagData& TagData : InterruptTagDelegates)
		{
			TagData.Delegate = AbilitySystemComponent->RegisterGameplayTagEvent(TagData.Tag).AddUObject(this, &UAbilityTask_Casting::GameplayTagCallback);
		}
		World->GetTimerManager().SetTimer(TimerHandle, this, &UAbilityTask_Casting::OnCastingSuccess, Time, false);
	}
}

void UAbilityTask_Casting::OnDestroy(bool AbilityIsEnding)
{
	if (AbilitySystemComponent)
	{
		if (UGASAbilitySystemComponent* GASAbilityComponent = Cast<UGASAbilitySystemComponent>(AbilitySystemComponent))
		{
			GASAbilityComponent->SetCastingTask(nullptr);
		}

		for (FCancelTagData const& TagData : InterruptTagDelegates)
		{
			AbilitySystemComponent->RegisterGameplayTagEvent(TagData.Tag).Remove(TagData.Delegate);
		}
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}

	Super::OnDestroy(AbilityIsEnding);
}

void UAbilityTask_Casting::GameplayTagCallback(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount <= 0)
	{
		return;
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnInterrupt.Broadcast();
	}
	EndTask();
}

UAbilityTask_Casting* UAbilityTask_Casting::WaitCasting(UGameplayAbility* OwningAbility, float Time, FGameplayTagContainer InterruptTags)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Duration(Time);

	UAbilityTask_Casting* TaskObject  = NewAbilityTask<UAbilityTask_Casting>(OwningAbility);
	TaskObject->Time                  = Time;

	TaskObject->InterruptTagDelegates.Empty(InterruptTags.Num());

	for (FGameplayTag const& Tag : InterruptTags)
	{
		TaskObject->InterruptTagDelegates.Add(FCancelTagData(Tag));
	}

	return TaskObject;
}

float UAbilityTask_Casting::GetProgress() const
{
	if (UWorld* World = GetWorld())
	{
		const float TimerElapsed = World->GetTimerManager().GetTimerElapsed(TimerHandle);

		if (TimerElapsed >= 0.f)
		{
			return TimerElapsed / Time;
		}
	}
	return -1.f;
}

void UAbilityTask_Casting::OnCastingSuccess()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnSuccess.Broadcast();
	}
	EndTask();
}
