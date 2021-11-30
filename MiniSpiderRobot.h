// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBaseMob.h"
#include "ProjectileBaseMobCannon.h"
#include "MiniSpiderRobot.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API AMiniSpiderRobot : public AEnemyBaseMob
{
	GENERATED_BODY()

public:
	AMiniSpiderRobot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void Attack_Implementation() override;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TSubclassOf<AProjectileBaseMobCannon> m_classProjectileCannon;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fCannonDamage;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fCannonRangeDamage;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fCannonRangeMagnification;

	UFUNCTION(BlueprintCallable)
		void FireCannon();


};
