// Fill out your copyright notice in the Description page of Project Settings.


#include "WarpLocation.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AWarpLocation::AWarpLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWarpLocation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWarpLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FRotator AWarpLocation::GetRotation()
{

	return UKismetMathLibrary::MakeRotFromX(GetActorForwardVector());
}

FVector AWarpLocation::GetLocation()
{
	return GetActorLocation();
}

