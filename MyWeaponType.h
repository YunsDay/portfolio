// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyWeaponType.generated.h"


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	NONE = 0,
	ShotSword,
	Spear,
	Axe,
	MaxENUM,
};

class GAME_API MyWeaponType
{
public:
	MyWeaponType();
	~MyWeaponType();
};
