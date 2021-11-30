// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyStateType.h"
#include "EnemyInterface.h"
#include "ProjectileSpdierTankGranade.h"
#include "ProjectileSpiderTankCannon.h"
#include "EnemyRangeDamage.h"
#include "Components/TimelineComponent.h"
#include "BossSpiderTank.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBossEventBind);

UCLASS()
class GAME_API ABossSpiderTank : public ACharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossSpiderTank();

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
		class UBoxComponent* m_BoxComponent;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UBoxComponent* m_BlockPawnBox;
	
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UParticleSystem* m_pBeAttackedParticle;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fBeAttackedParticleScale;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool m_bAwake;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool m_bIsActive;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool m_bIsDeath;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fMaxHp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fCurrentHp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fRushAttackDamage;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fEarthQuakeDamage;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fGranadeDamage;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fGranadeExplosionDamage;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fCannonDamage;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float m_fWhirlWindDamage;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FRotator m_rTargetRotate;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class ATPSCharacter* m_pPlayer;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FVector m_vSpawnCannonLocation;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EEnemyStateType m_eStateType;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TArray<UAnimMontage*> m_mtAttackMotion;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		int m_nAttackMotionMaxIndex;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TArray<UAnimMontage*> m_mtLongAttackMotion;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		int m_nLongAttackMotionMaxIndex;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		UAnimMontage* m_mtDeathMotion;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, VisibleDefaultsOnly)
		FBossEventBind DeathEventBinding;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TSubclassOf<AProjectileSpdierTankGranade> m_classGranade;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TSubclassOf<AProjectileSpiderTankCannon> m_classCannonBullet;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TSubclassOf<AEnemyRangeDamage> m_classRangeDamage;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TSubclassOf<AProjectileSpdierTankGranade> m_classLazer;
	
private:
	UPROPERTY(EditAnyWhere)
		bool m_bFlipFlop;

public:
	UFUNCTION(BlueprintCallable)
		void StartMachanic();
	UFUNCTION(BlueprintCallable)
		float Attack();
	UFUNCTION(BlueprintCallable)
		float LongDistanceAttack();

	//AttackFunction
	UFUNCTION(BlueprintCallable)
		void FireCannon();
	UFUNCTION(BlueprintCallable)
		void FireGranade();
	UFUNCTION(BlueprintCallable)
		void RushAttack();
	UFUNCTION(BlueprintCallable)
		void RushEnd();
	UFUNCTION(BlueprintCallable)
		void EarthQuake();
	UFUNCTION(BlueprintCallable)
		void WhirlWind();
	UFUNCTION(BlueprintCallable)
		void WhirlWindJudge();
	UFUNCTION(BlueprintCallable)
		void LazerAttack();
public:
	UFUNCTION(BlueprintCallable)
		void Death();
	UFUNCTION(BlueprintCallable)
		void Rotate();
private:
	UFUNCTION(BlueprintCallable)
		float CaculateSpawnCannonRotationYaw();
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UFUNCTION(BlueprintCallable)
		void Awake();
	
	UFUNCTION(BlueprintCallable)
		void SetStateType(EEnemyStateType eStateType);

public:
	UFUNCTION(BlueprintCallable)
		void SetPlayer(UPARAM(ref)ATPSCharacter * pPlayerRef);

	//Timeline
	UPROPERTY()
		FTimeline m_TimelineRotation;
	UFUNCTION()
		void TimelineUpdate();
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UCurveFloat* m_cCurve;

	//WhirlWindTimeline
	UPROPERTY()
		FTimeline m_TimelineWhirlWind;
	UFUNCTION()
		void TimelineWhirlWindUpdate();
	UFUNCTION()
		void TimelineWhirlWindFinished();
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UCurveFloat* m_cWhirlWindCurve;
	UPROPERTY()
		FRotator m_rCurrentRotate;
	UPROPERTY()
		FRotator m_rWhirlWindRotate;

};
