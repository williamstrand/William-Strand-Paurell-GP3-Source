// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VolcanoBoulder.generated.h"

UCLASS()
class GP03T06_API AVolcanoBoulder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVolcanoBoulder();

	void SetActive();
	void SetInactive();
	void SetTargetLocation() const;

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Target;

	UFUNCTION(BlueprintCallable, Category = Boulder)
	void Explode();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerHit(AActor* Player);

	UFUNCTION(BlueprintCallable, Category = Collision)
	void OnCollision(FHitResult Hit);

};
