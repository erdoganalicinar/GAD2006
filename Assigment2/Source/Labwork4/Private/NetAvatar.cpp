// Fill out your copyright notice in the Description page of Project Settings.


#include "NetAvatar.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
ANetAvatar::ANetAvatar() :
	MovementScale(1.f)
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	if(SpringArm==nullptr) return;
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if(Camera) Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

void ANetAvatar::BeginPlay()
{
	Super::BeginPlay();

	if(Camera) Camera->bUsePawnControlRotation = false;
	if(SpringArm) SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ANetAvatar::SetupPlayerInputComponent(class UInputComponent* inputComponent)
{
	Super::SetupPlayerInputComponent(inputComponent);

	inputComponent->BindAxis("Turn",this,&ACharacter::AddControllerYawInput);
	inputComponent->BindAxis("Lookup",this,&ACharacter::AddControllerPitchInput);

	inputComponent->BindAxis("MoveForward",this,&ANetAvatar::MoveForward);
	inputComponent->BindAxis("MoveRight",this,&ANetAvatar::MoveRight);
}

void ANetAvatar::MoveForward(float Val)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(.0f,Rotation.Yaw,.0f);
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementScale * Val);
}

void ANetAvatar::MoveRight(float Val)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(.0f,Rotation.Yaw,.0f);
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(ForwardDirection, MovementScale * Val);
}
