// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBaseMob.h"
#include "MobSpawner.generated.h"

UCLASS()
class GAME_API AMobSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMobSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TSubclassOf<AEnemyBaseMob> m_classToDoSpawnMob;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		AEnemyBaseMob* m_pSpawnedMob;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FVector m_vLocation;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fHp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fLimitDistance;

public:
	UFUNCTION(BlueprintCallable)
		void SpawnEnemy();
	UFUNCTION(BlueprintCallable)
		void Death();


};
