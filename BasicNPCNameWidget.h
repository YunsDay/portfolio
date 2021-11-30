// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BasicNPCNameWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API UBasicNPCNameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Init(UPARAM(ref) const FText& tNPCName, UPARAM(ref) const FText& tRoleText);
	virtual void Init_Implementation(UPARAM(ref) const FText& tNPCName, UPARAM(ref) const FText& tRoleText);
};
