// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateType.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	NONE = 0,
	Hp,
	MaxHp,
	Damage,
	CriticalDamage,
	CriticalChance,
	Defensive,
	Stamina,
	MaxStamina,
	StaminaRecoveryAmount,
	StaminaDiminishAmount,
	MaxENUM,
};

class GAME_API StateType
{
public:
	StateType();
	~StateType();
};
