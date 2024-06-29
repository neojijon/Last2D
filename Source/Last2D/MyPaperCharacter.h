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
	/** 케릭터가 움직임이 없을때 동작 */
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
* 2D 애니메이션에 Paper2D를 사용하는 캐릭터 클래스입니다. 
* 이 클래스는 다양한 캐릭터 상태를 관리하고 이에 따라 애니메이션을 
* 업데이트합니다. 
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

	//공격 Input 이벤트
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
	AllowPrivateAccess: 이 메타데이터는 블루프린트 편집기에서 비공개(private) 속성에 접근할 수 있게 합니다. 
	즉, 클래스의 비공개 속성이라도 블루프린트에서 해당 속성을 읽고 쓸 수 있도록 허용합니다.
	"true": 이 값은 AllowPrivateAccess를 활성화합니다. 즉, 이 속성을 true로 설정하면 해당 비공개 속성이 블루프린트에서 접근 가능해집니다.
	
	열거형(enum)과 맵(map) 구조를 사용하여 관리
	TMap을 사용 여러개의 플립북을 효율적으로 관리하고자 만듦
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterState, TObjectPtr<class UPaperFlipbook>> CharacterAnimations;

private:
	// 카메라와 Spring Arm Components 를  추가함.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;
	
};
