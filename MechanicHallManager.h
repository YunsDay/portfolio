// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "MechanicHallManager.generated.h"

UCLASS()
class GAME_API AMechanicHallManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMechanicHallManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class ABossSpiderTank* m_pSpiderTank;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class ATPSCharacter* m_pPlayer;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class AGasEngineTrap* m_pGasEngine;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class AInteractionBox* m_pShotDownInteractionBox;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class AInteractionBox* m_pBetteryInteractionBox;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class AWarpGateLevel* m_pWarpGate;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class  AWayPoint* m_pWayPoint;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class ATriggerCollision* m_pTriggerBox;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class ADoor* m_pDoor;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TArray<FText> m_arMissionText;

public:
	UFUNCTION()
		void EndQuest();
	UFUNCTION()
		void ShotDownEngine();
	UFUNCTION()
		void Interaction();
	UFUNCTION()
		void BossDeath();
	UFUNCTION()
		void DoorActive();
	UFUNCTION()
		void TriggedBox();

public:
	UPROPERTY()
		FTimeline m_TimelineRotator;
	UFUNCTION()
		void RotatorUpdate();
	UFUNCTION()
		void TimelineFinished();
	UPROPERTY(EditAnyWhere)
		class UCurveFloat* m_cCurve;

private:
	UFUNCTION()
		void InformationOpenInteractionUnbinding(UPARAM(ref) const FText & tInformation, bool bMoveLimit);


};
