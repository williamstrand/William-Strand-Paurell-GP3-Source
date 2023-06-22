// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "BoulderSpawner.h"
#include "BossAbility.h"
#include "BA_BasicAttack.h"
#include "BA_LavaBeam.h"
#include "BA_LavaFloor.h"
#include "Boss.generated.h"

UCLASS()
class GP03T06_API ABoss : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoss();
	virtual void Tick(float DeltaTime) override;
	
	void TurnToPlayer(float, float);
	FRotator GetRotationToPlayer(FVector Offset) const;

	UChildActorComponent* GetLavaPoint1() { return LavaBeamSpawn_1; }
	UChildActorComponent* GetLavaPoint2() { return LavaBeamSpawn_2; }
	UChildActorComponent* GetPoolPoint() { return LavaPoolSpawn; }
	UChildActorComponent* GetProjectilePoint() { return ProjectileSpawn; }

	UFUNCTION(BlueprintCallable)
	float GetTurnSpeed() { return TurnSpeed; }
	UFUNCTION(BlueprintCallable)
	void SetTurnSpeed(float NewSpeed) { TurnSpeed = NewSpeed; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Boss|Attacking")
	float AttackCooldown;

	float AttackTimer;

	UPROPERTY(EditAnywhere, Category = "Boss|Attacking")
	TArray<UBossAbility*> AttackList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Attacking")
	UBA_LavaBeam* LavaBeam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Attacking")
	UBA_LavaFloor* LavaFloor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Attacking")
	UBA_BasicAttack* BasicAttack;

	UPROPERTY()
	UBossAbility* CurrentAttack;

	UPROPERTY(EditAnywhere, Category = "Boss|Attacking")
	ABoulderSpawner* BoulderSpawner;

	UFUNCTION(BlueprintCallable, Category = "Boss")
	void StartFight();

	void Attack();

	bool bFightStarted = false;
	bool bBasicAttackNext = true;

	UPROPERTY(EditAnywhere, Category = "Boss")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Boss")
	UCapsuleComponent* Collider;

	UPROPERTY(EditAnywhere, Category = "Boss")
	UBoxComponent* FightStartTrigger;

	UPROPERTY(EditAnywhere, Category = "Boss")
	APlayerController* Player;

	UPROPERTY(EditAnywhere, Category = "Boss")
	float TurnSpeed = 2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Attacking")
	UChildActorComponent* LavaBeamSpawn_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Attacking")
	UChildActorComponent* LavaBeamSpawn_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Attacking")
	UChildActorComponent* LavaPoolSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Attacking")
	UChildActorComponent* ProjectileSpawn;

	UFUNCTION(BlueprintCallable, Category = "Boss")
	void Cleanup();
};
