// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPaperCharacter.h"


#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

#include "PaperFlipbookComponent.h"
#include "EnhancedInputComponent.h"
//#include "Math/Vector.h"
//#include "Math/Vector2D.h"

#include "MyPlayerController.h"



AMyPaperCharacter::AMyPaperCharacter()
{
	
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
    

    bIsAttacking = false;
    bIsJumping = false;

    PrimaryActorTick.bCanEverTick = true;
}



void AMyPaperCharacter::BeginPlay()
{
	Super::BeginPlay();

    bIsAttacking = false;    

    InitializeAnimations();

    if (GetSprite() != nullptr)
    {
        //GetSprite()->SetFlipbook(ECharacterState::Idle);        
        UpdateAnimation(ECharacterState::Idle);
        //GetSprite 이벤트 함수 등록
        GetSprite()->OnFinishedPlaying.AddDynamic(this, &AMyPaperCharacter::OnAttackFinished);

        //UE_LOG(LogTemp, Warning, TEXT("OnFinishedPlaying.AddDynamic"));
    }
}


void AMyPaperCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateCharacter();

}


void AMyPaperCharacter::Move(float Value)
{
    if (!bIsAttacking && bIsJumping == false)
    {
        AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
    }
}


void AMyPaperCharacter::Walk()
{
    UE_LOG(LogTemp, Warning, TEXT("Walk!"));

    UpdateAnimation(ECharacterState::Walk);
}


void AMyPaperCharacter::StartJump()
{
    if (CurrentState != ECharacterState::JumpReady )
    {
        Jump();
        bIsJumping = true;

        UpdateAnimation(ECharacterState::JumpReady);
    }    
}

void AMyPaperCharacter::StopJump()
{   
    StopJumping();
    bIsJumping = false;
    UpdateAnimation(ECharacterState::Landing);    
}


void AMyPaperCharacter::Attack()
{
    // 공격 로직을 여기에 구현
    if (!bIsAttacking && bIsJumping == false && CurrentState != ECharacterState::Attack01)
    {
        bIsAttacking = true;

        UpdateAnimation(ECharacterState::Attack01);
        GetSprite()->SetLooping(false);
        GetSprite()->PlayFromStart();
    }
}


void AMyPaperCharacter::OnAttackFinished()
{

    bIsAttacking = false;
    GetSprite()->SetLooping(true);
    GetSprite()->Play();
    UpdateAnimation(ECharacterState::Idle);
    
}


void AMyPaperCharacter::UpdateCharacter()
{
    const FVector PlayerVelocity = GetVelocity();
    const float PlayerSpeed = PlayerVelocity.Size();

    if (!bIsAttacking)
    {
        if (bIsJumping)
        {

            /* if (PlayerVelocity.Z == 0)
             {
                 UpdateAnimation(ECharacterState::JumpReady);
             }*/


            switch (CurrentState)
            {
            case ECharacterState::JumpReady:
                if (PlayerVelocity.Z > 0)
                {
                    UpdateAnimation(ECharacterState::JumpUp);
                }
                break;

            case ECharacterState::JumpUp:
                if (PlayerVelocity.Z <= 0)
                {
                    UpdateAnimation(ECharacterState::Jumping);
                }
                break;

            case ECharacterState::Jumping:
                if (PlayerVelocity.Z < 0)
                {
                    UpdateAnimation(ECharacterState::Falling);
                }
                break;

            case ECharacterState::Falling:
                if (GetCharacterMovement()->IsMovingOnGround())
                {
                    StopJump();
                }
                break;

            case ECharacterState::Landing:
               
                if (PlayerSpeed > 0.0f)
                {
                    UpdateAnimation(ECharacterState::Run);
                }
                else
                {
                    UpdateAnimation(ECharacterState::Idle);
                }
                break;

            default:
                if (PlayerSpeed > 0.0f)
                {
                    UpdateAnimation(ECharacterState::Run);
                }
                else
                {
                    UpdateAnimation(ECharacterState::Idle);
                }
                break;
            }

        }
        else
        {

            if (PlayerSpeed > 0.0f)
            {
                UpdateAnimation(ECharacterState::Run);
            }
            else
            {
                UpdateAnimation(ECharacterState::Idle);
            }
            // 방향에 따른 캐릭터 회전
            TurnRight();
        }

    }
     
}



void AMyPaperCharacter::TurnRight()
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



void AMyPaperCharacter::InitializeAnimations()
{

}

void AMyPaperCharacter::UpdateAnimation(ECharacterState NewState)
{
    if (CharacterAnimations.IsEmpty()) return;

    if (CharacterAnimations.Contains(NewState))
    {
        TObjectPtr<UPaperFlipbook> DesiredAnimation = CharacterAnimations[NewState];
        if (GetSprite()->GetFlipbook() != DesiredAnimation)
        {
            GetSprite()->SetFlipbook(DesiredAnimation);
            CurrentState = NewState;            
        }
    }
}


