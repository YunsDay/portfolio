// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "TPSCharacter.h"

void UPlayerAnimInstance::Init(class ATPSCharacter* pOwner)
{
	m_pOwner = Cast<ATPSCharacter>(GetOwningActor());
	AttackMontageOnInterruptedBinding.AddDynamic(m_pOwner ,&ATPSCharacter::OffWeaponCollision);
}

void UPlayerAnimInstance::PlayMontage(UAnimMontage* pAnimMontage, float fPlayRate)
{
	Montage_Play(pAnimMontage, fPlayRate);

	FOnMontageBlendingOutStarted CompleteDelegate;
	CompleteDelegate.BindUObject(this, &UPlayerAnimInstance::MontageComplete);
	Montage_SetEndDelegate(CompleteDelegate, pAnimMontage);
}

void UPlayerAnimInstance::MontageComplete(UAnimMontage* pAnimMontage, bool bInterrupted)
{
	if (bInterrupted && OnInterruptedBinding.IsBound())
		OnInterruptedBinding.Broadcast();
	else if(OnCompletedBinding.IsBound())
		OnCompletedBinding.Broadcast();

	OnCompletedBinding.Clear();
	OnInterruptedBinding.Clear();
}

void UPlayerAnimInstance::PlayAttackMontage(UAnimMontage* pAnimMontage, float fPlayRate)
{
	Montage_Play(pAnimMontage, fPlayRate);

	FOnMontageBlendingOutStarted CompleteDelegate;
	CompleteDelegate.BindUObject(this, &UPlayerAnimInstance::AttackMontageComplete);
	Montage_SetEndDelegate(CompleteDelegate, pAnimMontage);
}

void UPlayerAnimInstance::AttackMontageComplete(UAnimMontage* pAnimMontage, bool bInterrupted)
{
	if (bInterrupted && AttackMontageOnInterruptedBinding.IsBound())
		AttackMontageOnInterruptedBinding.Broadcast();
}
