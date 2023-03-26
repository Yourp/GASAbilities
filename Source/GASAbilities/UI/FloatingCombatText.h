// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FloatingCombatText.generated.h"

/**
 * 
 */
class UCurveVector;
class UCurveLinearColor;
class UCurveFloat;

UENUM()
enum class EFloatingCombatTextType : uint8
{
    Normal,
    Critical,

    Max
};

USTRUCT()
struct FFloatingCombatText
{
    GENERATED_BODY()

private:

    friend struct FFloatingCombatTextManager;

    EFloatingCombatTextType Type;

    float Duration;

    FVector StartPoint;

    FText FloatingText;

    FString FloatingTextInString;

public:

    FORCEINLINE uint8 GetType() const
    {
        return (uint8) Type;
    }
};

USTRUCT()
struct FFloatingCombatTextAnimationTemplate
{
    GENERATED_BODY()

private:

    friend struct FFloatingCombatTextManager;

    UPROPERTY(EditAnywhere)
    UCurveVector* PositionAnimationCurve;

    UPROPERTY(EditAnywhere)
    UCurveLinearColor* ColorAnimationCurve;

    UPROPERTY(EditAnywhere)
    UCurveFloat* TextSizeAnimationCurve;

    UPROPERTY(EditAnywhere)
    UFont* CombatTextFont;

    UPROPERTY(EditAnywhere)
    float BaseDuration;
};

USTRUCT(BlueprintType)
struct FFloatingCombatTextManager
{
    GENERATED_BODY()

private:

    UPROPERTY(EditAnywhere)
    TArray<FFloatingCombatTextAnimationTemplate> AnimationTemplates;

    UPROPERTY()
    TArray<FFloatingCombatText> AllFloatingCombatTexts;

    bool IsFloatingCombatTextExpired(FFloatingCombatText const& CombatText) const;

    void DrawFloatingCombatText(FFloatingCombatText const& CombatText, UCanvas* Canvas);

public:

    void Update(float Delta);

    void DrawAll(UCanvas* Canvas);

    void CreateFloatingCombatText(FText const& NewText, FVector const& DrawPoint, EFloatingCombatTextType AnimationType);
    void CreateFloatingCombatText(int32 Value, FVector const& DrawPoint, EFloatingCombatTextType AnimationType);
};
