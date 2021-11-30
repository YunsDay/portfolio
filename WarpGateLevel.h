// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarpGateLevel.generated.h"

UCLASS()
class GAME_API AWarpGateLevel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarpGateLevel();

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
		class UParticleSystem* m_pParticleTemplate;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool m_bWarpActivate;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FName m_tOpenLevelName;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class ATPSCharacter* m_pPlayer;

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
