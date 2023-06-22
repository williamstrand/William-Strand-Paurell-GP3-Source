// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbableLedge.h"


// Sets default values
AClimbableLedge::AClimbableLedge()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}