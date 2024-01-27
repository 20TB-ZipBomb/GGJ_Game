

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GGJ_MusicManager.generated.h"

UCLASS()
class GGJ_GAME_API AGGJ_MusicManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGGJ_MusicManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
