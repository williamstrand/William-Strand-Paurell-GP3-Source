// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABoss::ABoss()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);

	Collider = CreateDefaultSubobject<UCapsuleComponent>("Collision");
	Collider->SetupAttachment(Mesh);

	FightStartTrigger = CreateDefaultSubobject<UBoxComponent>("Fight Start Trigger");
	FightStartTrigger->SetupAttachment(RootComponent);

	LavaBeam = CreateDefaultSubobject<UBA_LavaBeam>("Beam");
	LavaFloor = CreateDefaultSubobject<UBA_LavaFloor>("Floor");
	BasicAttack = CreateDefaultSubobject<UBA_BasicAttack>("Basic Attack");
	
	LavaBeamSpawn_1 = CreateDefaultSubobject<UChildActorComponent>("LavaBeam SpawnPoint 1");
	LavaBeamSpawn_1->SetupAttachment(Mesh);
	LavaBeamSpawn_2 = CreateDefaultSubobject<UChildActorComponent>("LavaBeam SpawnPoint 2");
	LavaBeamSpawn_2->SetupAttachment(Mesh);
	LavaPoolSpawn = CreateDefaultSubobject<UChildActorComponent>("LavaPool SpawnPoint");
	LavaPoolSpawn->SetupAttachment(Mesh);
	ProjectileSpawn = CreateDefaultSubobject<UChildActorComponent>("Projectile SpawnPoint");
	ProjectileSpawn->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "BeginPlay");

	Player = GetWorld()->GetFirstPlayerController();
}

// Called every frameB
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bFightStarted) return;
	
	if(IsValid(CurrentAttack))
	{
		if(CurrentAttack->CheckActive()) return;
	}

	if(!IsValid(CurrentAttack) || !CurrentAttack->CheckActive())
	{
		AttackTimer -= DeltaTime;

		if(IsValid(BoulderSpawner))
		{
			if (!BoulderSpawner->IsEnabled())
			{
				BoulderSpawner->Enable();
			}
		}

		TurnToPlayer(DeltaTime, TurnSpeed);
	}

	if(AttackTimer <= 0)
	{
		Attack();
		AttackTimer = AttackCooldown;
	}
}

/**
 * @brief Makes boss attack.
 */
void ABoss::Attack()
{
	const auto AttackIndex = FMath::RandRange(0, AttackList.Num() - 1);
	UBossAbility* Ability;
	if(bBasicAttackNext)
	{
		Ability = Cast<UBossAbility>(BasicAttack);
		bBasicAttackNext = false;
		if(IsValid(BoulderSpawner)) BoulderSpawner->Enable();
	}
	else
	{
		Ability = AttackList[AttackIndex];
		bBasicAttackNext = true;
		if (IsValid(BoulderSpawner)) BoulderSpawner->Disable();
	}

	Ability->UseAbility();
	CurrentAttack = Ability;
}

/**
 * @brief Turns the boss to the player.
 * @param DeltaTime the delta time.
 * @param Speed the speed of the turn.
 */
void ABoss::TurnToPlayer(const float DeltaTime, const float Speed)
{
	const auto PlayerLocation = Player->GetPawn()->GetActorLocation();
	const auto TargetLocation = FVector(PlayerLocation.X, PlayerLocation.Y, GetActorLocation().Z);

	const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	const auto NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, Speed);
	SetActorRotation(NewRotation);
}

/**
 * @brief Initializes and starts the fight.
 */
void ABoss::StartFight()
{
	bFightStarted = true;
	AttackTimer = AttackCooldown;
	AttackList.Empty();
	AttackList.Add(Cast<UBossAbility>(LavaBeam));
	AttackList.Add(Cast<UBossAbility>(LavaFloor));

	if(IsValid(BoulderSpawner))
	{
		BoulderSpawner->Enable();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "BoulderSpawner is not valid");
	}
}

/**
 * @brief Gets the rotation to the player.
 * @param Origin the origin of the rotation.
 * @return the rotation.
 */
FRotator ABoss::GetRotationToPlayer(FVector Origin) const
{
	if (!IsValid(Player)) return FRotator::ZeroRotator;

	return UKismetMathLibrary::FindLookAtRotation(Origin, Player->GetPawn()->GetActorLocation() + FVector(0,0,100));
}

/**
 * @brief Cleans up the boss abilites.
 */
void ABoss::Cleanup()
{
	CurrentAttack->DeactivateAbility();
}
