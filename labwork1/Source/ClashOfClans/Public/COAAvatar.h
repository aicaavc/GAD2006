// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "COABaseChar.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "COAAvatar.generated.h"

/**
 * 
 */
UCLASS()
class CLASHOFCLANS_API ACOAAvatar : public ACOABaseChar
{
	GENERATED_BODY()
	


public: 
	ACOAAvatar();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Char")
	UCameraComponent* mCamera; 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Char")
	USpringArmComponent* mSpringArm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Char")
	float RunSpeed = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char")
	float Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char")
	float MaxStamina = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char")
	float StaminaGainRate = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char")
	float StaminaDrainRate = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char")
	bool bStaminaDrained = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char")
	bool bRunning = false;
	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:

	void UpdateMovementParams();
	void MoveForward(float value);
	void MoveRight(float value);
	void RunPressed();
	void RunReleased();
};

