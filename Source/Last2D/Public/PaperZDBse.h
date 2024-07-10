// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperZDBse.generated.h"



//class USpringArmComponent;
//class UCameraComponent;
//class UInputMappingContext;
//class UInputAction;
//struct FInputActionValue;
//template <typename T> struct TObjectPtr;


class UPaperFlipbook;
class USpringArmComponent;
class UCameraComponent;
class UPaperZDAnimationComponent;



/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class LAST2D_API APaperZDBse : public APaperCharacter
{
	GENERATED_BODY()

public:
	APaperZDBse();


protected:
	virtual void BeginPlay() override;	
	virtual void Tick(float DeltaTime) override;

public:

	void StartJump();
	void StopJump();

	void TurnRight();


	void MoveRight(float Value);

	void Walk();

	////공격 Input 이벤트
	//void Attack();

	//UFUNCTION()
	//void OnAttackFinished();



	

private:
	// Movement state
	//bool bIsAttacking;
	//bool bIsJumping;
	//FVector2D MovementInput;
//	FVector LastVelocity;

private:
	// 카메라와 Spring Arm Components 를  추가함.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	// 애니메이션 컴포넌트 선언
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperZDAnimationComponent> AnimationComponent;
};
