// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "MyPaperZDCharacter.generated.h"

class UPaperZDAnimationComponent;
class UPaperZDAnimInstance;
class UPaperZDAnimBP;

/**
 * Enum that defines the various states a character can be in.
 * This enum is used to manage and switch between different animations
 * based on the character's current state.
 */
UENUM(BlueprintType)
enum class ECharacterZDState : uint8
{
    /** The character is idle, not moving. */
    Idle UMETA(DisplayName = "Idle"),

    /** The character is walking. */
    Walk UMETA(DisplayName = "Walk"),

    /** The character is running. */
    Run UMETA(DisplayName = "Run"),

    /** The character is performing a basic attack. */
    Attack01 UMETA(DisplayName = "Attack01"),

    /** The character is preparing to jump. */
    JumpReady UMETA(DisplayName = "Jump Ready"),

    /** The character is in the initial upward movement of a jump. */
    JumpUp UMETA(DisplayName = "Jump Up"),

    /** The character is at the peak of the jump and moving horizontally. */
    Jumping UMETA(DisplayName = "Jumping"),

    /** The character is falling down after a jump. */
    Falling UMETA(DisplayName = "Falling"),

    /** The character has landed on the ground after a jump. */
    Landing UMETA(DisplayName = "Landing")
};

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

    /** Current state of the character. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    ECharacterZDState CurrentState;

private:
    UFUNCTION()
    void UpdateAnimationState();
};
