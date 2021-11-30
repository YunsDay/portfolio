// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentType.generated.h"


UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	NONE = 0,
	Weapon,
	Equipment,
	Max,
};


class GAME_API EquipmentType
{

public:
	EquipmentType();
	~EquipmentType();

};
