// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemSlotStruct.h"
#include "SlotDrag.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API USlotDrag : public UDragDropOperation
{

	GENERATED_BODY()

public:

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FS_ItemSlot m_sItemInfo;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		int32 m_nDraggedIndex;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool m_bIsItemFromInventory;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool m_bIsItemFromEquipment;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool m_bIsItemFromQuickSlot;

	UFUNCTION(BlueprintCallable)
		void Init(UPARAM(ref) const FS_ItemSlot &sItemIfno,int32 nDraggedIndex,bool bFromInventory, bool bFromEquipment, bool bFromQuickSlot);

};
