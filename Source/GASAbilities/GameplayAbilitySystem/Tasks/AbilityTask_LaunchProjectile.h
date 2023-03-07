// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_LaunchProjectile.generated.h"

/**
 * 
 */
UCLASS()
class UAbilityTask_LaunchProjectile : public UAbilityTask
{
	GENERATED_UCLASS_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTaskOnOutputDelegate);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskOnHitDelegate, UAbilitySystemComponent*, Victim);

public:

	UPROPERTY(BlueprintAssignable)
	FTaskOnHitDelegate OnHit;

	UPROPERTY(BlueprintAssignable)
	FTaskOnOutputDelegate OnTargetLost;

	UPROPERTY(BlueprintAssignable)
	FTaskOnOutputDelegate OnTaskDestroy;

	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_LaunchProjectile* LaunchProjectileToTarget
	(
		UGameplayAbility* OwningAbility,
		TSubclassOf<AActor> ProjectileClass,
		UAbilitySystemComponent* Target,
		float Speed = 100.f,
		float DistanceToEnd = 10.f
	);

	virtual void TickTask(float DeltaTime) override;
	virtual void InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

private:

	UPROPERTY()
	AActor* Projectile = nullptr;

	UPROPERTY(Replicated)
	UAbilitySystemComponent* Target = nullptr;

	UPROPERTY(Replicated)
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(Replicated)
	float Speed = 100.f;

	UPROPERTY(Replicated)
	float DistanceToEnd = 10.f;

	FVector CurrentLocation = FVector::ZeroVector;
	bool bCanTick = false;

	void PrepareToDestroy();

	void HitTarget();
	void TargetLost();

	FORCEINLINE void StartTicking() { bCanTick = true; }
};
