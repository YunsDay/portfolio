// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyStateType.generated.h"

UENUM(BlueprintType)
enum class EEnemyStateType : uint8
{
	None = 0,
	Idle,
	Alert,
	Death,
	Max,
};

class GAME_API EnemyStateType
{
public:
	EnemyStateType();
	~EnemyStateType();
};
