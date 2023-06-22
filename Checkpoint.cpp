// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint.h"

#include "PlayerCharacterController.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	TriggerBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	SpawnPoint->AttachToComponent(TriggerBox, FAttachmentTransformRules::KeepRelativeTransform);
}

/**
 * @brief Activates the checkpoint for the player.
 * @param Player the player to set the checkpoint for.
 */
void ACheckpoint::ActivateCheckpoint(APlayerCharacterController* Player)
{
	Player->SetCheckpoint(this);

	if (!bDisableOnActivation) return;

	TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

/**
 * @brief Gets the spawn point of the checkpoint.
 * @return an FVector representing the position of the spawn point.
 */
FVector ACheckpoint::GetSpawnPoint() const
{
	return SpawnPoint->GetComponentLocation();
}