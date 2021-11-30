// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WidgetManager.h"
#include "InteractionType.h"
#include "TPSCharacter.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.h"
#include "ConversationWidget.h"

// Sets default values for this component's properties
UWidgetManager::UWidgetManager() :
	m_arIsInterfaceOpen{},
	m_arIsUIOpen{},
	m_nOpenInterfaceCount{},
	m_pHUD{}
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	m_arIsInterfaceOpen.Init(false, (uint8)EInterfaceType::Max);
	m_arIsUIOpen.Init(false,(uint8)EUIType::Max);


}

// Called when the game starts
void UWidgetManager::BeginPlay()
{
	Super::BeginPlay();

	
	// ...
	
}


// Called every frame
void UWidgetManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UClass* UWidgetManager::CreateWidgetFromFileRoute(const FString& FileRoute)
{
	FStringClassReference MyWidgetClassRef(FileRoute);
	return MyWidgetClassRef.TryLoadClass<UMyUserWidget>();
}


void UWidgetManager::InitWidgetManager(ATPSCharacter* cOwner)
{
	m_pHUD = CreateWidget<UMyUserWidget>(GetWorld()->GetFirstPlayerController(),CreateWidgetFromFileRoute(TEXT("/Game/Game/UI/HUD.HUD_C")));
	m_pHUD->Init(cOwner);
	m_pHUD->AddToViewport();

	m_pConversationWidget = CreateWidget<UConversationWidget>(GetWorld()->GetFirstPlayerController(),m_pConversationSubClass);
}

void UWidgetManager::OpenCloseInterface(EInterfaceType eInterfaceType, bool bMoveLimit, bool bShowMouseCursor)
{
	if (m_arIsInterfaceOpen[(uint8)eInterfaceType])
	{
		m_arIsInterfaceOpen[(uint8)eInterfaceType] = false;
		m_pHUD->ShowHideInterface(ESlateVisibility::Hidden, eInterfaceType);
		m_nOpenInterfaceCount--;
		if (m_nOpenInterfaceCount == 0)
		{
			SetInputMode(false, false);
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
		}
	}
	else
	{
		m_arIsInterfaceOpen[(uint8)eInterfaceType] = true;
		m_pHUD->ShowHideInterface(ESlateVisibility::Visible, eInterfaceType);
		SetInputMode(bMoveLimit,bShowMouseCursor);
		m_nOpenInterfaceCount++;
	}
}

void UWidgetManager::OpenCloseUI(EUIType eUIType, bool bMoveLimit, bool bShowMouseCursor)
{
	if (m_arIsUIOpen[(uint8)eUIType])
	{
		m_arIsUIOpen[(uint8)eUIType] = false;
		m_pHUD->ShowHideUI(ESlateVisibility::Hidden, eUIType);
	}
	else
	{
		m_arIsUIOpen[(uint8)eUIType] = true;
		m_pHUD->ShowHideUI(ESlateVisibility::Visible, eUIType);		
	}

	SetInputMode(bMoveLimit, bShowMouseCursor);

}

void UWidgetManager::SetInputMode(bool bMoveLimit, bool bShowMouseCursor)
{
	GetWorld()->GetFirstPlayerController()->SetIgnoreLookInput(bMoveLimit);
	GetWorld()->GetFirstPlayerController()->SetIgnoreMoveInput(bMoveLimit);

	if (bShowMouseCursor)
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
	else 
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = bShowMouseCursor;
}

void UWidgetManager::ShowNPCQuestInterface(FName naQuestID)
{
}

void UWidgetManager::SetInteractionType(EInteractionType eInteractionType)
{
	m_pHUD->SetInteractionType(eInteractionType);
}


void UWidgetManager::ShowCooltimeLine(float fCooltime, FText tMessage)
{
	m_pHUD->SetCoolTimeText(tMessage, fCooltime);
	OpenCloseUI(EUIType::CooltimeLineUI, true, false);
}

void UWidgetManager::CloseCooltimeLine()
{
	CloseUIEnforcement(EUIType::CooltimeLineUI, false, false);
}

void UWidgetManager::UpdateCooltime(float fCooltime)
{
	m_pHUD->UpdateCooltime(fCooltime);
}

UMyUserWidget* UWidgetManager::GetUI(EUIType eUIType)
{
	return nullptr;
}

void UWidgetManager::SetInformationText(UPARAM(ref) const FText& tInformationText)
{
	m_pHUD->SetInformationText(tInformationText);
}

void UWidgetManager::CloseUIEnforcement(EUIType eUIType, bool bMoveLimit, bool bShowMouseCursor)
{
	m_arIsUIOpen[(uint8)eUIType] = false;
	m_pHUD->ShowHideUI(ESlateVisibility::Hidden, eUIType);
	SetInputMode(bMoveLimit, bShowMouseCursor);
}

void UWidgetManager::ShowHUD()
{
	m_pHUD->SetVisibility(ESlateVisibility::Visible);
}

void UWidgetManager::HideHUD()
{
	m_pHUD->SetVisibility(ESlateVisibility::Hidden);
}

bool UWidgetManager::GetIsVisibleHUD()
{
	return m_pHUD->GetIsVisible();
}

void UWidgetManager::OpenConversationWidget(UPARAM(ref) const FText& tNPCName, UPARAM(ref) const FText& tConversationText)
{
	HideHUD();
	m_pConversationWidget->SetConversationText(tNPCName,tConversationText);
	m_pConversationWidget->AddToViewport();
}

void UWidgetManager::CloseCOnversationWidget()
{
	m_pConversationWidget->RemoveFromParent();
	ShowHUD();
}

void UWidgetManager::Release()
{
	m_pHUD->RemoveFromParent();
}

void UWidgetManager::SetNPCQuestInterface(FName naQuestID)
{
}
