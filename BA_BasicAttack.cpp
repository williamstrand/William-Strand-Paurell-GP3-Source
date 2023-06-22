
#include "BA_BasicAttack.h"
#include "Boss.h"

void UBA_BasicAttack::BeginPlay()
{
	Super::BeginPlay();
}

void UBA_BasicAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!AbilityActive) return;

	Cast<ABoss>(Owner)->TurnToPlayer(DeltaTime, RotationSpeed);

	if(AttackCooldown > 0)
	{
		AttackCooldown -= DeltaTime;
		return;
	}

	OnShootTriggered.Broadcast();
	AttackCooldown = TimeBetweenShots;
}

/**
 * @brief Makes the boss shoot.
 */
void UBA_BasicAttack::Shoot()
{
	const FActorSpawnParameters SpawnInfo;
	
	const auto SpawnLocation = Owner->GetProjectilePoint()->GetComponentLocation();

	auto NewProjectile = GetWorld()->SpawnActor<AEnemyAttack_Projectile>(ProjectileClass, SpawnLocation, Owner->GetRotationToPlayer(SpawnLocation), SpawnInfo);
}

void UBA_BasicAttack::UseAbility()
{
	ActivateAbility();
}

void UBA_BasicAttack::ActivateAbility()
{
	Super::ActivateAbility();
}
void UBA_BasicAttack::DeactivateAbility()
{
	Super::DeactivateAbility();
}
