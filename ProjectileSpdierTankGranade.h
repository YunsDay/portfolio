// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyRangeDamage.h"
#include "ProjectileSpdierTankGranade.generated.h"

UCLASS()
class GAME_API AProjectileSpdierTankGranade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileSpdierTankGranade();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UProjectileMovementComponent* m_ProjectileMovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* m_SphereComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* m_StaticMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystemComponent* m_ParticleSystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AEnemyRangeDamage> m_classRangeDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystem* m_pExplosionParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundWave* m_pExplosionSoundWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fExplosionScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fExplosionDamage;

	UFUNCTION()
		void Init(float fDamage,float fExplosionScale, float fExplosionDamage);

private:
	UFUNCTION()
		void OnComponentHit(UPrimitiveComponent * HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
