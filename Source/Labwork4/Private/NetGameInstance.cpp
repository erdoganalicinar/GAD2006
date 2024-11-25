// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameInstance.h"

void UNetGameInstance::HostGame(FString MapName, FSPlayerInfo _PlayerInfo)
{
	if(GEngine) GEngine->AddOnScreenDebugMessage(0,1.f,FColor::Red,"Hosting Game");
	PlayerInfo = _PlayerInfo;
	if(GWorld) GWorld->ServerTravel(FString::Printf(TEXT("/Game/Maps/%s?listen"), *MapName));
}

void UNetGameInstance::JoinGame(FString Address, FSPlayerInfo _PlayerInfo)
{

	if(GEngine) GEngine->AddOnScreenDebugMessage(0,1.f,FColor::Green,
		FString::Printf(TEXT("Joining Game at %s..."), *Address));
	PlayerInfo = _PlayerInfo;
	if(GWorld) GWorld->GetFirstPlayerController()->ClientTravel(Address,TRAVEL_Absolute);;
}
