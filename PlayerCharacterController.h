// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Checkpoint.h"
#include "PlayerCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class GP03T06_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()


public:
	APlayerCharacterController();

	UFUNCTION(BlueprintCallable, Category = Checkpoint)
	void SetCheckpoint(ACheckpoint* Checkpoint);

	UFUNCTION(BlueprintCallable, Category = Checkpoint)
	void Respawn();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Checkpoint")
	ACheckpoint* CurrentCheckpoint;
	
};