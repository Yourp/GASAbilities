// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GASAbilitiesGameMode.generated.h"

class ACombatLog;

UCLASS(minimalapi)
class AGASAbilitiesGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGASAbilitiesGameMode();

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACombatLog> WorldCombatLogClass;

	UPROPERTY()
	ACombatLog* WorldCombatLog = nullptr;
};



