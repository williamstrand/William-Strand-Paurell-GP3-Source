// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto LedgeCheck = CheckForLedge(GetActorLocation() + FVector(0, 0, LedgeCheckHeight));

	if (LedgeCheck.Ledge != nullptr)
	{
		GrabLedge(LedgeCheck);
	}
}

/**
 * @brief Adds movement.
 * @param MovementVector The inputted direction. 
 */
void APlayerCharacter::Move(const FVector2D MovementVector)
{
	if (Controller == nullptr) return;
	
	if (bOnLedge)
	{
		ClimbLedge(MovementVector);
		return;
	}

	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	// add movement 
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

/**
 * @brief Adds movement when on a ledge.
 * @param MovementVector The inputted direction.
 */
void APlayerCharacter::ClimbLedge(const FVector2D MovementVector)
{
	if(MovementVector.IsZero()) return;
	if (!bCanReleaseLedge) return;
	const auto NormalizedMovement = MovementVector.GetSafeNormal();
	FVector TargetPos;
	FLedgeInfo LedgeInfo;

	switch (CurrentLedgeInfo.Direction)
	{
		case Horizontal:
			if (NormalizedMovement.Y < 0)
			{
				ReleaseLedge();
			}

			if (NormalizedMovement.X == 0) break;

			TargetPos = GetActorLocation() + NormalizedMovement.X * GetActorRightVector() * LedgeClimbSpeed;

			LedgeInfo = CheckForLedge(TargetPos + FVector(0, 0, LedgeCheckHeight));
			if (LedgeInfo.Ledge)
			{
				ForceGrabLedge(LedgeInfo);
			}
			break;

		case Vertical:
			if (NormalizedMovement.Y == 0) break;

			TargetPos = GetActorLocation() + NormalizedMovement.Y * GetActorUpVector() * LedgeClimbSpeed;
			if (CheckForLedge(TargetPos + FVector(0, 0, LedgeCheckHeight)).Ledge)
			{
				SetActorLocation(TargetPos);
			}
			break;
	}
}

/**
 * @brief Makes character jump.
 */
void APlayerCharacter::Jump()
{
	if (bOnLedge)
	{
		if (ReleaseLedge())
		{
			Super::Jump();
		}
		return;
	}

	Super::Jump();
}

/**
 * @brief Checks if player can grab a ledge.
 * @return true if player can grab ledge.
 */
FLedgeInfo APlayerCharacter::CheckForLedge(FVector Start) const
{
	FLedgeInfo LedgeInfo;
	FHitResult LedgeTraceHit;
	const FVector End = Start + GetActorForwardVector() * LedgeGrabRange;

	if (bDrawDebug) DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, .1f, 0, 5);
	GetWorld()->LineTraceSingleByChannel(LedgeTraceHit, Start, End, ECC_GameTraceChannel1);

	if (LedgeTraceHit.GetActor() == nullptr) return LedgeInfo;
	if (!LedgeTraceHit.GetActor()->ActorHasTag("Ledge")) return LedgeInfo;

	LedgeInfo.Ledge = LedgeTraceHit.GetActor();
	LedgeInfo.Normal = LedgeTraceHit.ImpactNormal;
	LedgeInfo.GrabPosition = LedgeTraceHit.ImpactPoint;
	LedgeInfo.Direction = Cast<AClimbableLedge>(LedgeInfo.Ledge)->GetDirection();
	return LedgeInfo;
}

/**
 * @brief Player grabs a ledge.
 * @param LedgeInfo struct containing information about the ledge.
 */
void APlayerCharacter::GrabLedge(const FLedgeInfo LedgeInfo)
{
	if (GetVelocity().Z > 0)  return;
	if (!bCanGrabLedge)       return;
	if (bOnLedge)             return;

	CurrentLedgeInfo = LedgeInfo;

	bOnLedge = true;
	bCanReleaseLedge = false;

	GetWorld()->GetTimerManager().SetTimer(ReleaseLedgeDelayTimerHandle, this, &APlayerCharacter::SetCanReleaseLedge, LedgeReleaseDelay);

	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->StopMovementImmediately();

	const auto NewRotation = (-LedgeInfo.Normal).Rotation();
	auto NewPos = FVector();

	if(LedgeInfo.Direction == Horizontal)
	{
		NewPos = FVector(LedgeInfo.GrabPosition.X, LedgeInfo.GrabPosition.Y, LedgeInfo.Ledge->GetActorLocation().Z - LedgeCheckHeight);
	}
	else
	{
		NewPos = FVector(LedgeInfo.GrabPosition.X, LedgeInfo.GrabPosition.Y, LedgeInfo.GrabPosition.Z - LedgeCheckHeight);
	}

	NewPos += LedgeInfo.Normal * LedgeGrabOffset;
	SetActorRotation(NewRotation);
	SetActorLocation(NewPos);
}

/**
 * @brief Player grabs a ledge without any checks.
 * @param LedgeInfo struct containing information about the ledge.
 */
void APlayerCharacter::ForceGrabLedge(const FLedgeInfo LedgeInfo)
{
	CurrentLedgeInfo = LedgeInfo;

	bOnLedge = true;

	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->StopMovementImmediately();

	const auto NewRotation = (-LedgeInfo.Normal).Rotation();
	auto NewPos = FVector();

	if (LedgeInfo.Direction == Horizontal)
	{
		NewPos = FVector(LedgeInfo.GrabPosition.X, LedgeInfo.GrabPosition.Y, LedgeInfo.Ledge->GetActorLocation().Z - LedgeCheckHeight);
	}
	else
	{
		NewPos = FVector(LedgeInfo.GrabPosition.X, LedgeInfo.GrabPosition.Y, LedgeInfo.GrabPosition.Z - LedgeCheckHeight);
	}

	NewPos += LedgeInfo.Normal * LedgeGrabOffset;
	SetActorRotation(NewRotation);
	SetActorLocation(NewPos);
}

/**
 * @brief Player lets go of ledge.
 * @return true if player can release ledge.
 */
bool APlayerCharacter::ReleaseLedge()
{
	if (!bOnLedge) return false;
	if (!bCanReleaseLedge) return false;

	CurrentLedgeInfo = FLedgeInfo();

	bOnLedge = false;
	bCanGrabLedge = false;
	GetWorld()->GetTimerManager().SetTimer(GrabLedgeDelayTimerHandle, this, &APlayerCharacter::SetCanGrabLedge, LedgeGrabDelay);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	return true;
}