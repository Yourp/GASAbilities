// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Tasks/AbilityTask_LaunchProjectile.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystemComponent.h"

UAbilityTask_LaunchProjectile::UAbilityTask_LaunchProjectile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bSimulatedTask  = true;
	bTickingTask    = true;
}

void UAbilityTask_LaunchProjectile::Activate()
{
	if (!ProjectileClass)
	{
		return;
	}

	check(AbilitySystemComponent);

	AActor const* Avatar = AbilitySystemComponent->GetAvatarActor();
	check(Avatar);

	UWorld* World = GetWorld();
	check(World);

	CurrentLocation = Avatar->GetActorLocation();

	FTimerHandle Timer;
	World->GetTimerManager().SetTimer(Timer, this, &UAbilityTask_LaunchProjectile::StartTicking, 0.1f, false);

	if (Ability && Ability->GetCurrentActorInfo()->IsNetAuthority())
	{
		return;
	}

	Projectile = World->SpawnActor(ProjectileClass, &Avatar->GetTransform());
}

void UAbilityTask_LaunchProjectile::PrepareToDestroy()
{
	const float DelayOfDestruction = 5.f;

	if (Projectile && !Projectile->GetLifeSpan())
	{
		for (UActorComponent* Component : Projectile->GetComponents())
		{
			if (Component->IsA<UParticleSystemComponent>())
			{
				Component->SetActive(false);
				continue;
			}

			if (USceneComponent* SceneComponent = Cast<USceneComponent>(Component))
			{
				SceneComponent->SetVisibility(false);
			}
		}
		Projectile->SetLifeSpan(DelayOfDestruction);
	}

	UWorld* World = GetWorld();
	check(World);

	FTimerHandle Timer;
	World->GetTimerManager().SetTimer(Timer, this, &UAbilityTask_LaunchProjectile::EndTask, DelayOfDestruction, false);
}

UAbilityTask_LaunchProjectile* UAbilityTask_LaunchProjectile::LaunchProjectileToTarget(UGameplayAbility* OwningAbility, TSubclassOf<AActor> ProjectileClass, UAbilitySystemComponent* Target, float Speed /*= 100.f*/, float DistanceToEnd /*= 10.f */)
{
	UAbilityTask_LaunchProjectile* TaskObject = NewAbilityTask<UAbilityTask_LaunchProjectile>(OwningAbility);

	TaskObject->ProjectileClass  = ProjectileClass;
	TaskObject->Target           = Target;
	TaskObject->Speed            = Speed;
	TaskObject->DistanceToEnd    = DistanceToEnd;

	return TaskObject;
}

void UAbilityTask_LaunchProjectile::TickTask(float DeltaTime)
{
	if (!bCanTick)
	{
		return;
	}

	if (TaskState == EGameplayTaskState::Finished && !bWasSuccessfullyDestroyed)
	{
		OnDestroy(!bIsSimulating);
		return;
	}

	if (!Target)
	{
		TargetLost();
		return;
	}
	
	AActor const* TargetAvatar = Target->GetAvatarActor();

	if (!TargetAvatar)
	{
		TargetLost();
		return;
	}

	FVector const  TargetLocation  = TargetAvatar->GetActorLocation();
	FVector        Direction       = TargetLocation - CurrentLocation;

	Direction.Normalize();

	float   const  Distance  = FVector::Distance(CurrentLocation, TargetLocation) - DistanceToEnd;
	FVector const  Movement  = Direction * Speed * DeltaTime;

	if (Distance <= Movement.Size())
	{
		if (Projectile)
		{
			Projectile->SetActorLocation(TargetLocation);
		}

		UWorld* World = GetWorld();
		check(World);

		FTimerHandle Timer;
		World->GetTimerManager().SetTimer(Timer, this, &UAbilityTask_LaunchProjectile::HitTarget, 0.1f, false);
		bCanTick = false;
		return;
	}

	CurrentLocation += Movement;

	if (Projectile)
	{
		Projectile->SetActorLocation(CurrentLocation);
	}
}

void UAbilityTask_LaunchProjectile::HitTarget()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnHit.Broadcast(Target);
	}

	PrepareToDestroy();
}

void UAbilityTask_LaunchProjectile::TargetLost()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTargetLost.Broadcast();
	}

	PrepareToDestroy();
}

void UAbilityTask_LaunchProjectile::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);

	OnTaskDestroy.Broadcast();
}

void UAbilityTask_LaunchProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UAbilityTask_LaunchProjectile, Target,          COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(UAbilityTask_LaunchProjectile, ProjectileClass, COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(UAbilityTask_LaunchProjectile, Speed,           COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(UAbilityTask_LaunchProjectile, DistanceToEnd,   COND_SimulatedOnly);
}

void UAbilityTask_LaunchProjectile::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
	Super::InitSimulatedTask(InGameplayTasksComponent);

	Activate();
}
