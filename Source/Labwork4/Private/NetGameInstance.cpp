// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameInstance.h"
#include "Net/UnrealNetwork.h"

void UNetGameInstance::Host(FString MapName, FSPlayerInfo _PlayerInfo)
{
	GEngine->AddOnScreenDebugMessage(0, 1.0f,FColor::Red, (TEXT("Hosting Game...")));
	PlayerInfo = _PlayerInfo;
	GWorld->ServerTravel(FString::Printf(TEXT("/Game/Maps/%s?listen"), *MapName));
}

void UNetGameInstance::Join(FString Address, FSPlayerInfo _PlayerInfo)
{

	GEngine->AddOnScreenDebugMessage(0,1.0f,FColor::Green, FString::Printf(TEXT("Joining Game at %s..."), *Address));
	PlayerInfo = _PlayerInfo;
	GWorld->GetFirstPlayerController()->ClientTravel(Address,TRAVEL_Absolute);;
}

