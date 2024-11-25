// Fill out your copyright notice in the Description page of Project Settings.


#include "COACharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACOACharacter::ACOACharacter() :

Health(100.0f),

MaxHealth(100.0f),

HealingRate(10.0f),

WalkSpeed(600.0f),

RunSpeed(1000.0f),

 bDead(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
}

// Called when the game starts or when spawned
void ACOACharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame DeltaTime = 1 => 1 second
// 60 fos => DeltaTime = ?
void ACOACharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bDead)
	{
		Health += FMath:: Min(MaxHealth, Health + HealingRate * DeltaTime);

	}
}

// Called to bind functionality to input
void ACOACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

