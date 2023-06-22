// Fill out your copyright notice in the Description page of Project Settings.


#include "VolcanoBoulder.h"
#include "BoulderSpawner.h"
#include "PlayerCharacter.h"

// Sets default values
AVolcanoBoulder::AVolcanoBoulder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Target = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target"));
	Target->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

/**
 * @brief Finds and sets the location of the target.
 */
void AVolcanoBoulder::SetTargetLocation() const
{
	if(!IsValid(Target)) return;

	FHitResult HitResult;
	const FVector Start = GetActorLocation();
	const FVector End = Start - GetActorUpVector() * 5000;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel2);

	if(HitResult.GetActor() == nullptr) return;

	Target->SetWorldLocation(HitResult.ImpactPoint);
}

/**
 * @brief Explodes the boulder.
 */
void AVolcanoBoulder::Explode()
{
	Destroy();
	// ABoulderSpawner::AddToBoulderPool(this);
}

/**
 * @brief Logic for when the boulder collides with something.
 * @param Hit the hit result of the collision.
 */
void AVolcanoBoulder::OnCollision(FHitResult Hit)
{
	Explode();

	if (Hit.GetActor() == nullptr) return;
	if (!Hit.GetActor()->ActorHasTag("Player")) return;
	
	OnPlayerHit(Hit.GetActor());
}

/**
 * @brief Sets the actor as active.
 */
void AVolcanoBoulder::SetActive()
{
	if (!IsValid(Mesh)) return;
	if (!IsValid(Target)) return;

	Mesh->SetVisibility(true);
	Target->SetVisibility(true);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetPhysicsLinearVelocity(FVector(0, 0, 0));

}

/**
 * @brief Sets the actor as inactive.
 */
void AVolcanoBoulder::SetInactive()
{
	if (!IsValid(Mesh) || !IsValid(Target)) return;

	Mesh->SetVisibility(false);
	Target->SetVisibility(false);
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetPhysicsLinearVelocity(FVector(0, 0, 0));
}
