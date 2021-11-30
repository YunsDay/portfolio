// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyRangeDamage.generated.h"

UCLASS()
class GAME_API AEnemyRangeDamage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyRangeDamage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* m_SphereComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bIsPlayerKnockDown;

	UFUNCTION(BlueprintCallable)
		void Init(float fDamage, float fMagnification, bool bPlayerKnockDown);

private:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
