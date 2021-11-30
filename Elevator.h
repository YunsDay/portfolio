// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Elevator.generated.h"

UCLASS()
class GAME_API AElevator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElevator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UStaticMeshComponent* m_StaticElevator;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UStaticMeshComponent* m_StaticDoorA;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UStaticMeshComponent* m_StaticDoorB;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UBoxComponent* m_BoxComponent;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UBoxComponent* m_MoveBoxComponent;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fLiftDistance;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool m_bUpDown;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FVector m_vCurrentLocation;

public:
	UFUNCTION()
		void OpenDoor();
	UFUNCTION()
		void CloseDoor();
	UFUNCTION()
		void MoveLift();

private:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	UFUNCTION()
		void OnMoveOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnMoveOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	UPROPERTY()
		FTimeline m_TimelineLocation;
	UFUNCTION()
		void TimelineLocationUpdate();
	UFUNCTION()
		void TimelineFinished();
	UPROPERTY(EditAnyWhere)
		class UCurveFloat* m_cCurve;

	UPROPERTY()
		FTimeline m_TimelineRotation;
	UPROPERTY(EditAnyWhere)
		class UCurveFloat* m_cRotationCurve;
	UFUNCTION()
		void TimelineRotationUpdate();

};
