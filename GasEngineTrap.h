// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GasEngineTrap.generated.h"

UCLASS()
class GAME_API AGasEngineTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGasEngineTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UStaticMeshComponent* m_GasEngineStaticMesh;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UStaticMeshComponent* m_FirstPartStaticMesh;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UStaticMeshComponent* m_SecondPartStaticMesh;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UStaticMeshComponent* m_ThirdPartStaticMesh;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UStaticMeshComponent* m_FourthPartStaticMesh;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UParticleSystemComponent* m_ParticleSystem;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fFirstPartSpeed;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fSecondPartSpeed;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fThirdPartSpeed;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fFourthPartSpeed;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool m_bShotDown;

public:
	UFUNCTION(BlueprintCallable)
		void ShotDownEngine();
	UFUNCTION(BlueprintCallable)
		void SetRotatorSpeed(float fRotatorSpeedOne, float fRotatorSpeedTwo, float fRotatorSpeedThree, float fRotatorSpeedFour, float fParticleTime);
		
};
