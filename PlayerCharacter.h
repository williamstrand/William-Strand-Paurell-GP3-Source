// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClimbableLedge.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

/**
 * @brief Contains information about a ledge.
 */
struct FLedgeInfo
{
	AActor* Ledge;
	FVector GrabPosition;
	FVector Normal;
	ELedgeDirection Direction;

	FLedgeInfo()
	{
		Ledge = nullptr;
		GrabPosition = FVector::ZeroVector;
		Normal = FVector::ZeroVector;
		Direction = Horizontal;
	}
};

UCLASS()
class GP03T06_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	void GrabLedge(const FLedgeInfo LedgeInfo);
	void ForceGrabLedge(const FLedgeInfo LedgeInfo);
	FLedgeInfo CheckForLedge(FVector Start) const;

	UFUNCTION(BlueprintCallable, Category = "Climbing")
	bool ReleaseLedge();
	void ClimbLedge(const FVector2D MovementVector);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void Move(const FVector2D MovementVector);

	virtual void Jump() override;

	FTimerHandle GrabLedgeDelayTimerHandle;
	FTimerHandle ReleaseLedgeDelayTimerHandle;

	bool bCanGrabLedge = true;
	bool bCanReleaseLedge = true;

	FLedgeInfo CurrentLedgeInfo;

	void SetCanGrabLedge() { bCanGrabLedge = true; }
	void SetCanReleaseLedge() { bCanReleaseLedge = true; }

	UPROPERTY(EditDefaultsOnly, Category = "Climbing|Ledge Check", meta = (ToolTip = "The range at which the player will grab on to ledges."))
	float LedgeGrabRange = 25.f;
	UPROPERTY(EditDefaultsOnly, Category = "Climbing|Ledge Check", meta = (ToolTip = "The height at which the player can grab on to a ledge."))
	float LedgeCheckHeight = 50;
	UPROPERTY(EditDefaultsOnly, Category = "Climbing|Ledge Grab", meta = (ToolTip = "The speed at which the player moves left and right when grabbed on to a ledge."))
	float LedgeClimbSpeed = 3;
	UPROPERTY(EditDefaultsOnly, Category = "Climbing|Ledge Grab", meta = (ToolTip = "The offset at which the player will be attached to the ledge."))
	float LedgeGrabOffset = 25;
	UPROPERTY(EditDefaultsOnly, Category = "Climbing|Delay", meta = (ToolTip = "The time it takes before player can release a ledge after it is grabbed."))
	float LedgeReleaseDelay = .15f;
	UPROPERTY(EditDefaultsOnly, Category = "Climbing|Delay", meta = (ToolTip = "The time it takes before player can grab again after releasing a ledge."))
	float LedgeGrabDelay = .1f;

	UPROPERTY(EditDefaultsOnly, Category = "Climbing|Debug")
	bool bDrawDebug = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly, Category = Climbing)
	bool bOnLedge = false;
};
