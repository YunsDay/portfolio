// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyWeaponType.h"
#include "MyWeapon.generated.h"

UCLASS()
class GAME_API AMyWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USceneComponent* m_DefaultSceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent * m_pStaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCapsuleComponent* m_AttackCapsuleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCapsuleComponent* m_SkillCapsuleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USceneComponent* m_CircleTracePoint;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATPSCharacter* m_pOwnerRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fSkillDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fCriticalDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fCriticalChancePercent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fMotionMagnification;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FName> DT_SkillRow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeaponType m_eWeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName m_tWeaponAttackMotionRow;

public:
	
	UFUNCTION(BlueprintCallable)
		void SetGenerateOverlapEvent(bool bOnOff);
	UFUNCTION(BlueprintCallable)
		void GetWeaponState(float& pDamage, float& pCriticalDamage, float& pCriticalChancePercent);
	UFUNCTION(BlueprintCallable)
		void SetSkillDamage(float fSkillDamage);
	UFUNCTION(BlueprintCallable)
		void SetMotionMagnification(float fMotionMagnification);
	UFUNCTION(BlueprintCallable)
		void SetGenerateSkill(bool bOnOff);

	UFUNCTION(BlueprintCallable)
		FName GetSkillRow(int nIndex);

private:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void OnOverlapSkillBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	UFUNCTION(BlueprintCallable)
		float GetFinalDamageCaculate();
	UFUNCTION(BlueprintCallable)
		float GetSkillDamageCaculate();

private:
	UFUNCTION()
		void CircleTraceOnHit();
	UFUNCTION()
		void GiveDamageToEnemy(class AActor* OtherActor, float fDamage);
	UPROPERTY()
		FHitResult m_sCircleTraceHitResult;

};
