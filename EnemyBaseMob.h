// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyStateType.h"
#include "EnemyInterface.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "StructGiveItem.h"
#include "EnemyBaseMob.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathEventBinding);

UCLASS()
class GAME_API AEnemyBaseMob : public ACharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBaseMob();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void TakeDamageCaculate(float fDamage, FName tHitBoneName, FVector vHitLocation, FRotator rHitRotator);
	virtual void TakeDamageCaculate_Implementation(float fDamage, FName tHitBoneName, FVector vHitLocation, FRotator rHitRotator) override;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class USphereComponent* m_SphereComponent;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EEnemyStateType m_eState;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool m_bIsDeath;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fMaxHp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fCurrentHp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fLimitDistance;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FVector m_vSpawnPoint;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class ATPSCharacter* m_pPlayer;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FRotator m_rTargetRotate;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		int m_nDeathPoseRandomIndex;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		int m_nMaxDeathPoseIndex;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		UAnimMontage* m_animAttack;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TArray<FS_GiveItem> m_arRandomGiveItem;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		int m_nGiveItemArraySize;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class AMobSpawner* m_pSpawner;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UParticleSystem* m_pBeAttackedParticle;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fBeAttackedParticleScale;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, VisibleDefaultsOnly)
		FDeathEventBinding DeathEventBinding;

public:
	UFUNCTION(BlueprintCallable)
		void InitSpawn(float fHp, float fLimitDistance, class AMobSpawner * pSpawner , FVector vSpawnPoint);
	UFUNCTION(BlueprintCallable)
		void StartAlert();
	UFUNCTION(BlueprintCallable)
		void ReturnToSpawnPoint();
	UFUNCTION(BlueprintCallable)
		void ReturnSpawnPointCaculate();
	UFUNCTION(BlueprintCallable)
		void Rotate(float fPlayRate, FRotator rTargetRotate);
	UFUNCTION(BlueprintCallable)
		void GiveItem();
	UFUNCTION(BlueprintCallable)
		void Death();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Attack();
	virtual void Attack_Implementation();


private:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:
	//Timeline
	UPROPERTY()
		FTimeline m_TimelineRotation;
	UFUNCTION()
		void TimelineUpdate();
	UFUNCTION()
		void TimelineFinished();
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UCurveFloat* m_cCurve;
};
