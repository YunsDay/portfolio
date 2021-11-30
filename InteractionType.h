// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionType.generated.h"

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	None = 0,
	Talk UMETA(DisplayeName="대화하기"),
	Get UMETA(DisplayeName = "획득하기"),
	Move UMETA(DisplayeName = "이동하기"),
	Active UMETA(DisplayeName = "작동하기")
};

class GAME_API InteractionType
{
public:
	InteractionType();
	~InteractionType();
};
