// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "IWebSocket.h"
#include "../../../../../../../../../../Program Files/Epic Games/UE_5.3/Engine/Source/Runtime/Core/Public/Containers/Map.h"
#include "Components/TextBlock.h"
#include "GGJ_GameInstance.generated.h"

#pragma region Structs

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
struct FScoreSubmission
{
	GENERATED_BODY()

	UPROPERTY()
	FString message_type;

	UPROPERTY()
	int32 score_in_cents;
};

USTRUCT()
struct FPlayerJobSubmittingFinishedMessage
{
	GENERATED_BODY()

	UPROPERTY()
	FString message_type;

	UPROPERTY()
	FString player_id;
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

USTRUCT()
struct FTimerFinished
{
	GENERATED_BODY()

	UPROPERTY()
	FString message_type;
};

USTRUCT(BlueprintType, Blueprintable)
struct FCardMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString card_id;

	UPROPERTY(BlueprintReadWrite)
	FString job_text;
};

USTRUCT(BlueprintType, Blueprintable)
struct FPlayerImprovMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString message_type;

	UPROPERTY(BlueprintReadWrite)
	FString player_id;

	UPROPERTY(BlueprintReadWrite)
	FCardMessage SelectedCard;

	UPROPERTY(BlueprintReadWrite)
	FCardMessage JobCard;

	UPROPERTY(BlueprintReadWrite)
	int32 time_in_seconds;
};

USTRUCT(BlueprintType, Blueprintable)
struct FCardIntercept
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FString message_type;

	UPROPERTY(BlueprintReadWrite)
	FString player_id;

	UPROPERTY(BlueprintReadWrite)
	FCardMessage intercepted_card;

	UPROPERTY(BlueprintReadWrite)
	int32 time_in_seconds;
};

USTRUCT(BlueprintType, Blueprintable)
struct FPlayerIdMessage
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FString message_type;

	UPROPERTY(BlueprintReadWrite)
	FString player_id;
};


#pragma endregion

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCardReceived, FString, JobTitle, int, TimeExtension);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerJoin, FString, PlayerId, FString, PlayerName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerCountUpdate, int32, PlayerCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyCodeRecieved, int32, LobbyCode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnImprovStart, FPlayerImprovMessage, ImprovMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinalScoreSubmitted, int32, TotalScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerSubmittedJob, FString, PlayerId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimerEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameFinished);
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

		UFUNCTION(BlueprintCallable)
		TArray<FString> GetPlayersBySalary();

		UPROPERTY(BlueprintAssignable, Category = "WebSocket")
		FOnPlayerCountUpdate PlayerCountUpdated;

		//weird name to prevent name conflict
		UPROPERTY(BlueprintAssignable, Category = "WebSocket")
		FOnPlayerJoin PlayerJustJoined;

		UPROPERTY(BlueprintAssignable, Category = "WebSocket")
		FOnTimerEnd TimerEnd;

		//weird name to prevent name conflict
		UPROPERTY(BlueprintAssignable, Category = "WebSocket")
		FOnGameFinished GameJustFinished;

		UPROPERTY(BlueprintAssignable, Category = "WebSocket")
		FOnLobbyCodeRecieved LobbyCodeRecieved;

		UPROPERTY(BlueprintAssignable, Category = "WebSocket")
		FOnPlayersSubmittedJobs PlayersSubmittedJobs;

		UPROPERTY(BlueprintAssignable, Category = "WebSocket")
		FOnPlayerSubmittedJob PlayerSubmittedJob;

		UPROPERTY(BlueprintAssignable, Category = "WebSocket")
		FOnPlayersSubmittedExperience PlayersSubmittedExperience;

		UPROPERTY(BlueprintAssignable, Category = "WebSocket")
		FOnFinalScoreSubmitted FinalScoreSubmitted;

		UPROPERTY(BlueprintAssignable, Category = "WebSocket")
		FOnImprovStart ImprovStart;

		UPROPERTY(BlueprintAssignable, Category = "WebSocket")
		FOnCardReceived CardReceived;

		UPROPERTY(BlueprintAssignable, Category = "WebSocket")
		FOnPlayerSubmittedJob PlayerIdReceived;

		UPROPERTY(BlueprintReadWrite, Category = "WebSocket")
		int32 PlayerCount;

		UPROPERTY(BlueprintReadWrite, Category = "WebSocket")
		int32 CurrentLobbyCode;

		UPROPERTY(BlueprintReadWrite, Category = "WebSocket")
		TMap<FString, FString> CurrentPlayers;

		UPROPERTY(BlueprintReadWrite, Category = "WebSocket")
		TMap<FString, int> Salaries;

		UPROPERTY(BlueprintReadWrite, Category = "WebSocket")
		bool ImprovStarted = false;
		

	private:
		TSharedPtr<IWebSocket> WebSocket;
};
