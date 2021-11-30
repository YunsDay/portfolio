// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemSlotStruct.h"
#include "StructEquipmentItem.h"
#include "StructUseItemInfoDataTable.h"
#include "Inventory.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
class GAME_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventory();

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		int32 m_nNumOfSlot;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		int32 m_nMoney;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		TArray<FS_ItemSlot> m_arInventory;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		TArray<FS_ItemSlot> m_arQuickSlot;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		TArray<FS_ItemSlot> m_arEquipSlot;


private:


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
		void Release();
	UFUNCTION(BlueprintCallable)
		void AddItem(UPARAM(ref)FS_ItemSlot& sItemInfo);
	
	
	
	UFUNCTION(BlueprintCallable)
		void ClearItem(UPARAM(ref) TArray<FS_ItemSlot>& arItemSlot, int32 nClearIndex);
	
	UFUNCTION(BlueprintCallable)
		void SwapItemInfo(UPARAM(ref)TArray<FS_ItemSlot>& arItemSlot, int32 nDstIndex, int32 nSrcIndex);
	UFUNCTION(BlueprintCallable)
		void CombineItemToOtherInventory(UPARAM(ref)TArray<FS_ItemSlot>& arFromItemSlot, int32 nFromIndex, UPARAM(ref)TArray<FS_ItemSlot>& arToItemSlot, int32 nToIndex);
	UFUNCTION(BlueprintCallable)
		void MoveItemToOtherInventory(UPARAM(ref)TArray<FS_ItemSlot>& arFromItemSlot, int32 nFromIndex, UPARAM(ref)TArray<FS_ItemSlot>& arToItemSlot, int32 nToIndex);
	UFUNCTION(BlueprintCallable)
		bool UseMoney(int32 nUseAmount);
	UFUNCTION(BlueprintCallable)
		void DecrementItemAmount(int32 nSlotIndex);
	UFUNCTION(BlueprintCallable)
		void GetEquipmentItemFromDataTable(UPARAM(ref)const FS_ItemSlot& sItemInfo, FS_EquipmentItem & sReturnValue);
	UFUNCTION(BlueprintCallable)
		void AddMoney(int nAddMoneyAmount);
	UFUNCTION(BlueprintCallable)
		void UseItemToQuickSlot(int32 nSlotIndex, FS_UseItemDataTable & sUseItemInfo);

private:
	UFUNCTION(BlueprintCallable)
		bool FindSameItem(UPARAM(ref)const FS_ItemSlot& sItemInfo, int32& nFindIndex);
	UFUNCTION(BlueprintCallable)
		void AddQuantity(UPARAM(ref)const FS_ItemSlot& sItemInfo, int32 nSameItemIndex);
	UFUNCTION(BlueprintCallable)
		void CreateStack(UPARAM(ref)const FS_ItemSlot& sItemInfo);
	UFUNCTION(BlueprintCallable)
		void SwapItemToOtherInventory(UPARAM(ref)TArray<FS_ItemSlot>& arDstItemSlot, int32 nDstIndex, UPARAM(ref)TArray<FS_ItemSlot>& arSrcItemSlot, int32 nSrcIndex);
	UFUNCTION(BlueprintCallable)
		bool FindEmptyInventoryIndex(int & nEmptyIndex);
	UPROPERTY()
		class UDataTable* m_pItemWeaponData;
	UPROPERTY()
		class UDataTable* m_pUseItemData;

};
