// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Net/UnrealNetwork.h"
#include "NetGameInstance.h"
#include "NetBaseCharacter.generated.h"

USTRUCT(BlueprintType)
struct FSMeshAssetList : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USkeletalMesh*> ListSkeletal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UStaticMesh*> ListStatic;
};

UCLASS()
class ANetBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANetBaseCharacter();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	FString GetCustomizationData();
	void ParseCustomizationData(FString BodyPartData);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void ChangeBodyPart(EBodyPart index, int Value, bool bDirectSet);

	UFUNCTION(BlueprintCallable)
	void ChangeGender (bool bIsFemale);
	
	UFUNCTION(Server, Reliable)
	void SubmitPlayerInfoToServer(FSPlayerInfo Info);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerInfoChanged();

	//Timer for waiting PlayerState
	UFUNCTION()
	void CheckPlayerState();

	UFUNCTION()
	void CheckPlayerInfo();

	//I might be transfer private Components into Public section idk

	bool bPlayerInfoReceived;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
private:

	UPROPERTY()
	USkeletalMeshComponent* PartFace;

	UPROPERTY()
	USkeletalMeshComponent* PartChest;

	UPROPERTY()
	UStaticMeshComponent* PartHair;

	UPROPERTY()
	UStaticMeshComponent* PartBeard;

	UPROPERTY()
	UStaticMeshComponent* PartEyes;

	UPROPERTY()
	USkeletalMeshComponent* PartHands;

	UPROPERTY()
	USkeletalMeshComponent* PartLegs;

	int BodyPartIndices[(int)EBodyPart::BP_Count];

	void UpdateBodyParts();

	static FSMeshAssetList* GetBodyPartList(EBodyPart Part, bool bIsFemale);

	FTimerHandle ClientDataCheckTimer;
};