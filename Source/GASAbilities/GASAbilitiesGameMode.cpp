// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASAbilitiesGameMode.h"
#include "GASAbilitiesCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Game/CombatLog.h"

AGASAbilitiesGameMode::AGASAbilitiesGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (!PlayerPawnBPClass.Class)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AGASAbilitiesGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (WorldCombatLogClass)
	{
		WorldCombatLog = Cast<ACombatLog>(GetWorld()->SpawnActor(WorldCombatLogClass));
	}
}


