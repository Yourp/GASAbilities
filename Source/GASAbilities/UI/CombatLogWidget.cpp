// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CombatLogWidget.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Internationalization/Internationalization.h"
#include "AbilitySystemComponent.h"

UTextBlock* UCombatLogWidget::CreateLogText(FCombatLogData const& LogData)
{
	UTextBlock* LogText             = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
	LogText->Font.Size              = LogSize;
	LogText->ColorAndOpacity        = DefaultColor;
	LogText->ShadowColorAndOpacity  = FLinearColor::Black;
	FText TextTemplate              = FText::FromString(TEXT("Unknown"));
	
	if (LogData.Amount > 0)
	{
		if (LogData.Attacker->GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
		{
			LogText->ColorAndOpacity = SelfHealingColor;
			TextTemplate = FText::FromString(FString::Printf(TEXT("You heal yourself for %d"), static_cast<int32>(LogData.Amount)));
		}
		else
		{
			TextTemplate = FText::FromString(FString::Printf(TEXT("Enemy heals himself for %d"), static_cast<int32>(LogData.Amount)));
		}
	}
	else
	{
		if (LogData.Attacker->GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
		{
			TextTemplate = FText::FromString(FString::Printf(TEXT("You deal %d damage to the Enemy"), FMath::Abs<int32>(LogData.Amount)));
		}
		else
		{
			LogText->ColorAndOpacity = TakenDamage;
			TextTemplate = FText::FromString(FString::Printf(TEXT("You take %d damage from the Enemy"), FMath::Abs<int32>(LogData.Amount)));
		}
	}

	LogText->SetText(TextTemplate);

	return LogText;
}

void UCombatLogWidget::UpdateLog(TArray<FCombatLogData> const& NewLogs)
{
	if (!CombatLogBox)
	{
		return;
	}

	for (FCombatLogData const& LogField : NewLogs)
	{
		if (CombatLogBox->GetChildrenCount() >= static_cast<int32>(MaxLogs))
		{
			CombatLogBox->RemoveChildAt(0);
		}

		CombatLogBox->AddChild(CreateLogText(LogField));
	}

	CombatLogBox->ScrollToEnd();
}

void UCombatLogWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UWorld* World = GetWorld())
	{
		if (ACombatLog* CombatLog = ACombatLog::Get(World))
		{
			CombatLog->OnAddingLogsDelegate.AddUniqueDynamic(this, &UCombatLogWidget::UpdateLog);
		}
		else
		{
			CombatLogSpawnedHandle = World->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &UCombatLogWidget::ConnectToCombatLogWhenItSpawned));
		}
	}
}

void UCombatLogWidget::ConnectToCombatLogWhenItSpawned(AActor* NewActor)
{
	if (ACombatLog* CombatLog = Cast<ACombatLog>(NewActor))
	{
		CombatLog->OnAddingLogsDelegate.AddUniqueDynamic(this, &UCombatLogWidget::UpdateLog);

		if (UWorld* World = GetWorld())
		{
			World->RemoveOnActorSpawnedHandler(CombatLogSpawnedHandle);
		}
	}
}
