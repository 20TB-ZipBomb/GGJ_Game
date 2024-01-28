// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "IWebSocket.h"
#include "Components/TextBlock.h"
#include "GGJ_GameInstance.generated.h"

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

		UPROPERTY(BlueprintAssignable)
		FOnPlayerCountUpdate PlayerCountUpdated;

		UPROPERTY(BlueprintReadWrite, Category = "WebSocket")
		int PlayerCount;

		TSharedPtr<IWebSocket> WebSocket;

};
