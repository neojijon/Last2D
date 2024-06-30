// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "MyPaperZDCharacter.generated.h"

class UPaperZDAnimationComponent;
class UPaperZDAnimInstance;
class UPaperZDAnimBP;

/**
 * A character class that uses PaperZD for 2D animations.
 * This class manages different character states and updates animations
 * accordingly.
 */
UCLASS(Blueprintable, BlueprintType)
class LAST2D_API AMyPaperZDCharacter : public APaperZDCharacter
{
    GENERATED_BODY()

public:
    AMyPaperZDCharacter();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
    virtual void Tick(float DeltaTime) override;

    void MoveRight(float Value);
    void StartJump();
    void StopJump();
    void StartAttack();     


private:
    // 카메라와 Spring Arm Components 를  추가함.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class USpringArmComponent> SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UCameraComponent> Camera;
};
