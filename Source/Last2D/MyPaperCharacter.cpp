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

    if (GetSprite() != nullptr)
    {
        GetSprite()->SetFlipbook(FB_Char_Idle);
        //GetSprite 이벤트 함수 등록
        GetSprite()->OnFinishedPlaying.AddDynamic(this, &AMyPaperCharacter::OnAttackFinished);

        UE_LOG(LogTemp, Warning, TEXT("OnFinishedPlaying.AddDynamic"));
    }
}


void AMyPaperCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //추가 구현하면 됨.
    UpdateCharacter();

}


void AMyPaperCharacter::Move(float Value)
{
    if (!bIsAttacking && bIsJumping == false)
    {
        AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
    }
}


void AMyPaperCharacter::Walk(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("Walk!"));
}


void AMyPaperCharacter::StartJump()
{
    Jump();
    bIsJumping = true;    
    GetSprite()->SetFlipbook(FB_Char_JumpReady);
}

void AMyPaperCharacter::StopJump()
{    
    StopJumping();
    bIsJumping = false;
    GetSprite()->SetFlipbook(FB_Char_Landing);
}


void AMyPaperCharacter::Attack(const FInputActionValue& Value)
{
    // 공격 로직을 여기에 구현
    if (!bIsAttacking && bIsJumping == false)
    {
        bIsAttacking = true;
        GetSprite()->SetFlipbook(FB_Char_Attack01);
        GetSprite()->SetLooping(false);        
        GetSprite()->PlayFromStart();
    }
}


void AMyPaperCharacter::OnAttackFinished()
{
    UE_LOG(LogTemp, Warning, TEXT("OnAttackFinished!"));

    bIsAttacking = false;
    GetSprite()->SetLooping(true);
    GetSprite()->Play();
    UpdateAnimation();

}



void AMyPaperCharacter::UpdateCharacter()
{   
    TurnRight();
    UpdateAnimation();
    
}



void AMyPaperCharacter::TurnRight()
{
    if (!bIsAttacking && !bIsJumping)
    {
        // Now setup the rotation of the controller based on the direction we are travelling
        const FVector PlayerVelocity = GetVelocity();
        float TravelDirection = PlayerVelocity.X;
        // Set the rotation so that the character faces his direction of travel.
        if (Controller != nullptr)
        {
            if (TravelDirection < 0.0f)
            {
                Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
            }
            else if (TravelDirection > 0.0f)
            {
                Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
            }
        }

        LastVelocity = PlayerVelocity;
    }
}


void AMyPaperCharacter::UpdateAnimation()
{
    const FVector PlayerVelocity = GetVelocity();
    const float PlayerSpeed = PlayerVelocity.SizeSquared();

    UPaperFlipbook* DesiredAnimation = FB_Char_Idle;

    if (GetCharacterMovement()->IsFalling())
    {
        if (PlayerVelocity.Z > 0)
        {
            DesiredAnimation = FB_Char_JumpUp;
        }
        else if(PlayerVelocity.Z < 0)
        {
            DesiredAnimation = FB_Char_Falling;
        }
        else
        {
            //if (bIsJumping)
            //{
                DesiredAnimation = FB_Char_JumpUp;
                bIsJumping = false;
            //}                  
        }
    }  
   /* else if (GetCharacterMovement()->IsMovingOnGround())
    {
        DesiredAnimation = FB_Char_Landing;        
    }*/
    else if (PlayerSpeed > 0.0f)
    {
        DesiredAnimation = FB_Char_Run;
    }

    if (GetSprite()->GetFlipbook() != DesiredAnimation)
    {
        GetSprite()->SetFlipbook(DesiredAnimation);
    }
}

/*
void AMyPaperCharacter::UpdateAnimation()
{
    
    if (!bIsAttacking && !bIsJumping)
    {
        const FVector PlayerVelocity = GetVelocity();
        const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

        // Are we moving or standing still?
        UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? FB_Char_Run : FB_Char_Idle;
        if (GetSprite()->GetFlipbook() != DesiredAnimation)
        {
            GetSprite()->SetFlipbook(DesiredAnimation);
        }

    }

    if (!bIsAttacking && bIsJumping) {
        if (GetSprite())
        {
            GetSprite()->SetFlipbook(FB_Char_JumpStart);
            GetSprite()->SetLooping(false);
            GetSprite()->PlayFromStart();
        }
    }
    else
    {
        if (GetSprite())
        {
            GetSprite()->SetFlipbook(FB_Char_JumpEnd);
            GetSprite()->SetLooping(false);
            GetSprite()->PlayFromStart();
        }
    }
    
}

*/
