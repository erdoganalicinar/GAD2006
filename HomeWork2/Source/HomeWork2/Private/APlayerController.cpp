// Fill out your copyright notice in the Description page of Project Settings.


#include "APlayerController.h"

AAPlayerController::AAPlayerController()
{

}

void AAPlayerController::BeginPlay()
{
	bEnableClickEvents = true;
	bShowMouseCursor = true;
	ClickEventKeys.AddUnique(EKeys::RightMouseButton);

}

void AAPlayerController::OnActorClicked(AActor* Actor, FKey key)
{
	UE_LOG(LogTemp, Warning, TEXT("OnClickedÇ %s- %s"), *Actor->GetActorLabel(), *key.ToString());
	if (GameManager) GameManager->OnActorClicked(Actor, key);

}
