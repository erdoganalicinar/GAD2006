// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameMode.h"

#include "Components/CapsuleComponent.h"
#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "NetAvatar.h"
#include "NetBaseCharacter.h"
#include "NetGameState.h"
#include "NetPlayerState.h"

ANetGameMode::ANetGameMode()
{
	DefaultPawnClass = ANetBaseCharacter::StaticClass();
	PlayerStateClass = ANetPlayerState::StaticClass();
	GameStateClass = ANetGameState::StaticClass();
}

AActor* ANetGameMode::GetPlayerStart(FString Name, int Index)
{
	FName PSName = (Index < 0) ? *Name : *FString::Printf(TEXT("%s%d"), *Name, Index % 4);

	for (TActorIterator<APlayerStart> It(GWorld); It; ++It)
	{
		if (APlayerStart* PS = Cast<APlayerStart>(*It))
		{
			if (PS->PlayerStartTag == PSName) return *It;
		}
	}

	return nullptr;
}

AActor* ANetGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	AActor* Start = AssignTeamAndPlayerStart(Player);
	return Start ? Start : Super::ChoosePlayerStart_Implementation(Player);
}

AActor* ANetGameMode::AssignTeamAndPlayerStart(AController* Player)
{
	AActor* Start = nullptr;
	ANetPlayerState* PlayerState = Player->GetPlayerState<ANetPlayerState>();
	if (PlayerState)
	{
		if (TotalGames == 0)
		{
			PlayerState->TeamID = TotalPlayerCount == 0 ? EPlayerTeam::TEAM_Blue : EPlayerTeam::TEAM_Red;
			PlayerState->PlayerIndex = TotalPlayerCount++;
			AllPlayers.Add(Cast<APlayerController>(Player));
		}
		else PlayerState->TeamID = PlayerState->Result == EGameResults::RESULT_Won ? EPlayerTeam::TEAM_Blue : EPlayerTeam::TEAM_Red;

		Start = (PlayerState->TeamID == EPlayerTeam::TEAM_Blue) ? GetPlayerStart("Blue", -1) : GetPlayerStart("Red", PlayerStartIndex++);
	}

	return Start;
}

void ANetGameMode::AvatarsOverlapped(ANetAvatar* AvatarA, ANetAvatar* AvatarB)
{
	ANetGameState* GameStateOpen = GetGameState<ANetGameState>();

	if (GameStateOpen==nullptr || GameStateOpen->WinningPlayer >=0) return;

	ANetPlayerState* PStateA = AvatarA->GetPlayerState<ANetPlayerState>();// AvatarA->GetPlayerState<ANetPlayerState>(); //Complier error = no matching member function for call to 'GetPlayerState'
	ANetPlayerState* PStateB = AvatarB->GetPlayerState<ANetPlayerState>();//Cast<ANetPlayerState>(AvatarB->GetPlayerState());// error: no matching member function for call to 'GetPlayerState'

	GameStateOpen->WinningPlayer = (PStateA->TeamID == EPlayerTeam::TEAM_Red) ? PStateA->PlayerIndex : PStateB->PlayerIndex;

	AvatarA->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	AvatarB->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GameStateOpen->OnVictory();

	for (APlayerController* Player : AllPlayers)
	{
		auto PState = Player->GetPlayerState<ANetPlayerState>();

		PState->Result = (PState->TeamID == EPlayerTeam::TEAM_Blue) ? EGameResults::RESULT_Lost : EGameResults::RESULT_Won;
	}
	FTimerHandle EndGameTimerHandle;
	GWorld->GetTimerManager().SetTimer(EndGameTimerHandle, this, &ANetGameMode::EndGame, 2.5f, false);
}

void ANetGameMode::EndGame()
{
	PlayerStartIndex = 0;
	TotalGames++;
	GetGameState<ANetGameState>()->WinningPlayer = -1;

	for (APlayerController* Player : AllPlayers)
	{
		APawn* Pawn = Player->GetPawn();
		Player->UnPossess();
		Pawn->Destroy();
		Player->StartSpot.Reset();
		RestartPlayer(Player);
	}

	ANetGameState* GameStateOpen = GetGameState<ANetGameState>();
	GameStateOpen->TriggerRestart();
}