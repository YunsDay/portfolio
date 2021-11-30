// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateType.h"
#include "RecoveryType.h"
#include "Engine/DataTable.h"
#include "StructUseItemInfoDataTable.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FS_UseItemDataTable: public FTableRowBase
{
public:

	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float fRecoveryVerpAmount;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float fCooltime;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		ERecoveryType eType;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		EStateType eRecoveryVerpState;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UParticleSystem* pVerpParticle;

};

class GAME_API StructUseItemInfoDataTable
{
public:
	StructUseItemInfoDataTable();
	~StructUseItemInfoDataTable();
};
