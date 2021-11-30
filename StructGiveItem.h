// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructGiveItem.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FS_GiveItem
{
public:
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FName tItemDataTableRow;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		int nAmount;
};

class GAME_API StructGiveItem
{
public:
	StructGiveItem();
	~StructGiveItem();
};
