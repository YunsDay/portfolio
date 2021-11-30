// Fill out your copyright notice in the Description page of Project Settings.


#include "WayPointLocation.h"

// Sets default values
AWayPointLocation::AWayPointLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWayPointLocation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWayPointLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
