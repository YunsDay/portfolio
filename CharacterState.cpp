// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterState.h"
#include "Math/Vector.h"
#include "TPSCharacter.h"

// Sets default values for this component's properties
UCharacterState::UCharacterState() :
	m_pOwnerRef{},
	m_bIsStaminaUse{},
	m_eStateType{},
	m_arPlayerState{},
	m_arPlayerStateBase{}
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_arPlayerState.Init(100.0f, (uint8)EStateType::MaxENUM);
	m_arPlayerStateBase.Init(100.0f, (uint8)EStateType::MaxENUM);

	m_arPlayerStateBase[(uint8)EStateType::CriticalChance] = 10.0f;
	m_arPlayerStateBase[(uint8)EStateType::StaminaRecoveryAmount]= 5.0f;
	m_arPlayerStateBase[(uint8)EStateType::StaminaDiminishAmount] = -5.0f;

	m_arPlayerState = m_arPlayerStateBase;

}


// Called when the game starts
void UCharacterState::BeginPlay()
{
	Super::BeginPlay();

	
	
}


// Called every frame
void UCharacterState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	

	if (m_bIsStaminaUse && m_pOwnerRef->GetVelocity().SizeSquared() > 10000.0f)
		SetStamina(GetState(EStateType::StaminaDiminishAmount) * DeltaTime);
	else
		SetStamina(GetState(EStateType::StaminaRecoveryAmount) * DeltaTime);
	// ...
}

bool UCharacterState::UseStaminaIfCan(float fUseAmount)
{
	float fValue{};
	fValue = GetState(EStateType::Stamina) - fUseAmount;
	if (fValue >= 0)
	{
		SetState(EStateType::Stamina, fValue);
		return true;
	}
	return false;
}

float UCharacterState::GetHpPercent()
{
	return m_arPlayerState[(uint8)EStateType::Hp]/ m_arPlayerState[(uint8)EStateType::MaxHp];
}

float UCharacterState::GetStaminaPercent()
{
	return m_arPlayerState[(uint8)EStateType::Stamina] / m_arPlayerState[(uint8)EStateType::MaxStamina];;
}

void UCharacterState::TakeDamage(float fDamage)
{
	float fFinalDamage{};

	fFinalDamage = fDamage - GetState(EStateType::Defensive);
	if (fFinalDamage <= 0)
		SetState(EStateType::Hp, FMath::Clamp(GetState(EStateType::Hp) - 1.0f, 0.0f, GetState(EStateType::MaxHp)));
	else
		SetState(EStateType::Hp, FMath::Clamp(GetState(EStateType::Hp)-fFinalDamage,0.0f,GetState(EStateType::MaxHp)));
}

void UCharacterState::Init(ATPSCharacter* cOwenr)
{
	m_pOwnerRef = cOwenr;
}

void UCharacterState::SetIsStaminaUse(bool bIsStaminaUse)
{
	m_bIsStaminaUse = bIsStaminaUse;
}

float UCharacterState::GetState(EStateType eStateType)
{
	return m_arPlayerState[(uint8)eStateType];
}

void UCharacterState::SetState(EStateType eStateType, float fValue)
{
	m_arPlayerState[(uint8)eStateType] = fValue;
}

void UCharacterState::Recovery(EStateType eStateType, float fRecoveryAmount)
{
	SetState(eStateType, FMath::Clamp(GetState(eStateType)+fRecoveryAmount, 0.0f, m_arPlayerStateBase[(uint8)eStateType]));
}

void UCharacterState::SetStateEquipWeapon(bool EquipUnEquip)
{
	float fDamage{};
	float fCriticalDamage{};
	float fCriticalChancePercent{};
	m_pOwnerRef->m_pWeapon->GetWeaponState(fDamage,fCriticalDamage,fCriticalChancePercent);
	if (EquipUnEquip)
	{
		SetState(EStateType::Damage, GetState(EStateType::Damage)+fDamage);
		SetState(EStateType::CriticalDamage, GetState(EStateType::CriticalDamage) + fCriticalDamage);
		SetState(EStateType::CriticalChance, GetState(EStateType::CriticalChance) + fCriticalChancePercent);
	}
	else
	{
		SetState(EStateType::Damage, GetState(EStateType::Damage) - fDamage);
		SetState(EStateType::CriticalDamage, GetState(EStateType::CriticalDamage) - fCriticalDamage);
		SetState(EStateType::CriticalChance, GetState(EStateType::CriticalChance) - fCriticalChancePercent);
	}
}

void UCharacterState::SetStamina(float fRecoveryAmount)
{
	SetState(EStateType::Stamina, FMath::Clamp(fRecoveryAmount + GetState(EStateType::Stamina), -0.1f, GetState(EStateType::MaxStamina)));
}

