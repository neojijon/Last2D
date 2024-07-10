// Fill out your copyright notice in the Description page of Project Settings.


#include "PaperZDBse.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

#include "EnhancedInputComponent.h"
#include "PaperFlipbookComponent.h"

#include "PaperZDAnimInstance.h"
#include "Components/InputComponent.h"
#include "Animation/AnimInstance.h"
//#include "Animation/PaperZDComponent.h"
#include "PaperZDAnimationComponent.h" // UPaperZDAnimationComponent 헤더 파일 포함



APaperZDBse::APaperZDBse()
{

    // 애니메이션 컴포넌트를 생성하고 기본 컴포넌트로 설정
    AnimationComponent = CreateDefaultSubobject<UPaperZDAnimationComponent>(TEXT("AnimationComponent"));

    // 컴포넌트를 루트 컴포넌트에 첨부
    if (AnimationComponent)
    {
        AddInstanceComponent(AnimationComponent);
        AnimationComponent->RegisterComponent();
    }
    


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


    //PrimaryActorTick.bCanEverTick = true;
}



void APaperZDBse::BeginPlay()
{
    Super::BeginPlay();

    //bIsAttacking = false;

    //InitializeAnimations();

    //if (GetSprite() != nullptr)
    //{
    //    //GetSprite()->SetFlipbook(ECharacterState::Idle);        
    //    UpdateAnimation(ECharacterState::Idle);
    //    //GetSprite 이벤트 함수 등록
    //    GetSprite()->OnFinishedPlaying.AddDynamic(this, &APaperZDBse::OnAttackFinished);

    //    //UE_LOG(LogTemp, Warning, TEXT("OnFinishedPlaying.AddDynamic"));
    //}
}


void APaperZDBse::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}



//void APaperZDBse::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//    Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//    // Input 관련 코드...
//}

void APaperZDBse::MoveRight(float Value)
{   
    AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);

    TurnRight();    
}


void APaperZDBse::Walk()
{
    UE_LOG(LogTemp, Warning, TEXT("Walk!"));
}


void APaperZDBse::StartJump()
{
    Super::Jump();
}

void APaperZDBse::StopJump()
{
    Super::StopJumping();    
}




void APaperZDBse::TurnRight()
{
    const FVector PlayerVelocity = GetVelocity();
    const float PlayerSpeed = PlayerVelocity.Size();

    // 방향에 따른 캐릭터 회전
    if (Controller != nullptr)
    {

        if (PlayerVelocity.X < 0.0f)
        {
            GetSprite()->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
        }
        else if (PlayerVelocity.X > 0.0f)
        {
            GetSprite()->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
        }
    }
}



