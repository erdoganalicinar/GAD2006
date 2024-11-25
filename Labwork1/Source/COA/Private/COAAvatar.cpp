// Fill out your copyright notice in the Description page of Project Settings.


#include "COAAvatar.h"
#include "GameFramework/CharacterMovementComponent.h"

ACOAAvatar::ACOAAvatar():

Stamina(100),
StaminaDrainRate(10),
StaminaGainRate(1),
MaxStamina(100),
bStamina(true)
{

	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mSpringArm->TargetArmLength = 300.0f;
	mSpringArm->SetupAttachment(RootComponent);

	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mCamera->SetupAttachment(mSpringArm, USpringArmComponent::SocketName);

	mCamera->bUsePawnControlRotation = false;
	mSpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;

}

void ACOAAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACOAAvatar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACOAAvatar::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter:: StopJumping);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ACOAAvatar::RunPressed);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ACOAAvatar::RunReleased);
}

void ACOAAvatar::Tick(float DeltaTime)
{
	if (!bDead)
	{
		Health += FMath::Min(MaxHealth, Health + HealingRate * DeltaTime);

	}

	UpdateStamina();

	if (Health >= MaxHealth) 
	{
		Health = MaxHealth;
	}
}

void ACOAAvatar::MoveForward(float Amount)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, Amount);


}



void ACOAAvatar::RunPressed()
{
	if (bStamina)
	{

		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
			bRunning = true;
			
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		bRunning = false;
	}

}

void ACOAAvatar::RunReleased()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bRunning = false;
}

void ACOAAvatar:: MoveRight(float value)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, value);
}

void ACOAAvatar::UpdateStamina()
{
	if (bRunning)
	{
		Stamina -= StaminaDrainRate;
		
	}
	if (!bRunning && Stamina < MaxStamina)
	{
		Stamina += StaminaGainRate;
		
	}
	if (Stamina <= 0) {
		bStamina = false;
		bStaminaDrained = true;
		RunReleased();
	}
	if (Stamina >= MaxStamina)
	{
		bStaminaDrained = false;
	}
	if(!bStaminaDrained)
	{
			bStamina = true;
	}

	

}