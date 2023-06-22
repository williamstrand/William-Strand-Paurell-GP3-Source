
#pragma once

#include "CoreMinimal.h"
#include "BossAbility.h"
#include "EnemyAttack_Projectile.h"
#include "BA_BasicAttack.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShoot);

UCLASS()
class GP03T06_API UBA_BasicAttack : public UBossAbility
{
	GENERATED_BODY()
	
private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void ActivateAbility() override;
	virtual void DeactivateAbility() override;

	UFUNCTION(BlueprintCallable)
	void Shoot();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemyAttack_Projectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
	float TimeBetweenShots = 2;
	float AttackCooldown;

	UPROPERTY(EditAnywhere)
	float RotationSpeed = 5;

	UPROPERTY(BlueprintAssignable)
	FOnShoot OnShootTriggered;
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void UseAbility() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnShoot();

	UFUNCTION(BlueprintCallable)
	float GetRotationSpeed() const { return RotationSpeed; }
	UFUNCTION(BlueprintCallable)
	void SetRotationSpeed(float Speed) { RotationSpeed = Speed; }
};
