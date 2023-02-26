// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GASAbilitySystemComponent.h"
#include "Player/GASPlayerController.h"
#include "Tasks/AbilityTask_Casting.h"

UAbilitySystemComponent* UGASAbilitySystemComponent::GetSelectedTarget() const
{
	if (AbilityActorInfo)
	{
		if (const AGASPlayerController* PlayerController = Cast<AGASPlayerController>(AbilityActorInfo->PlayerController))
		{
			if (PlayerController->GetSelectedTarget())
			{
				return PlayerController->GetSelectedTarget()->GetAbilitySystemComponent();
			}
		}
	}
	return nullptr;
}

void UGASAbilitySystemComponent::SetCastingTask(UAbilityTask_Casting* NewCastingTask)
{
	CastingTask = NewCastingTask;
}

float UGASAbilitySystemComponent::GetCastingProgress() const
{
	return CastingTask ? CastingTask->GetProgress() : -1.f;
}
