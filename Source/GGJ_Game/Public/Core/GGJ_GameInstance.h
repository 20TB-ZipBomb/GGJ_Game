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

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerCountUpdate, int32, PlayerCount);

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

		UPROPERTY(BlueprintAssignable)
		FOnPlayerCountUpdate PlayerCountUpdated;

		UPROPERTY(BlueprintReadWrite, Category = "WebSocket")
		int PlayerCount;

	private:
		TSharedPtr<IWebSocket> WebSocket;
};
