// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConversationWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API UConversationWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetConversationText(UPARAM(ref) const FText& tNPCName, UPARAM(ref) const FText& tConversationText);
	virtual void SetConversationText_Implementation(UPARAM(ref) const FText& tNPCName, UPARAM(ref) const FText& tConversationText);
};
