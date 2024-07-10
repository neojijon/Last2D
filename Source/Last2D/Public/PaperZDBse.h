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

	////���� Input �̺�Ʈ
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
	// ī�޶�� Spring Arm Components ��  �߰���.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	// �ִϸ��̼� ������Ʈ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperZDAnimationComponent> AnimationComponent;
};
