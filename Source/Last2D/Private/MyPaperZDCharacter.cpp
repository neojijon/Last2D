// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPaperZDCharacter.h"

#include "PaperZDAnimInstance.h"

#include "Components/WidgetComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Animation/AnimInstance.h"
#include "HPBarWidget.h" // HPBarWidget의 헤더 파일을 포함합니다


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



    // HP Widget Component 생성 및 설정
    HPWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidgetComp"));
    HPWidgetComp->SetupAttachment(RootComponent);
    HPWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
    HPWidgetComp->SetDrawSize(FVector2D(200, 50)); // HP 바 크기 설정
    //강제로 UI추가 
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/PaperZD/HPBarWidget")); // UMG 위젯 경로 설정
    if (WidgetClass.Succeeded())
    {
        HPWidgetComp->SetWidgetClass(WidgetClass.Class);
    }

    // 초기 체력 값 설정
    MaxHealth = 100.0f;
    Health = MaxHealth;
    HealthChangeRate = 10.0f;  // 초당 체력 변화량

}

void AMyPaperZDCharacter::BeginPlay()
{
    Super::BeginPlay();
    //CurrentState = ECharacterZDState::Idle;

    UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));


     // 무작위 초기 체력 설정
    Health = FMath::RandRange(0.0f, MaxHealth);
    HealthTweenDirection = FMath::RandBool() ? 1 : -1;  // 체력 변화 방향 설정

    // HPBar 위젯을 설정합니다
    UHPBarWidget* HPBar = Cast<UHPBarWidget>(HPWidgetComp->GetUserWidgetObject());
    if (HPBar)
    {
        HPBar->SetOwnerCharacter(this); // HPBar 위젯에 캐릭터를 설정합니다

        UE_LOG(LogTemp, Warning, TEXT("SetOwnerCharacter"));
    }

}

void AMyPaperZDCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);    

    UE_LOG(LogTemp, Warning, TEXT("Tick"));

    // HP를 실시간으로 업데이트
    Health += HealthChangeRate * HealthTweenDirection * DeltaTime;

    // 체력이 최대값 또는 최소값에 도달하면 방향 전환
    if (Health >= MaxHealth || Health <= 0.0f)
    {
        HealthTweenDirection *= -1;
        Health = FMath::Clamp(Health, 0.0f, MaxHealth);
    }


    //UpdateHealth(Health / MaxHealth);

    ////체력바을 업데이트 해주면됨. 호출하던지 UI쪽에서 호출하던지 하면 됨.
    //UHPBarWidget* HPBar = Cast<UHPBarWidget>(HPWidgetComp->GetUserWidgetObject());
    //if (HPBar)
    //{
    //    HPBar->UpdateHealthBar(Health / MaxHealth); // HPBar 위젯에 캐릭터를 설정합니다        
    //    UE_LOG(LogTemp, Warning, TEXT("Tick"));
    //}
    
}

void AMyPaperZDCharacter::UpdateHealth(float NewHealth)
{
    Health = NewHealth;

    // HPBar 업데이트 로직 추가
    UHPBarWidget* HPBar = Cast<UHPBarWidget>(HPWidgetComp->GetUserWidgetObject());
    if (HPBar)
    {
        // HPBar의 Progress Bar 값을 업데이트합니다.
        HPBar->UpdateHealthBar(Health / MaxHealth);
    }

}


void AMyPaperZDCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Input 관련 코드...
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

//
//void AMyPaperZDCharacter::Attack()
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
