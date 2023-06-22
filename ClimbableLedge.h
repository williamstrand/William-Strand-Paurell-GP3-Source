// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ClimbableLedge.generated.h"

UENUM()
enum ELedgeDirection
{
	Horizontal,
	Vertical
};

UCLASS()
class GP03T06_API AClimbableLedge : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AClimbableLedge();

	ELedgeDirection GetDirection() const { return Direction; }

protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* Box;
	
	UPROPERTY(EditInstanceOnly, Category = Ledge)
	TEnumAsByte<ELedgeDirection> Direction;
};
