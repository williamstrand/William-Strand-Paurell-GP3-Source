// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

UCLASS()
class GP03T06_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpoint();

	UFUNCTION(BlueprintCallable)
	FVector GetSpawnPoint() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Checkpoint")
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Checkpoint")
	USceneComponent* SpawnPoint;

	UPROPERTY(EditAnywhere, Category = Checkpoint)
	bool bDisableOnActivation = false;

	UFUNCTION(BlueprintCallable)
	void ActivateCheckpoint(APlayerCharacterController* Player);
};
