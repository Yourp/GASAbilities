// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingCombatText.h"
#include "Engine/Canvas.h"
#include "Curves/CurveVector.h"
#include "Curves/CurveLinearColor.h"
#include "Engine/Font.h"

bool FFloatingCombatTextManager::IsFloatingCombatTextExpired(FFloatingCombatText const& CombatText) const
{
    return CombatText.Duration >= AnimationTemplates[CombatText.GetType()].BaseDuration;
}

void FFloatingCombatTextManager::DrawFloatingCombatText(FFloatingCombatText const& CombatText, UCanvas* Canvas)
{
    FFloatingCombatTextAnimationTemplate const& AnimTemplate = AnimationTemplates[CombatText.GetType()];

    const UCurveVector*       PositionCurve   = AnimTemplate.PositionAnimationCurve;
    const UCurveLinearColor*  ColorCurve      = AnimTemplate.ColorAnimationCurve;
    const UCurveFloat*        TextSizeCurve   = AnimTemplate.TextSizeAnimationCurve;
    const UFont*              CombatTextFont  = AnimTemplate.CombatTextFont;

    if (!PositionCurve || !ColorCurve || !TextSizeCurve || !CombatTextFont)
    {
        return;
    }

    FVector const CurrentTextPosition = CombatText.StartPoint + PositionCurve->GetVectorValue(CombatText.Duration);

    /** Transforms our 3D world space vector to 2D screen coordinates. */
    FVector Location2D = Canvas->Project(CurrentTextPosition);

    /** TODO: Fix check X and Y coordinates. */
    if (!Location2D.Z)
    {
        /** We don't need to draw, if number is behind the screen. */
        return;
    }

    const float         Scale  = TextSizeCurve->GetFloatValue(CombatText.Duration);
    const FLinearColor  Color  = ColorCurve->GetLinearColorValue(CombatText.Duration);

    float OutWidth = 0.f, OutHeight = 0.f;

    /** Extracting text size on screen. This is for text pivot. */
    Canvas->TextSize(CombatTextFont, CombatText.FloatingTextInString, OutWidth, OutHeight, Scale, Scale);

    /** The pivot is on down center. */
    Location2D.X -= OutWidth / 2.f;
    Location2D.Y -= OutHeight;

    FVector2D ShadowPosition;
    const FLinearColor ShadowColor(0, 0, 0, 0.5f * Color.A);

    ShadowPosition.X = Location2D.X + 2.f;
    ShadowPosition.Y = Location2D.Y - 2.f;

    FCanvasTextItem TextItem(ShadowPosition, CombatText.FloatingText, CombatTextFont, ShadowColor);

    TextItem.Scale.X = Scale;
    TextItem.Scale.Y = Scale;

    /** Drawing shadow. */
    Canvas->DrawItem(TextItem);

    TextItem.Position = FVector2D(Location2D);
    TextItem.SetColor(Color);

    /** Drawing text. */
    Canvas->DrawItem(TextItem);
}

void FFloatingCombatTextManager::Update(float Delta)
{
    for (int32 ItemIndex = 0; ItemIndex < AllFloatingCombatTexts.Num();)
    {
        FFloatingCombatText& CurrentFCT = AllFloatingCombatTexts.GetData()[ItemIndex];

        if (IsFloatingCombatTextExpired(CurrentFCT))
        {
            AllFloatingCombatTexts.RemoveAtSwap(ItemIndex);
            continue;
        }

        CurrentFCT.Duration += Delta;
        ItemIndex++;
    }
}

void FFloatingCombatTextManager::DrawAll(UCanvas* Canvas)
{
    for (FFloatingCombatText const& CurrentFCT : AllFloatingCombatTexts)
    {
        DrawFloatingCombatText(CurrentFCT, Canvas);
    }
}

void FFloatingCombatTextManager::CreateFloatingCombatText(FText const& NewText, FVector const& DrawPoint, EFloatingCombatTextType AnimationType)
{
    FFloatingCombatText NewFloatingText;

    NewFloatingText.FloatingText          = NewText;
    NewFloatingText.FloatingTextInString  = NewText.ToString();
    NewFloatingText.Type                  = AnimationType;
    NewFloatingText.StartPoint            = DrawPoint;

    AllFloatingCombatTexts.Add(NewFloatingText);
}

void FFloatingCombatTextManager::CreateFloatingCombatText(int32 Value, FVector const& DrawPoint, EFloatingCombatTextType AnimationType)
{
    CreateFloatingCombatText(FText::AsNumber(Value), DrawPoint, AnimationType);
}
