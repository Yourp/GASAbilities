// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Cues/AuraFXGameplayCue.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

bool AAuraFXGameplayCue::WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	bHasHandledOnRemoveEvent = false;

	if (bHideWhenRemove)
	{
		Super::WhileActive_Implementation(MyTarget, Parameters);
	}

	if (bDeactivateWhenRemove)
	{
		for (UActorComponent* Component : GetComponents())
		{
			Component->SetActive(true, true);
		}
	}

	return false;
}

bool AAuraFXGameplayCue::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (bHideWhenRemove)
	{
		Super::OnRemove_Implementation(MyTarget, Parameters);
	}

	if (bDeactivateWhenRemove)
	{
		for (UActorComponent* Component : GetComponents())
		{
			Component->SetActive(false);
		}
	}

	return false;
}

void AAuraFXGameplayCue::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	if (EventType == EGameplayCueEvent::Removed && Parameters.MatchedTagName.IsValid())
	{
		if (IAbilitySystemInterface const* TargetAbility = Cast<IAbilitySystemInterface>(MyTarget))
		{
			if (TargetAbility->GetAbilitySystemComponent())
			{
				if (TargetAbility->GetAbilitySystemComponent()->HasMatchingGameplayTag(Parameters.MatchedTagName))
				{
					return;
				}
			}
		}
	}

	Super::HandleGameplayCue(MyTarget, EventType, Parameters);
}

