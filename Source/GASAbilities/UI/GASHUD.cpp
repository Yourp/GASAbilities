// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/GASHUD.h"
#include "AbilitySystemComponent.h"

AGASHUD::AGASHUD()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGASHUD::CreateFloatingCombatText(FText const& NewText, FVector const& DrawPoint, EFloatingCombatTextType AnimationType)
{
	FloatingTextManager.CreateFloatingCombatText(NewText, DrawPoint, AnimationType);
}

void AGASHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FloatingTextManager.Update(DeltaSeconds);
}

void AGASHUD::DrawHUD()
{
	Super::DrawHUD();

	FloatingTextManager.DrawAll(Canvas);
}

void AGASHUD::ConnectToCombatLogWhenItSpawned(AActor* NewActor)
{
	if (ACombatLog* CombatLog = Cast<ACombatLog>(NewActor))
	{
		CombatLog->OnAddingLogsDelegate.AddUniqueDynamic(this, &AGASHUD::CreateFloatingText);

		if (UWorld* World = GetWorld())
		{
			World->RemoveOnActorSpawnedHandler(CombatLogSpawnedHandle);
		}
	}
}

void AGASHUD::CreateFloatingText(TArray<FCombatLogData> const& NewLogs)
{
	for (FCombatLogData const& LogField : NewLogs)
	{
		if (!LogField.Target)
		{
			continue;
		}

		if (LogField.Target->GetOwner()->GetLocalRole() == ROLE_AutonomousProxy || LogField.Amount >= 0)
		{
			continue;
		}

		EFloatingCombatTextType  AnimationType  = LogField.bIsCrit ? EFloatingCombatTextType::Critical : EFloatingCombatTextType::Normal;
		FText                    FloatingText   = FText::AsNumber(FMath::Abs(LogField.Amount));
		FVector                  DrawPoint      = LogField.Target->GetOwner()->GetActorLocation();
		
		DrawPoint.Z += 80.f;

		FloatingTextManager.CreateFloatingCombatText(FloatingText, DrawPoint, AnimationType);
	}
}

void AGASHUD::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		if (ACombatLog* CombatLog = ACombatLog::Get(World))
		{
			CombatLog->OnAddingLogsDelegate.AddUniqueDynamic(this, &AGASHUD::CreateFloatingText);
		}
		else
		{
			CombatLogSpawnedHandle = World->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &AGASHUD::ConnectToCombatLogWhenItSpawned));
		}
	}
}
