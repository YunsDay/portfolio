// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileSpiderTankCannon.generated.h"

UCLASS()
class GAME_API AProjectileSpiderTankCannon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileSpiderTankCannon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UProjectileMovementComponent* m_ProjectileMovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* m_SphereComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* m_StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fDamage;

	UFUNCTION()
		void Init(float fDamage);

private:
	UFUNCTION()
		void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
