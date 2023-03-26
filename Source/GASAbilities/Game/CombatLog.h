// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "CombatLog.generated.h"

class UAbilitySystemComponent;


USTRUCT()
struct FCombatLogData
{
	GENERATED_BODY()

	UPROPERTY()
	UAbilitySystemComponent* Attacker = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* Target = nullptr;

	UPROPERTY()
	float Amount = 0.f;

	UPROPERTY()
	bool bIsCrit = false;
};

USTRUCT()
struct FCombatLogQueue
{
	GENERATED_BODY()

private:

	UPROPERTY()
	int32 LastIndex = -1;

	UPROPERTY()
	int32 MaxElements = 100;

	UPROPERTY()
	TArray<FCombatLogData> Logs;

public:

	void Reset(uint32 NewSize);

	FORCEINLINE uint32 GetLastIndex() const { return LastIndex; }
	FORCEINLINE uint32 GetMaxElements() const { return MaxElements; }

	int32 GetActualIndex(int32 Index) const;

	void AddLog(FCombatLogData& NewLog);

	FORCEINLINE FCombatLogData const& GetCombatLogData(int32 Index) { return Logs[Index]; }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddingLogs, TArray<FCombatLogData> const&, CombatLogs);

/**
 * 
 */
UCLASS(Blueprintable)
class ACombatLog : public AInfo
{
	GENERATED_BODY()
	
public:

	ACombatLog();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	static ACombatLog* Get(UWorld const* World);

	UPROPERTY(ReplicatedUsing = OnRep_CombatLog)
	FCombatLogQueue CombatLog;

	FOnAddingLogs OnAddingLogsDelegate;

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly)
	uint32 MaxSize;

	UFUNCTION()
	void OnRep_CombatLog(FCombatLogQueue const& OldCombatLog);

};
