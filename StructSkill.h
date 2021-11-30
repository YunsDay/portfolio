// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Styling/SlateTypes.h"
#include "StructSkill.generated.h"


USTRUCT(Atomic, BlueprintType)
struct FS_Skill : public FTableRowBase
{
public:
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FText tSkillName;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float fCoolTime;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float fUseStaminaAmount;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UAnimMontage* mtMotion;	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float fDamage;	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FProgressBarStyle sSkillImageStyle;
};

class GAME_API StructSkill
{
public:
	StructSkill();
	~StructSkill();
};
