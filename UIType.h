// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIType.generated.h"

UENUM(BlueprintType)
enum class EInterfaceType : uint8
{
	NONE = 0,
	Inventory,
	PlayerStateInterface,
	Max,
};

UENUM(BlueprintType)
enum class EUIType : uint8
{
	NONE = 0,
	InformationUI,
	InteractionUI,
	CooltimeLineUI,
	HpBarUI,
	QuickSlotUI,
	SkillSlotUI,
	Max,
};


class GAME_API UIType
{
public:
	UIType();
	~UIType();
};
