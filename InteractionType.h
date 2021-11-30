// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionType.generated.h"

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	None = 0,
	Talk UMETA(DisplayeName="��ȭ�ϱ�"),
	Get UMETA(DisplayeName = "ȹ���ϱ�"),
	Move UMETA(DisplayeName = "�̵��ϱ�"),
	Active UMETA(DisplayeName = "�۵��ϱ�")
};

class GAME_API InteractionType
{
public:
	InteractionType();
	~InteractionType();
};
