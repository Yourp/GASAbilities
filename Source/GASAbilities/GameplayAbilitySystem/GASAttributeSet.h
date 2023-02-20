// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GASAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)           \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)               \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)               \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChange, float, CurrentHealth, float, MaxHealth, float, OldHealth, float, OldMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnEnergyChange, float, CurrentEnergy, float, MaxEnergy, float, OldEnergy, float, OldMaxEnergy);

/**
 * 
 */
UCLASS()
class UGASAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	ATTRIBUTE_ACCESSORS(UGASAttributeSet, CurrentHealth);
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, CurrentEnergy);
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, MaxEnergy);
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, Power);
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, Resilience);

	FOnHealthChange OnHealthChangeDelegate;
	FOnEnergyChange OnEnergyChangeDelegate;


	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

private:

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	FGameplayAttributeData CurrentHealth;

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentEnergy)
	FGameplayAttributeData CurrentEnergy;

	UPROPERTY(ReplicatedUsing = OnRep_MaxEnergy)
	FGameplayAttributeData MaxEnergy;

	UPROPERTY(ReplicatedUsing = OnRep_Power)
	FGameplayAttributeData Power;

	UPROPERTY(ReplicatedUsing = OnRep_Resilience)
	FGameplayAttributeData Resilience;

	UFUNCTION()
	virtual void OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_CurrentEnergy(const FGameplayAttributeData& OldCurrentEnergy);

	UFUNCTION()
	virtual void OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy);

	UFUNCTION()
	virtual void OnRep_Power(const FGameplayAttributeData& OldPower);

	UFUNCTION()
	virtual void OnRep_Resilience(const FGameplayAttributeData& OldResilience);
};
