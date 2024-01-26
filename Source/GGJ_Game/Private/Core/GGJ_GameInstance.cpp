// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GGJ_GameInstance.h"
#include "WebSocketsModule.h"

void UGGJ_GameInstance::Init() 
{
	Super::Init();

	if (!FModuleManager::Get().IsModuleLoaded("WebSockets")) 
	{
		FModuleManager::Get().LoadModule("WebSockets");
	
	}

	WebSocket = FWebSocketsModule::Get().CreateWebSocket("ws://localhost:8080");

	GEngine->AddOnScreenDebugMessage(-1, -15.0f, FColor::Green, "Successfully connected");
	UE_LOG(LogTemp, Warning, TEXT("Hello World!"));


	//Runs whenever the websocket is connected to. Attach to the 'OnConnected' event
	WebSocket->OnConnected().AddLambda([]()
	{
		GEngine->AddOnScreenDebugMessage(-1, -15.0f, FColor::Green, "Successfully connected");
		UE_LOG(LogTemp, Warning, TEXT("Successfully connected"));

	});

	//Event that triggers if an error occurs whilst connected
	WebSocket->OnConnectionError().AddLambda([](const FString& Error)
	{
		GEngine->AddOnScreenDebugMessage(-1, -15.0f, FColor::Red, Error);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Error);
	});

	//Event that triggers upon closing the connection.
	WebSocket->OnClosed().AddLambda([](int32 StatusCode, const FString& Reason, bool bWasClean)
	{
		GEngine->AddOnScreenDebugMessage(-1, -15.0f, bWasClean ? FColor::Green : FColor::Red, "Connection closed" + Reason);
		UE_LOG(LogTemp, Warning, TEXT("Connection closed: %s"), *Reason);
	});

	//Event that triggers upon recieving a string message
	WebSocket->OnMessage().AddLambda([](const FString& Message)
	{
		GEngine->AddOnScreenDebugMessage(-1, -15.0f, FColor::Cyan, "Recieved Message" + Message);
		UE_LOG(LogTemp, Warning, TEXT("Recieved Message %s"), *Message);
	});

	//Event that triggers when client sends message to server
	WebSocket->OnMessageSent().AddLambda([](const FString& Message)
	{
		GEngine->AddOnScreenDebugMessage(-1, -15.0f, FColor::Cyan, "Recieved Message" + Message);
		UE_LOG(LogTemp, Warning, TEXT("Sent message %s"), *Message);
	});
	
	WebSocket->Connect();
}

void UGGJ_GameInstance::Shutdown()
{
	if (WebSocket->IsConnected())
	{
		WebSocket->Close();
	}

	Super::Shutdown();

}