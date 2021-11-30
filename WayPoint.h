// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "WayPointLocation.h"
#include "WayPoint.generated.h"

UCLASS()
class GAME_API AWayPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWayPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class USceneComponent* m_RootComponent;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UStaticMeshComponent* m_StaticMesh;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FVector m_vLocation;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TArray<AWayPointLocation*> m_pNextWayPointLocation;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		int m_nMaxPointIndex;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		int m_nNextPointIndex;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool m_bFlipFlop;
	

public:
	UFUNCTION(BlueprintCallable)
		void SetMoveNextWayPoint();
	UFUNCTION()
		void SetVisibility(bool bVisible);
	UFUNCTION()
		void EndNextPosition();

private:
	UFUNCTION()
		void PlayTimeline();
	UFUNCTION()
		void ReverseTimeline();

public:
	UPROPERTY()
		FTimeline m_TimelineLocation;
	UFUNCTION()
		void UpdateLocation();
	UFUNCTION()
		void TimelineFinished();
	UPROPERTY(EditAnyWhere)
		class UCurveFloat* m_cCurve;

};
