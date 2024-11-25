// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitBase.generated.h"

class AGameSlot;

UCLASS()
class HOMEWORK2_API AUnitBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnitBase();

	void AssignToSlot(AGameSlot* NewSlot);

	UFUNCTION(BlueprintImplementableEvent, BlueprintPure)
	bool IsControlledByThePlayer();

	UPROPERTY(EditAnywhere)
	FVector StartOffset;

	UPROPERTY(VisibleAnyWhere)
	AGameSlot* Slot;


};
