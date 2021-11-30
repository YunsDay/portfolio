// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyWeaponType.h"
#include "ItemSlotStruct.h"
#include "StructEquipmentItem.h"
#include "CharacterInterface.h"
#include "Components/TimelineComponent.h"
#include "StructSkill.h"
#include "TPSCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBindingEvent);

UENUM()
enum class EAttackType : uint8
{
	NONE = 0,
	LowAttack = 1,
	HighAttack = 4,
	Max,
};

UCLASS()
class GAME_API ATPSCharacter : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Interface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void CharacterTakeDamage(float fDamage, bool bTakeDown);
	virtual void CharacterTakeDamage_Implementation(float fDamage, bool bTakeDown) override;

	//KeyEvent
	UFUNCTION()
		void MoveForward(float fAxisValue);
	UFUNCTION()
		void MoveRight(float fAxisValue);
	UFUNCTION()
		void Turn(float fAxisValue);
	UFUNCTION()
		void LookUp(float fAxisValue);
	UFUNCTION()
		void StartRoll();
	UFUNCTION()
		void StartSprint();
	UFUNCTION()
		void StopSprint();
	UFUNCTION()
		void Interaction();
	UFUNCTION()
		void SkillPressQ();
	UFUNCTION()
		void SkillPressE();
	UFUNCTION()
		void SkillPressR();
	UFUNCTION()
		void OpenInventory();
	UFUNCTION()
		void OpenCharacterState();
	UFUNCTION()
		void UseFirstItem();
	UFUNCTION()
		void UseSecondItem();
	UFUNCTION()
		void UseThirdItem();


	//Component
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		class UInventory* m_Inventory;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		class UCharacterState* m_CharacterState;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UWidgetManager* m_WidgetManager;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		class UCameraComponent* m_MainCamera;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		class USpringArmComponent* m_FollowCamera;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		class UMySkillSystem* m_SkillSystem;


public:
	//variable
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool m_bIsRolling;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool m_bIsDeath;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool m_bIsWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool m_bMoveLimit;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool m_bSaveAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool m_bIsAttacking;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool m_bSkillReady;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool m_bInvincibility;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool m_bOtherActionLimit;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool m_bShowHUD;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int m_nSkillIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EWeaponType m_eWeaponType;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AMyWeapon* m_pWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float m_fSkillDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UAnimMontage* m_pSkillAnimMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FS_Skill m_sSkillInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* m_pRollIngAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* m_pTakeDownAnimMontage;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UUserWidget* m_pDeathUI;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> m_pDeathUISubclass;
	


private:
	UPROPERTY()
		class UDataTable* m_DTdataTable;
	UPROPERTY()
		class UPlayerAnimInstance* m_pAnimInstance;

public:
	//Function
	UFUNCTION(BlueprintCallable)
		void SetWorkSpeed(float fSpeed);
	UFUNCTION(BlueprintCallable)
		void SetOtherActionLimit(bool bLimit);
	UFUNCTION(BlueprintCallable)
		void SetMoveLimit(bool bLimit);
	UFUNCTION(BlueprintCallable)
		void SetWeaponCollision(bool bOnOff);
	UFUNCTION(BlueprintCallable)
		void SetWeaponSkillCollision(bool bOnOff);
	UFUNCTION(BlueprintCallable)
		void GetDamageCaculate(float& fDamage, float& fCriticalDamage, float& fCriticalChancePercent);
	UFUNCTION(BlueprintCallable)
		void SetFollowCameraRotation(FVector vTargetVector);
	UFUNCTION(BlueprintCallable)
		void SetInputMode(bool bMovingLimit, bool bShowMouseCursor);
	UFUNCTION(BlueprintCallable)
		void SetShowHUDInLevelBlurprint(bool bShow);
	UFUNCTION(BlueprintCallable)
		void RotateLastInput();
	UFUNCTION(BlueprintCallable)
		void Death();
	
private:
	UFUNCTION(BlueprintCallable)
		void SetBeginInputModeUseBeginEvent(bool bSetInputLimit);
	UFUNCTION(BlueprintCallable)
		void AllReset();

	//GameStart
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bCameraTimelineStart;
	UFUNCTION(BlueprintCallable)
		void SetGameStartInLevelBlueprint(bool bStart);
	UFUNCTION(BlueprintCallable)
		void GameStart();

public:
	//Camera
	UPROPERTY()
		FTimeline m_CameraTimeline;
	UFUNCTION()
		void CameraTimelineUpdate();
	UFUNCTION()
		void CameraTimelineFinished();
	UPROPERTY(EditAnyWhere)
		class UCurveFloat* m_cCameraCurve;

	UFUNCTION(BlueprintCallable)
		void StartCamera();
	UFUNCTION(BlueprintCallable)
		void StartGameMode();

