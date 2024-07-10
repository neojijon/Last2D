// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPBarWidget.generated.h"

class AMyPaperZDCharacter;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class LAST2D_API UHPBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//블루프린트에서 함수를 구현하고 싶을때 사용함.
	//UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void SetOwnerCharacter(ACharacter* Character);

	void UpdateHealthBar(float HealthPercent);

protected:
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	class AMyPaperZDCharacter* Owner;


private:
		
	//private 에  추가했을때 접근 가능하게 하려면  추가 해줘야 함. meta = (AllowPrivateAccess = "true")	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProgressBar> HealthProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> MaxHP;

	
};
