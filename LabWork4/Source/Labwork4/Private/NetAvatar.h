// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "NetAvatar.generated.h"

/**
 * 
 */
UCLASS()
class ANetAvatar : public ANetBaseCharacter
{
	GENERATED_BODY()
	
public:
	
	ANetAvatar();

	//UPROPERTY(BlueprintReadWrite)
	//UCameraComponent* Camera;

	//UPROPERTY(BlueprintReadWrite)
	//USpringArmComponent* SpringArm;


	UPROPERTY(BlueprintReadWrite)
	float SpeedMultiplier;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_PlayerSpeedChanged)
	float Speed;

	UFUNCTION()
	void OnRep_PlayerSpeedChanged();

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Server, Reliable)
	void SetSpeedInServer(float sped);

private:
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void RunPressed();
	void RunReleased();
};
