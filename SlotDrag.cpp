// Fill out your copyright notice in the Description page of Project Settings.


#include "SlotDrag.h"

void USlotDrag::Init(UPARAM(ref) const FS_ItemSlot& sItemIfno, int32 nDraggedIndex, bool bFromInventory, bool bFromEquipment, bool bFromQuickSlot)
{
	m_sItemInfo = sItemIfno;
	m_nDraggedIndex = nDraggedIndex;
	m_bIsItemFromInventory = bFromInventory;
	m_bIsItemFromEquipment = bFromEquipment;
	m_bIsItemFromQuickSlot = bFromQuickSlot;
}
