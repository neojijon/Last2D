// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPaperZDCharacter.h"

#include "PaperZDAnimInstance.h"

#include "Components/WidgetComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Animation/AnimInstance.h"
#include "HPBarWidget.h" // HPBarWidget�� ��� ������ �����մϴ�


AMyPaperZDCharacter::AMyPaperZDCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    //ī�޶�, �������� ���۳�Ʈ�� �� ����
  // Spring Arm Component
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 300.0f;
    SpringArm->SetRelativeRotation(FRotator(-10.0f, -90.0f, 0.0f)); //Ⱦ��ũ�� ȭ�� 
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

    //�������� ��ġ �ʱ�ȭ
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



    // HP Widget Component ���� �� ����
    HPWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidgetComp"));
    HPWidgetComp->SetupAttachment(RootComponent);
    HPWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
    HPWidgetComp->SetDrawSize(FVector2D(200, 50)); // HP �� ũ�� ����
    //������ UI�߰� 
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/PaperZD/HPBarWidget")); // UMG ���� ��� ����
    if (WidgetClass.Succeeded())
    {
        HPWidgetComp->SetWidgetClass(WidgetClass.Class);
    }

    // �ʱ� ü�� �� ����
    MaxHealth = 100.0f;
    Health = MaxHealth;
    HealthChangeRate = 10.0f;  // �ʴ� ü�� ��ȭ��

}

void AMyPaperZDCharacter::BeginPlay()
{
    Super::BeginPlay();
    //CurrentState = ECharacterZDState::Idle;

    UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));


     // ������ �ʱ� ü�� ����
    Health = FMath::RandRange(0.0f, MaxHealth);
    HealthTweenDirection = FMath::RandBool() ? 1 : -1;  // ü�� ��ȭ ���� ����

    // HPBar ������ �����մϴ�
    UHPBarWidget* HPBar = Cast<UHPBarWidget>(HPWidgetComp->GetUserWidgetObject());
    if (HPBar)
    {
        HPBar->SetOwnerCharacter(this); // HPBar ������ ĳ���͸� �����մϴ�

        UE_LOG(LogTemp, Warning, TEXT("SetOwnerCharacter"));
    }

}

void AMyPaperZDCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);    

    UE_LOG(LogTemp, Warning, TEXT("Tick"));

    // HP�� �ǽð����� ������Ʈ
    Health += HealthChangeRate * HealthTweenDirection * DeltaTime;

    // ü���� �ִ밪 �Ǵ� �ּҰ��� �����ϸ� ���� ��ȯ
    if (Health >= MaxHealth || Health <= 0.0f)
    {
        HealthTweenDirection *= -1;
        Health = FMath::Clamp(Health, 0.0f, MaxHealth);
    }


    //UpdateHealth(Health / MaxHealth);

    ////ü�¹��� ������Ʈ ���ָ��. ȣ���ϴ��� UI�ʿ��� ȣ���ϴ��� �ϸ� ��.
    //UHPBarWidget* HPBar = Cast<UHPBarWidget>(HPWidgetComp->GetUserWidgetObject());
    //if (HPBar)
    //{
    //    HPBar->UpdateHealthBar(Health / MaxHealth); // HPBar ������ ĳ���͸� �����մϴ�        
    //    UE_LOG(LogTemp, Warning, TEXT("Tick"));
    //}
    
}

void AMyPaperZDCharacter::UpdateHealth(float NewHealth)
{
    Health = NewHealth;

    // HPBar ������Ʈ ���� �߰�
    UHPBarWidget* HPBar = Cast<UHPBarWidget>(HPWidgetComp->GetUserWidgetObject());
    if (HPBar)
    {
        // HPBar�� Progress Bar ���� ������Ʈ�մϴ�.
        HPBar->UpdateHealthBar(Health / MaxHealth);
    }

}


void AMyPaperZDCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Input ���� �ڵ�...
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
