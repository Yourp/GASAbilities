// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StatusBroadcaster.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatusUpdate, float, CurrentValue, float, MaxValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetUpdate, const TScriptInterface<IStatusBroadcaster>&, NewTarget);

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

	FOnTargetUpdate OnTargetUpdateDelegate;

	virtual void BroadcastHealth() = 0;
	virtual void BroadcastEnergy() = 0;
	virtual void BroadcastTarget() = 0;
};
