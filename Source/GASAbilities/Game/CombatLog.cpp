// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CombatLog.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"
#include "UI/CombatLogWidget.h"

ACombatLog::ACombatLog()
{
	bReplicates      = true;
	bAlwaysRelevant  = true;

	PrimaryActorTick.bCanEverTick = false;

	CombatLog.Reset(MaxSize);
}

void ACombatLog::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACombatLog, CombatLog);
}

ACombatLog* ACombatLog::Get(UWorld const* World)
{
	if (World)
	{
		for (TActorIterator<ACombatLog> CombatLog(World, ACombatLog::StaticClass()); CombatLog; ++CombatLog)
		{
			return *CombatLog;
		}
	}

	return nullptr;
}

void ACombatLog::OnRep_CombatLog(FCombatLogQueue const& OldCombatLog)
{
	TArray<FCombatLogData> LogDifference;
	int32 DifferenceSize = CombatLog.GetLastIndex() - OldCombatLog.GetLastIndex();

	if (DifferenceSize < 0)
	{
		/** So we made a circle. We will add this to get the difference. */
		DifferenceSize += CombatLog.GetMaxElements();
	}

	LogDifference.Reserve(DifferenceSize);

	for (int32 Index = 0; Index < DifferenceSize; Index++)
	{
		const int32 ActualNewItemIndex = OldCombatLog.GetActualIndex(Index + 1);

		if (CombatLog.IsLogValid(ActualNewItemIndex))
		{
			LogDifference.Add(CombatLog.GetCombatLogData(ActualNewItemIndex));
		}
	}
	OnAddingLogsDelegate.Broadcast(LogDifference);
}

void ACombatLog::BeginPlay()
{
	Super::BeginPlay();

	CombatLog.Reset(MaxSize);
}

void FCombatLogQueue::Reset(uint32 NewSize)
{
	Logs.Empty(NewSize);
}

int32 FCombatLogQueue::GetActualIndex(int32 IndexInQueue) const
{
	return (IndexInQueue + LastIndex) % MaxElements;
}

void FCombatLogQueue::AddLog(FCombatLogData& NewLog)
{
	LastIndex = ++LastIndex % MaxElements;

	if (Logs.Num() >= MaxElements)
	{
		Logs[LastIndex] = NewLog;
		return;
	}

	Logs.Add(NewLog);
}
