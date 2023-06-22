// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "VolcanoBoulder.h"
#include "BoulderSpawner.generated.h"


UCLASS()
class GP03T06_API ABoulderSpawner : public AActor
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	ABoulderSpawner();

	static TArray<AVolcanoBoulder*> BoulderPool;
	static AVolcanoBoulder* GetBoulderFromPool();
	static void AddToBoulderPool(AVolcanoBoulder* Boulder);

protected:

	UPROPERTY(EditAnywhere, Category = "Spawning")
		UBoxComponent* SpawnArea;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		UBoxComponent* SpawnTrigger;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning", meta = (ToolTip = "The boulder blueprint"))
		TSubclassOf<AVolcanoBoulder> BoulderClass;

	void SpawnBoulder(FVector SpawnLocation);
	FVector GetRandomSpawnPoint(FVector PlayerPosition) const;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ToolTip = "The minimum and maximum time between spawns."))
		FVector2D MinMaxTimeBetweenSpawns = FVector2D();
	float SpawnTimer = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ToolTip = "The difference in height needed for spawn position to count as out of bounds."))
		float OutOfBoundsSpawnHeight = 150.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Spawning", meta = (ToolTip = "Is player in the spawn trigger."))
		bool bPlayerInTrigger = false;

	UPROPERTY(BlueprintReadWrite, Category = "Spawning", meta = (ToolTip = "Reference to the player"))
		AActor* Player;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Spawning")
		bool bIsActive = true;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
		void Disable();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
		void Enable();

	bool IsEnabled() const { return bIsActive; }

};