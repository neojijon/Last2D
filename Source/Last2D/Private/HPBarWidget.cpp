// Fill out your copyright notice in the Description page of Project Settings.


#include "HPBarWidget.h"

#include "Components/ProgressBar.h"

#include "Components/TextBlock.h"
#include "MyPaperZDCharacter.h"


// HPBarWidget Blueprint (UMG)
void UHPBarWidget::SetOwnerCharacter(ACharacter* Character)
{
    // 캐릭터의 HP 정보를 Progress Bar에 업데이트하는 로직을 구현합니다.
    if (Character)
    {
        AMyPaperZDCharacter* MyCharacter = Cast<AMyPaperZDCharacter>(Character);
        if (MyCharacter)
        {
            Owner = MyCharacter;
            UpdateHealthBar(MyCharacter->GetHealth() / MyCharacter->GetMaxHealth());
        }
    }
}

// HPBarWidget Blueprint (UMG)
void UHPBarWidget::UpdateHealthBar(float HealthPercent)
{    
    if (HealthProgressBar)
    {
        HealthProgressBar->SetPercent(HealthPercent);        
    }

}


void UHPBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (Owner == nullptr)
        return;

    HealthProgressBar->SetPercent(Owner->GetHealth() / Owner->GetMaxHealth());

    FNumberFormattingOptions Opts;
    Opts.SetMaximumFractionalDigits(0);

    CurrentHP->SetText(FText::AsNumber(Owner->GetHealth(), &Opts));
    MaxHP->SetText(FText::AsNumber(Owner->GetMaxHealth(), &Opts));
}
