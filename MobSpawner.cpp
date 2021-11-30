// Fill out your copyright notice in the Description page of Project Settings.


#include "MobSpawner.h"

// Sets default values
AMobSpawner::AMobSpawner():
	m_classToDoSpawnMob{},
	m_pSpawnedMob{},
	m_vLocation{},
	m_fHp{},
	m_fLimitDistance{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMobSpawner::BeginPlay()
{
	Super::BeginPlay();
	m_vLocation = GetActorLocation();
	m_vLocation.Z = GetActorLocation().Z + 50.0f;
	SpawnEnemy();
}

// Called every frame
void AMobSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMobSpawner::SpawnEnemy()
{
	m_pSpawnedMob = GetWorld()->SpawnActor<AEnemyBaseMob>(m_classToDoSpawnMob, m_vLocation, FRotator(0.0f, 0.0f, 0.0f));
	if (m_pSpawnedMob)
	{
		m_pSpawnedMob->InitSpawn(m_fHp, m_fLimitDistance, this, m_vLocation);
		m_pSpawnedMob->DeathEventBinding.AddDynamic(this, &AMobSpawner::Death);
	}
}

void AMobSpawner::Death()
{
	m_pSpawnedMob->DeathEventBinding.Clear();
	m_pSpawnedMob = nullptr;
	
	//Delay
	FTimerHandle sWaitHandle{};
	float fWaitTime{};
	fWaitTime = 8.0f;
	GetWorld()->GetTimerManager().SetTimer(sWaitHandle, FTimerDelegate::CreateLambda([&]()
	{
		SpawnEnemy();
	}), fWaitTime, false);

}

