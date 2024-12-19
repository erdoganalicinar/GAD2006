// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NetGameInstance.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EPlayerTeam : uint8
{
	TEAM_Unknown,
	TEAM_Blue,
	TEAM_Red,
};

UENUM(BlueprintType)
enum class EGameResults : uint8
{
	RESULT_Undefined,
	RESULT_Lost,
	RESULT_Won,
};

UENUM(BlueprintType)
enum class EBodyPart : uint8
{
	BP_Face = 0,
	BP_Hair = 1,
	BP_Chest = 2,
	BP_Hands = 3,
	BP_Legs = 4,
	BP_Beard = 5,
	BP_BodyType = 6,
	BP_Count = 7,
};

USTRUCT(BlueprintType)
struct FSPlayerInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString NickName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CustomizationData;
	
	UPROPERTY(BlueprintReadOnly)
	EPlayerTeam TeamID;
};

USTRUCT(BlueprintType)
struct FSBodyPartSelection
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int Indices[(int)EBodyPart::BP_Count];
};

UCLASS()
class UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable) //BP's exposing referances as Out Params. It's Suck
	void HostGame(FString MapName, FSPlayerInfo PlayerInfo);

	UFUNCTION(BlueprintCallable)
	void JoinGame(FString Address, FSPlayerInfo PlayerInfo);

	UPROPERTY(BlueprintReadWrite)
	FSPlayerInfo PlayerInfo;
};
