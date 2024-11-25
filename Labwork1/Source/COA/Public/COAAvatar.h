// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "COACharacter.h"
#include "Camera/CameraComponent.h"
#include "Gameframework/SpringArmComponent.h"
#include "COAAvatar.generated.h"

/**
 * 
 */
UCLASS()
class COA_API ACOAAvatar : public ACOACharacter
{
	GENERATED_BODY()
public:
	// Called to bind functionality to input

	ACOAAvatar();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* mCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* mSpringArm;
	UPROPERTY(EditAnywhere, Category = "COA")
	float MaxStamina;
	UPROPERTY(EditAnywhere, Category = "COA")
	float Stamina;
	UPROPERTY(EditAnywhere, Category = "COA")
	float StaminaDrainRate;
	UPROPERTY(EditAnywhere, Category = "COA")
	float StaminaGainRate;

	UPROPERTY(EditAnywhere, Category = "COA")
	bool bStamina;
	UPROPERTY(EditAnywhere, Category = "COA")
	bool bStaminaDrained;
	UPROPERTY(EditAnywhere, Category = "COA")
	bool bRunning;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

	virtual void Tick(float DeltaTime) override;

private:
	void MoveForward(float value);
	void MoveRight(float value);
	void UpdateStamina();
	void RunPressed();
	void RunReleased();



	
};

