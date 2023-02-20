// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StatusBroadcaster.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatusUpdate, float, CurrentValue, float, MaxValue);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStatusBroadcaster : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IStatusBroadcaster
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	FOnStatusUpdate OnHealthUpdateDelegate;
	FOnStatusUpdate OnEnergyUpdateDelegate;

	virtual float GetCurrentHealth() const = 0;
	virtual float GetMaxHealth() const = 0;
	virtual float GetCurrentEnergy() const = 0;
	virtual float GetMaxEnergy() const = 0;

};
