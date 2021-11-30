// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarpGate.generated.h"

UCLASS()
class GAME_API AWarpGate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarpGate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UParticleSystemComponent* m_ParticleSystem;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UCapsuleComponent* m_CapsuleComponent;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool m_bWarpActivate;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class ATPSCharacter* m_pPlayer;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class AWarpLocation* m_pWarpLocation;
public:
	UFUNCTION(BlueprintCallable)
		void SetWarpActivate(bool bIsActive);
private:
	UFUNCTION()
		void Warp();
private:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
