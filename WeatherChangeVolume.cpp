// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherChangeVolume.h"

// Sets default values
AWeatherChangeVolume::AWeatherChangeVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
}

// Called when the game starts or when spawned
void AWeatherChangeVolume::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AWeatherChangeVolume::OnOverlapBegin);
	
}

/**
 * @brief On Overlap Begin
 * @param OverlappedComp 
 * @param OtherActor 
 * @param OtherComp 
 * @param OtherBodyIndex 
 * @param bFromSweep 
 * @param SweepResult 
 */
void AWeatherChangeVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnVolumeEnterDelegate.Broadcast(NewWeather);
	if (bDestroyOnEnter)
	{
		Destroy();
	}
}