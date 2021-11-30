// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentType.h"
#include "Engine/DataTable.h"
#include "MyWeapon.h"
#include "StructEquipmentItem.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FS_EquipmentItem : public FTableRowBase
{
public:
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FName tName;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EEquipmentType eEquipmentType;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TSubclassOf<AMyWeapon> cWeapon;
	

};

class GAME_API StructEquipmentItem
{
public:
	StructEquipmentItem();
	~StructEquipmentItem();
};
