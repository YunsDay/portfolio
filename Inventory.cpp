// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UInventory::UInventory():
	m_pItemWeaponData{},
	m_nNumOfSlot{},
	m_nMoney{},
	m_arInventory{},
	m_arQuickSlot{},
	m_arEquipSlot{},
	m_pUseItemData{}
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	FS_ItemSlot sEmptySlot{};
	m_nNumOfSlot = 25;
	m_arInventory.Init(sEmptySlot, 25);
	m_arEquipSlot.Init(sEmptySlot, 3);
	m_arQuickSlot.Init(sEmptySlot, 3);
	// ...
}


// Called when the game starts
void UInventory::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventory::Release()
{
	for (int i = 0; i < m_nNumOfSlot; i++)
	{
		ClearItem(m_arInventory,i);
	}
	for (int i = 0; i < 3; i++)
	{
		ClearItem(m_arQuickSlot,i);
		ClearItem(m_arEquipSlot,i);
	}
}

void UInventory::AddItem(UPARAM(ref)FS_ItemSlot& sItemInfo)
{
	int nSameIndex{};

	if (sItemInfo.bStackAble && FindSameItem(sItemInfo, nSameIndex))
		AddQuantity(sItemInfo, nSameIndex);
	else
		CreateStack(sItemInfo);
}

bool UInventory::FindEmptyInventoryIndex(int& nEmptyIndex)
{
	int nIndex{};

	nIndex = 0;
	while (m_arInventory[nIndex].pImage)
	{
		nIndex++;
	}
	if (nIndex >= m_nNumOfSlot)
		return false;

	nEmptyIndex = nIndex;
	return true;
}

void UInventory::ClearItem(UPARAM(ref)TArray<FS_ItemSlot>& arItemSlot, int32 nClearIndex)
{
	FS_ItemSlot sEmptySlot{};

	arItemSlot[nClearIndex] = sEmptySlot;
}

void UInventory::SwapItemInfo(UPARAM(ref)TArray<FS_ItemSlot>& arItemSlot, int32 nDstIndex, int32 nSrcIndex)
{
	FS_ItemSlot sTmp{};
	sTmp = arItemSlot[nDstIndex];
	arItemSlot[nDstIndex] = arItemSlot[nSrcIndex];
	arItemSlot[nSrcIndex] = sTmp;
}

void UInventory::CombineItemToOtherInventory(UPARAM(ref)TArray<FS_ItemSlot>& arFromItemSlot, int32 nFromIndex, UPARAM(ref)TArray<FS_ItemSlot>& arToItemSlot, int32 nToIndex)
{
	arToItemSlot[nToIndex].nQuantity += arFromItemSlot[nFromIndex].nQuantity;
	if (arToItemSlot[nToIndex].nQuantity > arToItemSlot[nToIndex].nMaxQuantity)
	{
		int nTmp;
		nTmp = arToItemSlot[nToIndex].nQuantity - arToItemSlot[nToIndex].nMaxQuantity;
		arToItemSlot[nToIndex].nQuantity = arToItemSlot[nToIndex].nMaxQuantity;
		arFromItemSlot[nFromIndex].nQuantity = nTmp;
	}
	else
	{
		ClearItem(arFromItemSlot, nFromIndex);
	}
}

void UInventory::MoveItemToOtherInventory(UPARAM(ref)TArray<FS_ItemSlot>& arFromItemSlot, int32 nFromIndex, UPARAM(ref)TArray<FS_ItemSlot>& arToItemSlot, int32 nToIndex)
{
	if (arToItemSlot[nToIndex].pImage && arToItemSlot[nToIndex].tItemName != arFromItemSlot[nFromIndex].tItemName)
		SwapItemToOtherInventory(arFromItemSlot,nFromIndex,arToItemSlot,nToIndex);
	else if (arToItemSlot[nToIndex].tItemName == arFromItemSlot[nFromIndex].tItemName)
	{
		CombineItemToOtherInventory(arFromItemSlot,nFromIndex,arToItemSlot,nToIndex);
	}
	else
	{
		arToItemSlot[nToIndex] = arFromItemSlot[nFromIndex];
		ClearItem(arFromItemSlot,nFromIndex);
	}
}

