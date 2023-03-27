// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "CombatLog.generated.h"

class UAbilitySystemComponent;

/** This is the data structure of one hit. It is transmitted over the network. */
USTRUCT()
struct FCombatLogData
{
	GENERATED_BODY()

	/** The one who did the damage/heal. */
	UPROPERTY()
	UAbilitySystemComponent* Attacker = nullptr;

	/** The one who took the damage/heal. */
	UPROPERTY()
	UAbilitySystemComponent* Target = nullptr;

	/** Amount of damage/healing. Below 0 is damage. Over than 0 - healing. */
	UPROPERTY()
	float Amount = 0.f;

	/** Was the healing or damage critical? */
	UPROPERTY()
	bool bIsCrit = false;
};

/** 
 * This structure is responsible for the list of hits. It contains the list of hit history. It is replicated over the network.
 * This implementation is through TArray on the principle of replacing an older log with a newer one.
 */
USTRUCT()
struct FCombatLogQueue
{
	GENERATED_BODY()

private:

	/** The number of the last updated log in the array. */
	UPROPERTY()
	int32 LastIndex = -1;

	/** How many logs we will keep in the list. */
	UPROPERTY()
	int32 MaxElements = 100;

	/** List of logs. */
	UPROPERTY()
	TArray<FCombatLogData> Logs;

public:

	/** Clears the list of logs and reserves memory for new items. */
	void Reset(uint32 NewSize);

	/** The number of the last updated log in the array. */
	FORCEINLINE uint32 GetLastIndex()   const { return LastIndex;   }

	/** How many logs we will keep in the list. */
	FORCEINLINE uint32 GetMaxElements() const { return MaxElements; }

	/** Returns the index of the item in the queue. It depends on the last updated log. */
	int32 GetActualIndex(int32 IndexInQueue) const;

	/** Adds a new log or updates the oldest log to a newer one. */
	void AddLog(FCombatLogData& NewLog);

	/** Returns the log by index in the list. */
	FORCEINLINE FCombatLogData const& GetCombatLogData(int32 Index) { return Logs[Index]; }

	/** Returns true if a log in the list exists at the specified index. */
	FORCEINLINE bool IsLogValid(int32 Index) { return Logs.IsValidIndex(Index); }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddingLogs, TArray<FCombatLogData> const&, CombatLogs);

/** This actor replicates the combat log to the clients. It is created on the server. */
UCLASS(Blueprintable)
class ACombatLog : public AInfo
{
	GENERATED_BODY()
	
public:

	ACombatLog();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Search for this actor in the world and returns the first one found. */
	static ACombatLog* Get(UWorld const* World);

	/** The structure itself, which contains a list of logs. Logs are added on the server and are replicated to the clients. */
	UPROPERTY(ReplicatedUsing = OnRep_CombatLog)
	FCombatLogQueue CombatLog;

	/** Called on clients, it allows you to work with a list of new logs added. */
	FOnAddingLogs OnAddingLogsDelegate;

protected:
	virtual void BeginPlay() override;

private:

	/** Needed to initialize the maximum number of logs in the list. */
	UPROPERTY(EditDefaultsOnly)
	uint32 MaxSize;

	/** Called on the client and here is where the new logs are defined and further work with them. */
	UFUNCTION()
	void OnRep_CombatLog(FCombatLogQueue const& OldCombatLog);
};
