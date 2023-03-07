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
		const UGASAbilitySystemComponent* AbilitySystemComponent = ActorInfo ? Cast<UGASAbilitySystemComponent>(ActorInfo->AbilitySystemComponent) : nullptr;

		if (!AbilitySystemComponent)
		{
			return false;
		}

		const UAbilitySystemComponent* SelectedTarget = AbilitySystemComponent->GetSelectedTarget();

		if (!SelectedTarget)
		{
			return false;
		}

		if (SelectedTarget == AbilitySystemComponent)
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

void UGASAbilityBase::SetCapturedTarget(UAbilitySystemComponent* NewCapturedTarget)
{
	CapturedTarget = NewCapturedTarget;
}

UAbilitySystemComponent* UGASAbilityBase::GetCapturedTarget() const
{
	return CapturedTarget;
}

void UGASAbilityBase::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (ActorInfo && ActorInfo->IsNetAuthority())
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
		{
			for (TSubclassOf<UGameplayAbility> const& AbilityClass : AdditionalAbilities)
			{
				AbilitySystemComponent->GiveAbility(AbilityClass);
			}
		}
	}
}

void UGASAbilityBase::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	if (ActorInfo && ActorInfo->IsNetAuthority())
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
		{
			for (TSubclassOf<UGameplayAbility> const& AbilityClass : AdditionalAbilities)
			{
				AbilitySystemComponent->ClearAbility(Spec.Handle);
			}
		}
	}

	Super::OnRemoveAbility(ActorInfo, Spec);
}
