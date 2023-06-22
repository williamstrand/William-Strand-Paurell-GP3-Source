// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "WeatherChangeVolume.generated.h"

enum EWeather;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTriggerEnter, EWeather, NewWeather);


UCLASS()
class GP03T06_API AWeatherChangeVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeatherChangeVolume();
	
	UPROPERTY(BlueprintAssignable, Category = Volume)
	FOnTriggerEnter OnVolumeEnterDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, Category = Weather)
	TEnumAsByte<EWeather> NewWeather;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = Volume)
	bool bDestroyOnEnter = false;
};
