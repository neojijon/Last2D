// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPaperZDCharacter.h"

#include "PaperZDAnimInstance.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Animation/AnimInstance.h"


AMyPaperZDCharacter::AMyPaperZDCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    //카메라, 스프링암 컴퍼넌트를 값 세팅
  // Spring Arm Component
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 300.0f;
    SpringArm->SetRelativeRotation(FRotator(-10.0f, -90.0f, 0.0f)); //횡스크롤 화면 
    //SpringArm->SetWorldRotation(FRotator(0.0f, 0.0f, -90.0f));
    SpringArm->bDoCollisionTest = false;
    SpringArm->bInheritPitch = false;
    SpringArm->bInheritYaw = false;
    SpringArm->bInheritRoll = false;



    // Use only Yaw from the controller and ignore the rest of the rotation.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    // Set the size of our collision capsule.
    //GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
    //GetCapsuleComponent()->SetCapsuleRadius(40.0f);


    // Camera Component
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    //점프관련 수치 초기화
        // Configure character movement
    GetCharacterMovement()->GravityScale = 2.0f;
    GetCharacterMovement()->AirControl = 0.80f;
    GetCharacterMovement()->JumpZVelocity = 1000.f;
    GetCharacterMovement()->GroundFriction = 3.0f;
    GetCharacterMovement()->MaxWalkSpeed = 300.0f;
    GetCharacterMovement()->MaxFlySpeed = 600.0f;

    // Lock character motion onto the XZ plane, so the character can't move in or out of the screen
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

}

void AMyPaperZDCharacter::BeginPlay()
{
    Super::BeginPlay();
    //CurrentState = ECharacterZDState::Idle;
}

void AMyPaperZDCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    //UpdateAnimationState();
}

void AMyPaperZDCharacter::MoveRight(float Value)
{
    AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
    //CurrentState = (Value != 0.0f) ? ECharacterZDState::Run : ECharacterZDState::Idle;
}

void AMyPaperZDCharacter::StartJump()
{
    Jump();
   // CurrentState = ECharacterZDState::JumpReady;
}

void AMyPaperZDCharacter::StopJump()
{
    StopJumping();
   // CurrentState = ECharacterZDState::Landing;
}

//void AMyPaperZDCharacter::StartAttack()
//{
//    if (CurrentState != ECharacterZDState::Attack01)
//    {
//        CurrentState = ECharacterZDState::Attack01;
//    }
//}

//void AMyPaperZDCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
//{
//    Super::SetupPlayerInputComponent(PlayerInputComponent);
//    PlayerInputComponent->BindAxis("MoveRight", this, &AMyPaperZDCharacter::MoveRight);
//    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyPaperZDCharacter::StartJump);
//    PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyPaperZDCharacter::StopJump);
//    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyPaperZDCharacter::StartAttack);
//}
