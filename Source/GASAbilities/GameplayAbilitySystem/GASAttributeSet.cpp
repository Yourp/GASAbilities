// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/GASAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

void UGASAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, CurrentHealth,  COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, MaxHealth,      COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, CurrentEnergy,  COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, MaxEnergy,      COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, Power,          COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, Resilience,     COND_None, REPNOTIFY_Always);
}

bool UGASAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute.GetName() == "CurrentHealth")
	{
		if (Data.EvaluatedData.Magnitude < 0.f)
		{
			Data.EvaluatedData.Magnitude *= GetResilience();
		}
	}
	
	return true;
}

void UGASAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute.GetName() == "CurrentHealth")
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute.GetName() == "CurrentEnergy")
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxEnergy());
	}
}

void UGASAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, CurrentHealth, OldCurrentHealth);

	OnHealthChangeDelegate.Broadcast(GetCurrentHealth(), GetMaxHealth(), OldCurrentHealth.GetCurrentValue(), GetMaxHealth());
}

void UGASAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, MaxHealth, OldMaxHealth);

	OnHealthChangeDelegate.Broadcast(GetCurrentHealth(), GetMaxHealth(), GetCurrentHealth(), OldMaxHealth.GetCurrentValue());
}

void UGASAttributeSet::OnRep_CurrentEnergy(const FGameplayAttributeData& OldCurrentEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, CurrentEnergy, OldCurrentEnergy);

	OnEnergyChangeDelegate.Broadcast(GetCurrentEnergy(), GetMaxEnergy(), OldCurrentEnergy.GetCurrentValue(), GetMaxEnergy());
}

void UGASAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, MaxEnergy, OldMaxEnergy);

	OnEnergyChangeDelegate.Broadcast(GetCurrentEnergy(), GetMaxEnergy(), GetCurrentEnergy(), OldMaxEnergy.GetCurrentValue());
}

void UGASAttributeSet::OnRep_Power(const FGameplayAttributeData& OldPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, Power, OldPower);
}

void UGASAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, Resilience, OldResilience);
}
