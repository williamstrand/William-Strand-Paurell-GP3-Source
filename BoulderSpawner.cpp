// Fill out your copyright notice in the Description page of Project Settings.


#include "BoulderSpawner.h"

TArray<AVolcanoBoulder*> ABoulderSpawner::BoulderPool;

// Sets default values
ABoulderSpawner::ABoulderSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Trigger"));
	SpawnTrigger->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Bounds"));
	SpawnArea->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called every frame
void ABoulderSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsActive) return;

	if(SpawnTimer > 0)
	{	
		SpawnTimer -= DeltaTime;
		return;
	}

	if(bPlayerInTrigger)
	{
		SpawnTimer = FMath::FRandRange(MinMaxTimeBetweenSpawns.X, MinMaxTimeBetweenSpawns.Y);
		if(IsValid(Player))
		{
			SpawnBoulder(GetRandomSpawnPoint(Player->GetActorLocation()));
		}
	}
}

/**
 * @brief Spawns a boulder at targeted location.
 * @param SpawnLocation the location to spawn the boulder at.
 */
void ABoulderSpawner::SpawnBoulder(const FVector SpawnLocation)
{
	if(BoulderPool.Num() > 0 && false)
	{
		AVolcanoBoulder* Boulder = GetBoulderFromPool();
		if (!IsValid(Boulder)) return;
		Boulder->SetActive();
		Boulder->SetActorLocation(SpawnLocation);
		Boulder->SetTargetLocation();
		return;
	}

	const FActorSpawnParameters SpawnInfo;
	auto NewBoulder = GetWorld()->SpawnActor<AVolcanoBoulder>(BoulderClass, SpawnLocation, GetActorRotation(), SpawnInfo);
	NewBoulder->SetActive();
	NewBoulder->SetTargetLocation();
}

/**
 * @brief Adds a boulder to the pool and disables it.
 * @param Boulder the boulder to add to the pool.
 */
void ABoulderSpawner::AddToBoulderPool(AVolcanoBoulder* Boulder)
{
	BoulderPool.Add(Boulder);
	Boulder->SetInactive();
}

/**
 * TODO FIX COMMENT
 * @brief Gets and removes a boulder from the pool.
 * @return the boulder from the pool.
 */
AVolcanoBoulder* ABoulderSpawner::GetBoulderFromPool()
{
	if(BoulderPool.Num() <= 0) return nullptr;

	return BoulderPool.Pop();
}

/**
 * @brief Disables the spawner.
 */
void ABoulderSpawner::Disable()
{
	bIsActive = false;
}

/**
 * @brief Enables the spawner.
 */
void ABoulderSpawner::Enable()
{
	bIsActive = true;
}

/**
 * @brief Gives a random position within the bounds of the spawn box around the player.
 * @param PlayerPosition The position of the player.
 * @return a random position within the bounds of the spawn box around the player.
 */
FVector ABoulderSpawner::GetRandomSpawnPoint(const FVector PlayerPosition) const
{
	const auto NewBoxPosition = FVector(PlayerPosition.X, PlayerPosition.Y, SpawnArea->GetComponentLocation().Z);
	SpawnArea->SetWorldLocation(NewBoxPosition);
	const auto Position = UKismetMathLibrary::RandomPointInBoundingBox(SpawnArea->GetComponentLocation(), SpawnArea->GetScaledBoxExtent());

	FHitResult HitResult;
	const FVector Start = Position;
	const FVector End = Start - GetActorUpVector() * 5000;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic);

	if(FMath::Abs(HitResult.ImpactPoint.Z - PlayerPosition.Z) > OutOfBoundsSpawnHeight) return NewBoxPosition;

	return Position;
}
