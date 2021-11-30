// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionType.h"
#include "InteractionBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTakeEndEventBinding);

UCLASS()
class GAME_API AInteractionBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractionBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UStaticMeshComponent* m_StaticMesh;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UBoxComponent* m_BoxComponent;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool m_bTake;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fCooltime;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FText m_tCooltimeText;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EInteractionType m_eInteractionType;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class ATPSCharacter* m_pPlayer;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, VisibleDefaultsOnly)
		FTakeEndEventBinding TakeEndEvent;
public:
	UFUNCTION(BlueprintCallable)
		void TakeEvent();
	UFUNCTION(BlueprintCallable)
		void Gathering();

private:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
