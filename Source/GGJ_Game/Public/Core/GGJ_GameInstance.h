// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "IWebSocket.h"
#include "Components/TextBlock.h"
#include "GGJ_GameInstance.generated.h"
USTRUCT()
struct FLobbyCodeMessage
{
	GENERATED_BODY()

	UPROPERTY()
	FString message_type;
	
	UPROPERTY()
	uint32 lobby_code;
};

USTRUCT()
struct FPlayerStruct
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString player_id;

	UPROPERTY()
	FString name;
};

USTRUCT()
struct FPlayerJoinedMessage
{
	GENERATED_BODY()

	UPROPERTY()
	FString message_type;

	UPROPERTY()
	FPlayerStruct player;
};

USTRUCT()
struct FPlayerJobSubmittingFinishedMessage
{
	GENERATED_BODY()

	UPROPERTY()
	FString message_type;

	UPROPERTY()
	uint32 player_id;
};

USTRUCT()
struct FGameStartMessage
{
	GENERATED_BODY()

	UPROPERTY()
	FString message_type;

	UPROPERTY()
	uint32 number_of_jobs;
};

USTRUCT(BlueprintType)
struct FCardMessage
{
	GENERATED_BODY()

	UPROPERTY()
	FString card_id;

	UPROPERTY()
	FString job_text;
};

USTRUCT(BlueprintType)
struct FPlayerImprovMessage
{
	GENERATED_BODY()

	UPROPERTY()
	FString message_type;

	UPROPERTY()
	FString player_id;

	UPROPERTY()
	FCardMessage picked_card;

	UPROPERTY()
	FCardMessage job_card;

	UPROPERTY()
	uint32 time_in_seconds;
};

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerCountUpdate, int32, PlayerCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyCodeRecieved, int32, LobbyCode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnImprovStart, FPlayerImprovMessage, ImprovMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayersSubmittedJobs);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayersSubmittedExperience);

UCLASS()
class GGJ_GAME_API UGGJ_GameInstance : public UGameInstance
{
	GENERATED_BODY()

	public:
		virtual void Init() override;

		UFUNCTION(BlueprintCallable, Category = "WebSocket")
		virtual void Shutdown() override;

		UFUNCTION(BlueprintCallable, Category = "WebSocket")
		void OnStartGame();
	
		UFUNCTION(BlueprintCallable, Category = "WebSocket")
		void RequestStartGame();

		UPROPERTY(BlueprintAssignable, Category = "WebSocket")
		FOnPlayerCountUpdate PlayerCountUpdated;

		UPROPERTY(BlueprintAssignable, Category = "WebSocket")
		FOnLobbyCodeRecieved LobbyCodeRecieved;

		UPROPERTY(BlueprintAssignable, Category = "WebSocket")
		FOnPlayersSubmittedJobs PlayersSubmittedJobs;

		UPROPERTY(BlueprintAssignable, Category = "WebSocket")
		FOnPlayersSubmittedExperience PlayersSubmittedExperience;

		UPROPERTY(BlueprintAssignable, Category = "WebSocket")
		FOnImprovStart ImprovStart;

		UPROPERTY(BlueprintReadWrite, Category = "WebSocket")
		int32 PlayerCount;

		UPROPERTY(BlueprintReadWrite, Category = "WebSocket")
		int32 CurrentLobbyCode;

		UPROPERTY(BlueprintReadWrite, Category = "WebSocket")
		TMap<FString, FString> CurrentPlayers;

	private:
		TSharedPtr<IWebSocket> WebSocket;
};
