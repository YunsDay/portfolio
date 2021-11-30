// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerCollision.h"
#include "Components/BoxComponent.h"

// Sets default values
ATriggerCollision::ATriggerCollision():
	m_BoxComponent{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	m_BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&ATriggerCollision::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ATriggerCollision::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATriggerCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerCollision::SetActive(bool bActive)
{
	m_BoxComponent->SetGenerateOverlapEvents(bActive);
}

void ATriggerCollision::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (TriggerEvent.IsBound())
		TriggerEvent.Broadcast();
}
