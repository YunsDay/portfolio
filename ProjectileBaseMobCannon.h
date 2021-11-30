// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyRangeDamage.h"
#include "ProjectileBaseMobCannon.generated.h"

UCLASS()
class GAME_API AProjectileBaseMobCannon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBaseMobCannon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UProjectileMovementComponent* m_ProjectileMovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* m_SphereComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* m_StaticMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AEnemyRangeDamage> m_classRangeDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fExplosionDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fExplosionRangeMagnification;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bIsCharacterKnockDown;

	UFUNCTION()
		void Init(float fDamage, float fExplosionDamage, float fExplosionRangeMagnification, bool bPlayerKnockDown);

private:
	UFUNCTION()
		void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