public:
	//Infomation
	UFUNCTION(BlueprintCallable)
		void OpenInformation(UPARAM(ref) const FText &tInformationText,bool bMoveLimit);
	UFUNCTION(BlueprintCallable)
		void CloseInformation();

	//Inventory
	UFUNCTION(BlueprintCallable)
		void AddItemFromDataTable(FName tItemDataTableRow);
	UFUNCTION(BlueprintCallable)
		void AddMoney(int nMoney);
private:
	UFUNCTION(BlueprintCallable)
		void UseItemFunction(int nKeyboardIndex);

	//Interaction
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, VisibleDefaultsOnly)
		FBindingEvent InteractionEventBinding;
	UFUNCTION(BlueprintCallable)
		void OpenInteractionUI(EInteractionType eInteractionType);
	UFUNCTION(BlueprintCallable)
		void CloseInteractionUI();

	//AttackTree
private:
	UFUNCTION(BlueprintCallable)
		void Attack(EAttackType eAttackType);
	UFUNCTION(BlueprintCallable)
		void LowAttack();
	UFUNCTION(BlueprintCallable)
		void HighAttack();
	UFUNCTION(BlueprintCallable)
		void ComboAttackSave();
	UFUNCTION(BlueprintCallable)
		void AttackComboTree();
	UFUNCTION(BlueprintCallable)
		void SetResetCombo();
	UFUNCTION(BlueprintCallable)
		void SetAttackMotion();
public:
	UFUNCTION(BlueprintCallable)
		void ResetCombo();
	UFUNCTION(BlueprintCallable)
		void OffWeaponCollision();

private:
	//AttackVariable
	UPROPERTY(VisibleAnywhere)
		int m_nAttackCount;
	UPROPERTY(VisibleAnywhere)
		TArray<UAnimMontage*> m_arMtAttackMotion;
	UPROPERTY(VisibleAnywhere)
		TArray<float> m_arAttackMotionMagnification;
	UPROPERTY(VisibleAnywhere)
		TArray<float> m_arAttackPlayRate;
	UPROPERTY(VisibleAnywhere)
		EAttackType m_eAttackType;
	UPROPERTY(VisibleAnywhere)
		class UDataTable* m_DTAttackMotion;

	//Equipment
public:
	UFUNCTION(BlueprintCallable)
		void Equip(UPARAM(ref) const FS_EquipmentItem& sEquipmentData);
	UFUNCTION(BlueprintCallable)
		void UnEquip(UPARAM(ref) const FS_EquipmentItem& sEquipmentData);

private:
	UFUNCTION(BlueprintCallable)
		void EquipEuipment(UPARAM(ref) const FS_EquipmentItem& sEquipmentData);
	UFUNCTION(BlueprintCallable)
		void UnEquipEquipment(UPARAM(ref) const FS_EquipmentItem& sEquipmentData);

private:
	UFUNCTION(BlueprintCallable)
		void EquipWeapon(UPARAM(ref) const FS_EquipmentItem& sEquipmentData);
	UFUNCTION(BlueprintCallable)
		void UnEquipWeapon(UPARAM(ref) const FS_EquipmentItem& sEquipmentData);


	//Conversation
public:
	UFUNCTION()
		void ConversationOpen(UPARAM(ref) const FText& tNPCName, UPARAM(ref) const FText& tConversationText);
	UFUNCTION()
		void ConversationClose();




	//SkillFunction
private:
	UFUNCTION(BlueprintCallable)
		void SetSkill();
	UFUNCTION(BlueprintCallable)
		bool UseSkill(int nKeyboardIndex, float& fDamage);
	UFUNCTION(BlueprintCallable)
		void UseSkillFunction(int nKeyboardIndex);
	UFUNCTION(BlueprintCallable)
		void MakeRangeDamage(FVector vMakeLocation);

	//TimelineCooltime
public:
	UPROPERTY()
		FTimeline m_timelineCoolTime;
	UFUNCTION(BlueprintCallable)
		void Cooltime(float fCoolTime, FText tMessage);
	UFUNCTION(BlueprintCallable)
		void CooltimeStop();
	

private:
	UFUNCTION()
		void TimelineUpdate();
	UFUNCTION()
		void TimelineFinished();
	UFUNCTION(BlueprintCallable)
		void FinishedCooltime();
	UPROPERTY(EditAnyWhere)
		class UCurveFloat* m_cCurve;
	UFUNCTION(BlueprintCallable)
		void UpdateCooltime(float fCoolTime);


public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, VisibleDefaultsOnly)
		FBindingEvent EndCoolTimeEventBinding;
};
