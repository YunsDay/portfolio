#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "Engine/DataTable.h"
#include "ItemSlotStruct.generated.h"


USTRUCT(Atomic,BlueprintType)
struct FS_ItemSlot : public FTableRowBase
{
public:
	GENERATED_USTRUCT_BODY()

public:
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FName ID_DataTable;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FName tItemName;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		UTexture2D* pImage;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		int nQuantity;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		int nMaxQuantity;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool bStackAble;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool bUseAble;

};