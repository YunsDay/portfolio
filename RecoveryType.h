// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RecoveryType.generated.h"

UENUM(BlueprintType)
enum class ERecoveryType : uint8
{
	NONE = 0,
	Recovery,
	Verp,
	PermanentUp,
	MaxENUM,
};

class GAME_API RecoveryType
{
public:
	RecoveryType();
	~RecoveryType();
};
