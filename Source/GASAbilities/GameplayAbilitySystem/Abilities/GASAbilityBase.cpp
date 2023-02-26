// Fill out your copyright notice in the Description page of Project Settings.


#include "GASAbilityBase.h"
#include "GameplayAbilitySystem/GASAbilitySystemComponent.h"

bool UGASAbilityBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /*= nullptr*/, const FGameplayTagContainer* TargetTags /*= nullptr*/, OUT FGameplayTagContainer* OptionalRelevantTags /*= nullptr*/) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (!bIsPositive)
	{
		UAbilitySystemComponent* SelectedTarget = GetSelectedTarget();

		if (!SelectedTarget)
		{
			return false;
		}

		if (SelectedTarget == GetAbilitySystemComponentFromActorInfo())
		{
			return false;
		}
	}

	return true;
}

UAbilitySystemComponent* UGASAbilityBase::GetSelectedTarget() const
{
	if (UGASAbilitySystemComponent* AbilitySystemComponent = Cast<UGASAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
	{
		return AbilitySystemComponent->GetSelectedTarget();
	}

	return nullptr;
}
