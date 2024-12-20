// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameSlot.h"
#include "GameManager.h"
#include "APlayerController.generated.h"


/**
 * 
 */



UCLASS()
class HOMEWORK2_API AAPlayerController : public APlayerController
{
	GENERATED_BODY()



public:
	AAPlayerController();

	void BeginPlay() override;

	void OnActorClicked(AActor* Slot, FKey key);

	UPROPERTY(BlueprintReadWrite)
		AGameManager* GameManager;
	
};
