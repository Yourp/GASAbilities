// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GASAbilitySystemComponent.h"
#include "Player/GASPlayerController.h"

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
