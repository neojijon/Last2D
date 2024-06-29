// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPaperZDCharacter.h"

#include "PaperZDAnimInstance.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Animation/AnimInstance.h"


AMyPaperZDCharacter::AMyPaperZDCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCharacterMovement()->GravityScale = 2.0f;
    GetCharacterMovement()->AirControl = 0.8f;
    GetCharacterMovement()->JumpZVelocity = 1000.f;
    GetCharacterMovement()->GroundFriction = 3.0f;
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;
    GetCharacterMovement()->MaxFlySpeed = 600.0f;
}

void AMyPaperZDCharacter::BeginPlay()
{
    Super::BeginPlay();
    CurrentState = ECharacterZDState::Idle;
}

void AMyPaperZDCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateAnimationState();
}

void AMyPaperZDCharacter::MoveRight(float Value)
{
    AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
    CurrentState = (Value != 0.0f) ? ECharacterZDState::Run : ECharacterZDState::Idle;
}

void AMyPaperZDCharacter::StartJump()
{
    Jump();
    CurrentState = ECharacterZDState::JumpReady;
}

void AMyPaperZDCharacter::StopJump()
{
    StopJumping();
    CurrentState = ECharacterZDState::Landing;
}

void AMyPaperZDCharacter::StartAttack()
{
    if (CurrentState != ECharacterZDState::Attack01)
    {
        CurrentState = ECharacterZDState::Attack01;
    }
}

void AMyPaperZDCharacter::UpdateAnimationState()
{
    //UPaperZDAnimInstance* AnimInstance = Cast<UPaperZDAnimInstance>(GetAnimInstance());
    //if (AnimInstance)
    //{        
    //    // BlueprintCallable 함수로 상태를 업데이트
    //    //AnimInstance->SetAnimParameter(TEXT("CharacterState"), static_cast<int32>(CurrentState));        
    //}
}

void AMyPaperZDCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMyPaperZDCharacter::MoveRight);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyPaperZDCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyPaperZDCharacter::StopJump);
    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyPaperZDCharacter::StartAttack);
}
