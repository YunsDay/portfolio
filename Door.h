// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Door.generated.h"

UCLASS()
class GAME_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UStaticMeshComponent* m_StaticDoorFrame;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UStaticMeshComponent* m_StaticDoorA;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UStaticMeshComponent* m_StaticDoorB;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UBoxComponent* m_BoxComponent;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool m_bDoorActivate;
public:
	UFUNCTION()
		void OpenDoor();
	UFUNCTION()
		void CloseDoor();
	UFUNCTION()
		void ActivateDoor();

private:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UPROPERTY()
		FTimeline m_TimelineLocation;
	UFUNCTION()
		void TimelineUpdate();
	UPROPERTY(EditAnyWhere)
		class UCurveFloat* m_cCurve;


};
