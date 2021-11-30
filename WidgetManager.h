// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UIType.h"
#include "WidgetManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UWidgetManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWidgetManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	UClass* CreateWidgetFromFileRoute(const FString & FileRoute);

public:
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		TArray<bool> m_arIsInterfaceOpen;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		TArray<bool> m_arIsUIOpen;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		int m_nOpenInterfaceCount;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		UMyUserWidget* m_pHUD;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		class UConversationWidget* m_pConversationWidget;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> m_pConversationSubClass;



public:
	UFUNCTION(BlueprintCallable)
		void InitWidgetManager(ATPSCharacter* cOwner);
	UFUNCTION(BlueprintCallable)
		void OpenCloseInterface(EInterfaceType eInterfaceType, bool bMoveLimit, bool bShowMouseCursor);
	UFUNCTION(BlueprintCallable)
		void OpenCloseUI(EUIType eUIType, bool bMoveLimit, bool bShowMouseCursor);
	UFUNCTION(BlueprintCallable)
		void SetInputMode(bool bMoveLimit, bool bShowMouseCursor);
	UFUNCTION(BlueprintCallable)
		void ShowNPCQuestInterface(FName naQuestID);
	UFUNCTION(BlueprintCallable)
		void SetInteractionType(EInteractionType eInteractionType);
	UFUNCTION(BlueprintCallable)
		void ShowCooltimeLine(float fCooltime, FText tMessage);
	UFUNCTION(BlueprintCallable)
		void CloseCooltimeLine();
	UFUNCTION(BlueprintCallable)
		void UpdateCooltime(float fCooltime);
	UFUNCTION(BlueprintCallable)
		UMyUserWidget* GetUI(EUIType eUIType);
	UFUNCTION(BlueprintCallable)
		void SetInformationText(UPARAM(ref) const FText& tInformationText);
	UFUNCTION(BlueprintCallable)
		void CloseUIEnforcement(EUIType eUIType, bool bMoveLimit, bool bShowMouseCursor);
	UFUNCTION(BlueprintCallable)
		void ShowHUD();
	UFUNCTION(BlueprintCallable)
		void HideHUD();
	UFUNCTION(BlueprintCallable)
		bool GetIsVisibleHUD();
	UFUNCTION(BlueprintCallable)
		void OpenConversationWidget(UPARAM(ref) const FText& tNPCName, UPARAM(ref) const FText& tConversationText);
	UFUNCTION(BlueprintCallable)
		void CloseCOnversationWidget();
	UFUNCTION(BlueprintCallable)
		void Release();

	
private:
	UFUNCTION(BlueprintCallable)
		void SetNPCQuestInterface(FName naQuestID);


};
