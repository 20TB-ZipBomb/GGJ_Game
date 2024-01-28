// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GGJ_GameInstance.h"
#include "WebSocketsModule.h"
#include "JsonObjectConverter.h"
#include "Dom/JsonObject.h"

namespace
{
	const FString WebSocketURL{ "ws://127.0.0.1:4040/connect" };
	const FString MessageTypeString{ "message_type" };

	// Send
	const FString CreateLobby{ "create_lobby" };
	const FString StartGame{ "game_start" };

	// Receive
	const FString ConnectionRefused{ "connection_refused" };
	const FString LobbyCode{ "lobby_code" };
	const FString PlayerJoined{ "player_joined" };
	const FString ReceivedCards{ "received_cards" };
	const FString PlayerJobSubmittingFinished{ "player_job_submitting_finished" };
	
	

} // namespace

void UGGJ_GameInstance::Init() 
{
	Super::Init();

	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}
}

void UGGJ_GameInstance::OnStartGame()
{

	WebSocket = FWebSocketsModule::Get().CreateWebSocket(WebSocketURL);

	GEngine->AddOnScreenDebugMessage(-1, -15.0f, FColor::Green, "Successfully connected");
	UE_LOG(LogTemp, Warning, TEXT("Hello World!"));


	//Runs whenever the websocket is connected to. Attach to the 'OnConnected' event
	WebSocket->OnConnected().AddLambda([this]()
		{
			GEngine->AddOnScreenDebugMessage(-1, -15.0f, FColor::Green, "Successfully connected");
			UE_LOG(LogTemp, Warning, TEXT("Successfully connected"));

			// Create a JSON object
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

			// Add key-value pairs to the JSON object
			JsonObject->SetStringField(MessageTypeString, CreateLobby);

			// Convert the JSON object to a string
			FString JsonString;
			TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&JsonString);
			FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

			WebSocket->Send( *JsonString );

			PlayerCount++;
		} );

	//Event that triggers if an error occurs whilst connected
	WebSocket->OnConnectionError().AddLambda([](const FString& Error)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Error);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Error);
		});

	//Event that triggers upon closing the connection.
	WebSocket->OnClosed().AddLambda([](int32 StatusCode, const FString& Reason, bool bWasClean)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, bWasClean ? FColor::Green : FColor::Red, "Connection closed" + Reason);
			UE_LOG(LogTemp, Warning, TEXT("Connection closed: %s"), *Reason);
		});

	//Event that triggers upon recieving a string message
	WebSocket->OnMessage().AddLambda([this](const FString& Message)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "Recieved Message" + Message);
			UE_LOG(LogTemp, Warning, TEXT("Recieved Message %s"), *Message);

			TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create( Message );
			TSharedPtr<FJsonObject> JsonMessage;
		
			if ( !FJsonSerializer::Deserialize( JsonReader, JsonMessage ) )
			{
				UE_LOG(LogTemp, Error, TEXT( "Big Sad no Json F" ) );
				return;
			}
			
			FString messageType;
			if ( !JsonMessage->TryGetStringField( MessageTypeString, messageType) )
			{
				UE_LOG(LogTemp, Error, TEXT( "Big Sad no %s F" ), *MessageTypeString);
				return;
			}
		
			if ( messageType.Equals( ConnectionRefused ) )
			{
				UE_LOG(LogTemp, Error, TEXT( "Lmao get griefed" ) );
			}
			else if ( messageType.Equals( PlayerJoined ) )
			{
				FPlayerJoinedMessage playerJoinedMessage;
				FJsonObjectConverter::JsonObjectStringToUStruct( Message, &playerJoinedMessage, 0, 0, false );
				
				PlayerCount++;
				PlayerCountUpdated.Broadcast(PlayerCount);

			}
			else if ( messageType.Equals( LobbyCode ) )
			{
				FLobbyCodeMessage lobbyCodeMessage;
				FJsonObjectConverter::JsonObjectStringToUStruct( Message, &lobbyCodeMessage, 0, 0, false );
			}
			else if ( messageType.Equals( StartGame ) )
			{
				FGameStartMessage gameStartMessage;
				FJsonObjectConverter::JsonObjectStringToUStruct( Message, &gameStartMessage, 0, 0, false );
			}
			else if ( messageType.Equals( ReceivedCards ) )
			{
				// todo idk change the scene or smth
			}
			else if ( messageType.Equals( PlayerJobSubmittingFinished ) )
			{
				FPlayerJobSubmittingFinishedMessage playerJobSubmittingFinishedMessage;
				FJsonObjectConverter::JsonObjectStringToUStruct( Message, &playerJobSubmittingFinishedMessage, 0, 0, false );
			}			
			else
			{
				UE_LOG(LogTemp, Error, TEXT( "Client doesn't know what %s is lol" ), *messageType );
			}
		});

	//Event that triggers when client sends message to server
	WebSocket->OnMessageSent().AddLambda([](const FString& Message)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "Sent Message" + Message);
			UE_LOG(LogTemp, Warning, TEXT("Sent message %s"), *Message);
		});

	WebSocket->Connect();
}

void UGGJ_GameInstance::RequestStartGame()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(MessageTypeString, StartGame);

	// Convert the JSON object to a string
	FString JsonString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	WebSocket->Send( *JsonString );

	// TODO Change scene or whatever
}

void UGGJ_GameInstance::Shutdown()
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		WebSocket->Close();
	}

	PlayerCount--;

	Super::Shutdown();

}