void UInventory::UseItemToQuickSlot(int32 nSlotIndex, FS_UseItemDataTable& sUseItemInfo)
{
	m_pUseItemData = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, TEXT("DataTable'/Game/Game/System/Inven/DataTable/DT_UseItemData.DT_UseItemData'")));
	FS_UseItemDataTable* pUseItem{};

	if (m_pUseItemData)
		pUseItem = m_pUseItemData->FindRow<FS_UseItemDataTable>(m_arQuickSlot[nSlotIndex].ID_DataTable, TEXT(""));

	if (pUseItem)
		sUseItemInfo = *pUseItem;

}

bool UInventory::UseMoney(int32 nUseAmount)
{
	if (m_nMoney - nUseAmount >= 0)
	{
		m_nMoney -= nUseAmount;
		return true;
	}
	else
		return false;

	return false;
}

void UInventory::DecrementItemAmount(int32 nSlotIndex)
{
	if (m_arQuickSlot[nSlotIndex].pImage)
		m_arQuickSlot[nSlotIndex].nQuantity--;

	if (m_arQuickSlot[nSlotIndex].nQuantity <= 0)
		ClearItem(m_arQuickSlot, nSlotIndex);
}

void UInventory::GetEquipmentItemFromDataTable(UPARAM(ref) const FS_ItemSlot& sItemInfo, FS_EquipmentItem& sReturnValue)
{
	FString strDataTableRoute = TEXT("DataTable'/Game/Game/System/Inven/DataTable/DT_Equipment.DT_Equipment'");
	m_pItemWeaponData = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, *strDataTableRoute));
	FS_EquipmentItem* pEquipmentItem{};

	if (m_pItemWeaponData)
		pEquipmentItem = m_pItemWeaponData->FindRow<FS_EquipmentItem>(sItemInfo.ID_DataTable, FString(""));

	if (pEquipmentItem)
		sReturnValue = *pEquipmentItem;

}

void UInventory::AddMoney(int nAddMoneyAmount)
{
	m_nMoney += nAddMoneyAmount;
}

bool UInventory::FindSameItem(UPARAM(ref) const FS_ItemSlot& sItemInfo, int32& nFindIndex)
{
	int32 nIndex{};
	nIndex = 0;
	while (nIndex < m_nNumOfSlot)
	{
		if (m_arInventory[nIndex].tItemName == sItemInfo.tItemName && m_arInventory[nIndex].nQuantity != m_arInventory[nIndex].nMaxQuantity)
		{
			nFindIndex = nIndex;
			return true;
		}
		nIndex++;
	}
	return false;
}

void UInventory::AddQuantity(UPARAM(ref) const FS_ItemSlot& sItemInfo, int32 nSameItemIndex)
{
	FS_ItemSlot sEmptySlot{};

	m_arInventory[nSameItemIndex].nQuantity += sItemInfo.nQuantity;
	if (m_arInventory[nSameItemIndex].nQuantity > m_arInventory[nSameItemIndex].nMaxQuantity)
	{
		sEmptySlot = m_arInventory[nSameItemIndex];
		sEmptySlot.nQuantity -= m_arInventory[nSameItemIndex].nMaxQuantity;
		CreateStack(sEmptySlot);
		m_arInventory[nSameItemIndex].nQuantity = m_arInventory[nSameItemIndex].nMaxQuantity;
	}

}

void UInventory::CreateStack(UPARAM(ref) const FS_ItemSlot& sItemInfo)
{
	int nEmptyIndex;
	if(FindEmptyInventoryIndex(nEmptyIndex))
		m_arInventory[nEmptyIndex] = sItemInfo;
}

void UInventory::SwapItemToOtherInventory(UPARAM(ref)TArray<FS_ItemSlot>& arDstItemSlot, int32 nDstIndex, UPARAM(ref)TArray<FS_ItemSlot>& arSrcItemSlot, int32 nSrcIndex)
{
	FS_ItemSlot sTmp{};
	sTmp = arDstItemSlot[nDstIndex];
	arDstItemSlot[nDstIndex] = arSrcItemSlot[nSrcIndex];
	arSrcItemSlot[nSrcIndex] = sTmp;
}
