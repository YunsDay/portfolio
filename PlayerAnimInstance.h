// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInterrupted);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCompleted);

UCLASS()
class GAME_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
		class ATPSCharacter* m_pOwner;
public:
	
	void Init(class ATPSCharacter* pOwner);

	//PlayMontage
	UFUNCTION(BlueprintCallable)
		void PlayMontage(UAnimMontage* pAnimMontage, float fPlayRate);
	UFUNCTION()
		void MontageComplete(class UAnimMontage* pAnimMontage, bool bInterrupted);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, VisibleDefaultsOnly)
		FOnInterrupted OnInterruptedBinding;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, VisibleDefaultsOnly)
		FOnCompleted OnCompletedBinding;

	//PlayAttackMontage
	UFUNCTION(BlueprintCallable)
		void PlayAttackMontage(UAnimMontage* pAnimMontage, float fPlayRate);
	UFUNCTION()
		void AttackMontageComplete(class UAnimMontage* pAnimMontage, bool bInterrupted);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, VisibleDefaultsOnly)
		FOnInterrupted AttackMontageOnInterruptedBinding;

};
