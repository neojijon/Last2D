// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "MyPaperCharacter.generated.h"

class UPaperFlipbook;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
template <typename T> struct TObjectPtr;



UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	/** �ɸ��Ͱ� �������� ������ ���� */
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
* 2D �ִϸ��̼ǿ� Paper2D�� ����ϴ� ĳ���� Ŭ�����Դϴ�. 
* �� Ŭ������ �پ��� ĳ���� ���¸� �����ϰ� �̿� ���� �ִϸ��̼��� 
* ������Ʈ�մϴ�. 
*/
UCLASS(Blueprintable, BlueprintType)
class LAST2D_API AMyPaperCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	AMyPaperCharacter();


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
public:

	//virtual void Jump() override;
	//virtual void StopJumping() override;

	void StartJump();
	void StopJump();

	void TurnRight();


	void Move(float Value);

	void Walk();

	//���� Input �̺�Ʈ
	void Attack();

	UFUNCTION()	
	void OnAttackFinished();

private:

	void InitializeAnimations();
	void UpdateAnimation(ECharacterState NewState);
	void UpdateCharacter();

private:
	// Movement state
	bool bIsAttacking;
	bool bIsJumping;
	FVector2D MovementInput;
	FVector LastVelocity;

	ECharacterState CurrentState;	


public:
	/*
	AllowPrivateAccess: �� ��Ÿ�����ʹ� ��������Ʈ �����⿡�� �����(private) �Ӽ��� ������ �� �ְ� �մϴ�. 
	��, Ŭ������ ����� �Ӽ��̶� ��������Ʈ���� �ش� �Ӽ��� �а� �� �� �ֵ��� ����մϴ�.
	"true": �� ���� AllowPrivateAccess�� Ȱ��ȭ�մϴ�. ��, �� �Ӽ��� true�� �����ϸ� �ش� ����� �Ӽ��� ��������Ʈ���� ���� ���������ϴ�.
	
	������(enum)�� ��(map) ������ ����Ͽ� ����
	TMap�� ��� �������� �ø����� ȿ�������� �����ϰ��� ����
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterState, TObjectPtr<class UPaperFlipbook>> CharacterAnimations;

private:
	// ī�޶�� Spring Arm Components ��  �߰���.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;
		// Add PaperZD
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PaperZD", meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UPaperZDAnimationComponent> PaperZDAnimation;
};
