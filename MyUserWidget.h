// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIType.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Init(ACharacter* owner);
	virtual void Init_Implementation(ACharacter* owner);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ShowHideInterface(ESlateVisibility eInVisibility, EInterfaceType eInterfaceType);
	virtual void ShowHideInterface_Implementation(ESlateVisibility eInVisibility, EInterfaceType eInterfaceType);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ShowHideUI(ESlateVisibility eInVisibility, EUIType eUIType);
	virtual void ShowHideUI_Implementation(ESlateVisibility eInVisibility, EUIType eUIType);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetInteractionType(EInteractionType eInteractionType);
	virtual void SetInteractionType_Implementation(EInteractionType eInteractionType);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetInformationText(UPARAM(ref) const FText & tInfomationText);
	virtual void SetInformationText_Implementation(UPARAM(ref) const FText& tInfomationText);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetCoolTimeText(UPARAM(ref) const FText& tInfomationText, float fCooltime);
	virtual void SetCoolTimeText_Implementation(UPARAM(ref) const FText& tInfomationText, float fCooltime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdateCooltime(float fCooltime);
	virtual void UpdateCooltime_Implementation(float fCooltime);

};
