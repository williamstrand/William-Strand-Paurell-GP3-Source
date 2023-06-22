// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"

APlayerCharacterController::APlayerCharacterController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

/**
 * @brief Sets the checkpoint for the player.
 * @param Checkpoint the checkpoint to set.
 */
void APlayerCharacterController::SetCheckpoint(ACheckpoint* Checkpoint)
{
	CurrentCheckpoint = Checkpoint;
}

/**
 * @brief Respawns the player at the current checkpoint.
 */
void APlayerCharacterController::Respawn()
{
	GetPawn()->SetActorLocation(CurrentCheckpoint->GetSpawnPoint());
}
