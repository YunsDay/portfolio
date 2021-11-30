// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InformationOpen.generated.h"

UCLASS()
class GAME_API AInformationOpen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInformationOpen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UBoxComponent* m_BoxComponent;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FText m_tText;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool m_bMoveLimit;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class ATPSCharacter* m_pPlayer;
	UFUNCTION(BlueprintCallable)
		void InteractionEvent();

private:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
