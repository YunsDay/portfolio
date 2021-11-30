// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StructGiveItem.h"
#include "GiveItemBox.generated.h"

UCLASS()
class GAME_API AGiveItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGiveItemBox();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UStaticMeshComponent* m_StaticMesh;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class USphereComponent* m_SphereComponent;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		UParticleSystemComponent* m_Particle;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool m_bGet;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fCooltime;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FText m_tCooltimeText;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TArray<FS_GiveItem> m_arGiveItem;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		int m_nArraySize;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class ATPSCharacter* m_pPlayer;
	
		 
public:
	UFUNCTION(BlueprintCallable)
		void FinishGetItem();
	UFUNCTION(BlueprintCallable)
		void GiveItem();
	UFUNCTION(BlueprintCallable)
		void EndCooltime();

private:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
