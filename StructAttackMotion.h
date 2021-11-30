// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StructAttackMotion.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FS_AttackMotion : public FTableRowBase
{
public:
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		UAnimMontage* mtFirstLowMotion;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float fFirstLowMotionMagnification;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float fFirstLowPlayRate;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		UAnimMontage* mtSecondLowMotion;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float fSecondLowMotionMagnification;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float fSecondLowPlayRate;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		UAnimMontage* mtThirdLowMotion;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float fThirdLowMotionMagnification;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float fThirdLowPlayRate;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		UAnimMontage* mtFirstHighMotion;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float fFirstHighMotionMagnification;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float fFirstHighPlayRate;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		UAnimMontage* mtSecondHighMotion;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float fSecondHighMotionMagnification;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float fSecondHighPlayRate;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		UAnimMontage* mtThirdHighMotion;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float fThirdHighMotionMagnification;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float fThirdHighPlayRate;

};

class GAME_API StructAttackMotion
{
public:
	StructAttackMotion();
	~StructAttackMotion();
};
