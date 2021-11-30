// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Inventory.h"
#include "CharacterState.h"
#include "WidgetManager.h"
#include "MyWeapon.h"
#include "MySkillSystem.h"
#include "StructUseItemInfoDataTable.h"
#include "RecoveryType.h"
#include "StructAttackMotion.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerAnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "EnemyInterface.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ATPSCharacter::ATPSCharacter():
	m_bIsRolling{},
	m_bIsDeath{},
	m_bIsWeapon{},
	m_bMoveLimit{},
	m_bSaveAttack{},
	m_bIsAttacking{},
	m_bSkillReady{},
	m_bInvincibility{},
	m_bOtherActionLimit{},
	m_bShowHUD{},
	m_nAttackCount{},
	m_nSkillIndex{},
	m_Inventory{},
	m_CharacterState{},
	m_MainCamera{},
	m_FollowCamera{},
	m_eWeaponType{},
	m_pWeapon{},
	m_WidgetManager{},
	m_SkillSystem{},
	m_DTdataTable{},
	m_pAnimInstance{},
	m_fSkillDamage{},
	m_pSkillAnimMontage{},
	m_pRollIngAnimMontage{},
	m_pTakeDownAnimMontage{},
	m_arMtAttackMotion{},
	m_arAttackMotionMagnification{},
	m_arAttackPlayRate{},
	m_eAttackType{},
	m_DTAttackMotion{},
	m_timelineCoolTime{},
	m_cCurve{},
	m_bCameraTimelineStart{}

{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	m_FollowCamera = CreateDefaultSubobject<USpringArmComponent>(TEXT("FollowCamera"));
	m_FollowCamera->SetupAttachment(reinterpret_cast<USceneComponent*>(GetCapsuleComponent()));
	m_FollowCamera->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f,8.5f), FRotator(0.0f, 0.0f, 0.0f));
	m_FollowCamera->TargetArmLength = 450.0f;
	m_FollowCamera->SocketOffset = FVector(0.0f,0.0f,45.0f);
	m_FollowCamera->bUsePawnControlRotation = true;

	m_MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	m_MainCamera->SetupAttachment(reinterpret_cast<USceneComponent*>(m_FollowCamera));
	m_MainCamera->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	m_MainCamera->bUsePawnControlRotation = true;


	m_Inventory = CreateDefaultSubobject<UInventory>(TEXT("BPC_Inventory"));
	
	m_CharacterState = CreateDefaultSubobject<UCharacterState>(TEXT("BPC_CharacterState"));
	
	m_WidgetManager = CreateDefaultSubobject<UWidgetManager>(TEXT("BPC_WidgetManager"));
	
	m_SkillSystem = CreateDefaultSubobject<UMySkillSystem>(TEXT("BPC_SkillSystem"));

}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	m_arMtAttackMotion.Init(nullptr, 7);
	m_arAttackMotionMagnification.Init(0.0f, 7);
	m_arAttackPlayRate.Init(0.0f, 7);
	
	m_CharacterState->Init(this);
	m_WidgetManager->InitWidgetManager(this);

	if (m_cCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEvent TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, TEXT("TimelineUpdate"));
		TimelineFinishedCallback.BindUFunction(this, TEXT("TimelineFinished"));

		m_timelineCoolTime.AddInterpFloat(m_cCurve, TimelineCallback);
		m_timelineCoolTime.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}

	if (m_cCameraCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEvent TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, TEXT("CameraTimelineUpdate"));
		TimelineFinishedCallback.BindUFunction(this, TEXT("CameraTimelineFinished"));

		m_CameraTimeline.AddInterpFloat(m_cCameraCurve,TimelineCallback);
		m_CameraTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}

	if (m_bCameraTimelineStart)
		GameStart();

	if (m_bShowHUD)
	{
		m_WidgetManager->ShowHUD();
		StartGameMode();
	}

	m_pAnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	m_pAnimInstance->Init(this);

	m_pDeathUI = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), m_pDeathUISubclass);;
}

void ATPSCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	InteractionEventBinding.Clear();

	m_WidgetManager->Release();
	m_Inventory->Release();

	m_pDeathUI->RemoveFromParent();
	m_pDeathUI = nullptr;
}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_timelineCoolTime.TickTimeline(DeltaTime);
	m_CameraTimeline.TickTimeline(DeltaTime);
}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ATPSCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ATPSCharacter::LookUp);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ATPSCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ATPSCharacter::StopSprint);
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &ATPSCharacter::StartRoll);
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &ATPSCharacter::Interaction);
	PlayerInputComponent->BindAction("OpenInventory", IE_Pressed, this, &ATPSCharacter::OpenInventory);
	PlayerInputComponent->BindAction("OpenCharacterState",IE_Pressed,this, &ATPSCharacter::OpenCharacterState);
	PlayerInputComponent->BindAction("UseFirstItem",IE_Pressed,this,&ATPSCharacter::UseFirstItem);
	PlayerInputComponent->BindAction("UseSecondItem", IE_Pressed,this,&ATPSCharacter::UseSecondItem);
	PlayerInputComponent->BindAction("UseThirdItem", IE_Pressed,this, &ATPSCharacter::UseThirdItem);
	PlayerInputComponent->BindAction("LowAttack", IE_Pressed, this, &ATPSCharacter::LowAttack);
	PlayerInputComponent->BindAction("HighAttack", IE_Pressed, this, &ATPSCharacter::HighAttack);
	PlayerInputComponent->BindAction("SkillQ", IE_Pressed, this, &ATPSCharacter::SkillPressQ);
	PlayerInputComponent->BindAction("SkillE", IE_Pressed, this, &ATPSCharacter::SkillPressE);
	PlayerInputComponent->BindAction("SkillR", IE_Pressed, this, &ATPSCharacter::SkillPressR);

}

void ATPSCharacter::CharacterTakeDamage_Implementation(float fDamage, bool bTakeDown)
{
	if (!m_bInvincibility && !m_bIsDeath)
	{
		m_CharacterState->TakeDamage(fDamage);
		if (bTakeDown)
		{
			m_pAnimInstance->OnInterruptedBinding.AddDynamic(this, &ATPSCharacter::AllReset);
			m_pAnimInstance->OnCompletedBinding.AddDynamic(this, &ATPSCharacter::AllReset);
			m_pAnimInstance->PlayMontage(m_pTakeDownAnimMontage,1.0f);
		}
	}

	if (m_CharacterState->GetState(EStateType::Hp) <= 0 && !m_bIsDeath)
		Death();
		
}

void ATPSCharacter::MoveForward(float fAxisValue)
{
	FVector Direction;
	FRotator Rotate;
	
	Rotate = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	Direction = FQuat(Rotate).GetForwardVector();
	if (!m_bMoveLimit)
		AddMovementInput(Direction, fAxisValue);

}

void ATPSCharacter::MoveRight(float fAxisValue)
{
	FVector Direction;
	FRotator Rotate;

	Rotate = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	Direction = FQuat(Rotate).GetRightVector();
	if(!m_bMoveLimit)
		AddMovementInput(Direction, fAxisValue);
}

void ATPSCharacter::Turn(float fAxisValue)
{
	AddControllerYawInput(fAxisValue);
}

void ATPSCharacter::LookUp(float fAxisValue)
{
	AddControllerPitchInput(fAxisValue);
}

void ATPSCharacter::StartRoll()
{
	if (!m_bIsRolling && m_CharacterState->UseStaminaIfCan(10.0f) && m_pAnimInstance)
	{
		m_bIsRolling = true;
		RotateLastInput();
		m_pAnimInstance->OnCompletedBinding.AddDynamic(this, &ATPSCharacter::AllReset);
		m_pAnimInstance->OnInterruptedBinding.AddDynamic(this, &ATPSCharacter::AllReset);
		m_pAnimInstance->PlayMontage(m_pRollIngAnimMontage, 1.5f);
	}
}

void ATPSCharacter::StartSprint()
{
	SetWorkSpeed(700.0f);
	m_CharacterState->SetIsStaminaUse(true);
}

void ATPSCharacter::StopSprint()
{
	SetWorkSpeed(500.0f);
	m_CharacterState->SetIsStaminaUse(false);
}

void ATPSCharacter::SetAttackMotion()
{
	
	FString strItemDataPath = TEXT("DataTable'/Game/Game/System/AttackTree/DT_AttackMotion.DT_AttackMotion'");
	m_DTAttackMotion = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, *strItemDataPath));
	FS_AttackMotion* pAttackMotion{};
	FS_AttackMotion sAttackMotion;

	if (m_DTAttackMotion)
		pAttackMotion = m_DTAttackMotion->FindRow<FS_AttackMotion>(m_pWeapon->m_tWeaponAttackMotionRow, FString(""));

	if (pAttackMotion)
		sAttackMotion = *pAttackMotion;

	m_arMtAttackMotion[1] = sAttackMotion.mtFirstLowMotion;
	m_arMtAttackMotion[2] = sAttackMotion.mtSecondLowMotion;
	m_arMtAttackMotion[3] = sAttackMotion.mtThirdLowMotion;
	m_arMtAttackMotion[4] = sAttackMotion.mtFirstHighMotion;
	m_arMtAttackMotion[5] = sAttackMotion.mtSecondHighMotion;
	m_arMtAttackMotion[6] = sAttackMotion.mtThirdHighMotion;

	m_arAttackMotionMagnification[1] = sAttackMotion.fFirstLowMotionMagnification;
	m_arAttackMotionMagnification[2] = sAttackMotion.fSecondLowMotionMagnification;
	m_arAttackMotionMagnification[3] =  sAttackMotion.fThirdLowMotionMagnification;
	m_arAttackMotionMagnification[4] = sAttackMotion.fFirstHighMotionMagnification;
	m_arAttackMotionMagnification[5] = sAttackMotion.fSecondHighMotionMagnification;
	m_arAttackMotionMagnification[6] = sAttackMotion.fThirdHighMotionMagnification;

	m_arAttackPlayRate[1] = sAttackMotion.fFirstLowPlayRate;
	m_arAttackPlayRate[2] = sAttackMotion.fSecondLowPlayRate;
	m_arAttackPlayRate[3] = sAttackMotion.fThirdLowPlayRate;
	m_arAttackPlayRate[4] = sAttackMotion.fFirstHighPlayRate;
	m_arAttackPlayRate[5] = sAttackMotion.fSecondHighPlayRate;
	m_arAttackPlayRate[6] = sAttackMotion.fThirdHighPlayRate;

}

void ATPSCharacter::SetWorkSpeed(float fSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = fSpeed;
}

void ATPSCharacter::SetOtherActionLimit(bool bLimit)
{
	m_bOtherActionLimit = bLimit;
	m_bIsRolling = bLimit;
}

void ATPSCharacter::SetMoveLimit(bool bLimit)
{
	m_bMoveLimit = bLimit;
}

void ATPSCharacter::SetWeaponCollision(bool bOnOff)
{
	if(m_pWeapon)
		m_pWeapon->SetGenerateOverlapEvent(bOnOff);
}

void ATPSCharacter::SetWeaponSkillCollision(bool bOnOff)
{
	if(m_pWeapon)
		m_pWeapon->SetGenerateSkill(bOnOff);
}

void ATPSCharacter::GetDamageCaculate(float& fDamage, float& fCriticalDamage, float& fCriticalChancePercent)
{
	fDamage = m_CharacterState->GetState(EStateType::Damage);
	fCriticalDamage = m_CharacterState->GetState(EStateType::CriticalDamage);
	fCriticalChancePercent = m_CharacterState->GetState(EStateType::CriticalChance);
}

void ATPSCharacter::SetFollowCameraRotation(FVector vTargetVector)
{
}

void ATPSCharacter::SetInputMode(bool bMovingLimit, bool bShowMouseCursor)
{
	m_WidgetManager->SetInputMode(bMovingLimit,bShowMouseCursor);
}

void ATPSCharacter::SetShowHUDInLevelBlurprint(bool bShow)
{
	m_bShowHUD = bShow;
}

void ATPSCharacter::RotateLastInput()
{
	if (UKismetMathLibrary::NotEqual_RotatorRotator(UKismetMathLibrary::MakeRotFromX(GetCharacterMovement()->GetLastInputVector()), FRotator(0.0f, 0.0f, 0.0f), 0.0001f))
		SetActorRotation(UKismetMathLibrary::MakeRotFromX(GetCharacterMovement()->GetLastInputVector()));
	else
		SetActorRotation(GetActorRotation());	
}

void ATPSCharacter::Death()
{
	m_bIsDeath = true;
	GetController()->UnPossess();

	FTimerHandle sWaitHandle{};
	float fWaitTime{};

	fWaitTime = 2.0f;
	GetWorld()->GetTimerManager().SetTimer(sWaitHandle, FTimerDelegate::CreateLambda([&]()
	{
		SetInputMode(true, true);
		if(m_pDeathUI)
			m_pDeathUI->AddToViewport();

	}), fWaitTime, false);
	
}

void ATPSCharacter::OpenInformation(UPARAM(ref) const FText& tInformationText, bool bMoveLimit)
{
	m_WidgetManager->SetInformationText(tInformationText);
	SetMoveLimit(bMoveLimit);
	SetOtherActionLimit(bMoveLimit);
	m_WidgetManager->OpenCloseUI(EUIType::InformationUI, false, false);
}

void ATPSCharacter::CloseInformation()
{
	SetMoveLimit(false);
	SetOtherActionLimit(false);
	m_WidgetManager->CloseUIEnforcement(EUIType::InformationUI, false, false);
}

void ATPSCharacter::SetResetCombo()
{
	m_nAttackCount = 0;
	m_bSaveAttack = false;
	m_bIsAttacking = false;
}

void ATPSCharacter::SetBeginInputModeUseBeginEvent(bool bSetInputLimit)
{
	GetWorld()->GetFirstPlayerController()->SetIgnoreLookInput(bSetInputLimit);
	SetMoveLimit(bSetInputLimit);
	m_bOtherActionLimit = bSetInputLimit;
}

void ATPSCharacter::Equip(UPARAM(ref) const FS_EquipmentItem& sEquipmentData)
{
	switch (sEquipmentData.eEquipmentType)
	{
	case EEquipmentType::Weapon:
		EquipWeapon(sEquipmentData);
		break;
	case EEquipmentType::Equipment:
		EquipEuipment(sEquipmentData);
		break;
	default:
		break;
	}
}

void ATPSCharacter::UnEquip(UPARAM(ref) const FS_EquipmentItem& sEquipmentData)
{
	switch (sEquipmentData.eEquipmentType)
	{
	case EEquipmentType::Weapon:
		UnEquipWeapon(sEquipmentData);
		break;
	case EEquipmentType::Equipment:
		UnEquipEquipment(sEquipmentData);
		break;
	default:
		break;
	}
}

void ATPSCharacter::EquipEuipment(UPARAM(ref) const FS_EquipmentItem& sEquipmentData)
{
}

void ATPSCharacter::UnEquipEquipment(UPARAM(ref) const FS_EquipmentItem& sEquipmentData)
{
}

void ATPSCharacter::EquipWeapon(UPARAM(ref) const FS_EquipmentItem& sEquipmentData)
{
	if (m_pWeapon)
	{
		m_CharacterState->SetStateEquipWeapon(false);
		m_pWeapon->Destroy();
		m_pWeapon = nullptr;
	}


	m_pWeapon=GetWorld()->SpawnActor<AMyWeapon>(sEquipmentData.cWeapon, FVector(0.0f,0.0f,0.0f),FRotator(0.0f,0.0f,0.0f));
	m_pWeapon->m_pOwnerRef = this;
	m_pWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Weapon"));
	m_bIsWeapon = true;
	m_eWeaponType = m_pWeapon->m_eWeaponType;
	m_CharacterState->SetStateEquipWeapon(true);
	SetSkill();
	SetAttackMotion();

}

void ATPSCharacter::UnEquipWeapon(UPARAM(ref) const FS_EquipmentItem& sEquipmentData)
{	
	m_CharacterState->SetStateEquipWeapon(false);
	m_bIsWeapon = false;
	m_eWeaponType = EWeaponType::NONE;
	m_pWeapon->Destroy();
	m_pWeapon = nullptr;
	SetSkill();
}

void ATPSCharacter::ConversationOpen(UPARAM(ref) const FText& tNPCName, UPARAM(ref) const FText& tConversationText)
{
	SetMoveLimit(true);
	SetOtherActionLimit(true);
	m_WidgetManager->OpenConversationWidget(tNPCName,tConversationText);
}

void ATPSCharacter::ConversationClose()
{
	SetMoveLimit(false);
	SetOtherActionLimit(false);
	m_WidgetManager->CloseCOnversationWidget();
}

void ATPSCharacter::AllReset()
{	
	ResetCombo();
	SetMoveLimit(false);
	SetOtherActionLimit(false);
	m_bInvincibility = false;
	SetWeaponCollision(false);
	SetWeaponSkillCollision(false);
	CooltimeStop();
}

void ATPSCharacter::SetGameStartInLevelBlueprint(bool bStart)
{
	m_bCameraTimelineStart = bStart;
}

void ATPSCharacter::GameStart()
{
	StartCamera();
	StartGameMode();
}

void ATPSCharacter::CameraTimelineUpdate()
{	
	m_FollowCamera->SocketOffset.Set(0.0f,0.0f,m_cCameraCurve->GetFloatValue(m_CameraTimeline.GetPlaybackPosition()));
}

void ATPSCharacter::CameraTimelineFinished()
{
	SetBeginInputModeUseBeginEvent(false);
	SetOtherActionLimit(false);
	m_WidgetManager->ShowHUD();

}

void ATPSCharacter::StartCamera()
{
	SetBeginInputModeUseBeginEvent(true);
	SetOtherActionLimit(true);
	m_CameraTimeline.Play();
}

void ATPSCharacter::StartGameMode()
{
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
}

void ATPSCharacter::SetSkill()
{
	FS_Skill sNoneSkill{};
	FS_Skill* pSetSkill{};
	FString strItemDataPath = TEXT("DataTable'/Game/Game/System/Skill/DataTable/DT_SkillDataTable.DT_SkillDataTable'");
	m_DTdataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, *strItemDataPath));

	for (int nIndex = 0; nIndex < 3; nIndex++)
	{
		if (m_DTdataTable)
			pSetSkill = m_DTdataTable->FindRow<FS_Skill>(m_pWeapon->DT_SkillRow[nIndex], FString(""));

		if (m_pWeapon && pSetSkill)
			m_SkillSystem->SetSkill(nIndex, *pSetSkill);
		else
			m_SkillSystem->SetSkill(nIndex, sNoneSkill);
	}
}

bool ATPSCharacter::UseSkill(int nKeyboardIndex, float& fDamage)
{
	if (!m_pWeapon)
		return false;

	m_nSkillIndex = nKeyboardIndex;
	m_SkillSystem->GetSkillInfo(m_nSkillIndex,m_sSkillInfo);
	if (m_SkillSystem->GetSkillReady(m_nSkillIndex) && !m_bOtherActionLimit && m_CharacterState->UseStaminaIfCan(m_sSkillInfo.fUseStaminaAmount))
	{
		m_SkillSystem->UseSkill(m_nSkillIndex);
		fDamage = m_sSkillInfo.fDamage;
		return true;
	}

	return false;
}

void ATPSCharacter::AddItemFromDataTable(FName tItemDataTableRow)
{
	FString strItemDataPath = TEXT("DataTable'/Game/Game/System/Inven/DataTable/DT_ItemDataTable.DT_ItemDataTable'");
	m_DTdataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, *strItemDataPath));

	FS_ItemSlot* sFindItemSlot{};

	if (m_DTdataTable)
		sFindItemSlot = (m_DTdataTable->FindRow<FS_ItemSlot>(tItemDataTableRow, TEXT("")));

	if (sFindItemSlot)
		m_Inventory->AddItem(*sFindItemSlot);
}

void ATPSCharacter::AddMoney(int nMoney)
{
	m_Inventory->AddMoney(nMoney);
}

void ATPSCharacter::UseItemFunction(int nKeyboardIndex)
{
	FS_UseItemDataTable sUseItemInfo{};
	m_Inventory->UseItemToQuickSlot(nKeyboardIndex-1, sUseItemInfo);

	if (sUseItemInfo.pVerpParticle)
		UGameplayStatics::SpawnEmitterAtLocation(this, sUseItemInfo.pVerpParticle, GetMesh()->GetBoneLocation(TEXT("spine_02")));
	
	switch (sUseItemInfo.eType)
	{
	case ERecoveryType::Recovery:
		m_CharacterState->Recovery(sUseItemInfo.eRecoveryVerpState, sUseItemInfo.fRecoveryVerpAmount);
		m_Inventory->DecrementItemAmount(nKeyboardIndex - 1);
		break;
	case ERecoveryType::Verp:
		break;
	case ERecoveryType::PermanentUp:
		break;
	default:
		break;
	}
}

void ATPSCharacter::OpenInteractionUI(EInteractionType eInteractionType)
{
	m_WidgetManager->SetInteractionType(eInteractionType);
	m_WidgetManager->OpenCloseUI(EUIType::InteractionUI,false,false);
}

void ATPSCharacter::CloseInteractionUI()
{
	m_WidgetManager->CloseUIEnforcement(EUIType::InteractionUI, false, false);
}

void ATPSCharacter::Attack(EAttackType eAttackType)
{

	m_eAttackType = eAttackType;
	if (m_bIsWeapon && m_bIsAttacking)
		m_bSaveAttack = true;
	else if(m_bIsWeapon)
	{
		m_bIsAttacking = true;
		AttackComboTree();
	}

}

void ATPSCharacter::LowAttack()
{
	if (!m_bOtherActionLimit)
		Attack(EAttackType::LowAttack);
}

void ATPSCharacter::HighAttack()
{
	if (!m_bOtherActionLimit)
		Attack(EAttackType::HighAttack);
}

void ATPSCharacter::ResetCombo()
{
	SetResetCombo();
}

void ATPSCharacter::OffWeaponCollision()
{
	SetWeaponCollision(false);
}

void ATPSCharacter::ComboAttackSave()
{
	if (m_bSaveAttack)
	{
		m_bSaveAttack = false;
		AttackComboTree();
	}
}

void ATPSCharacter::AttackComboTree()
{
	m_pWeapon->SetMotionMagnification(m_arAttackMotionMagnification[m_nAttackCount +  (uint8)m_eAttackType]);
	m_pAnimInstance->PlayAttackMontage(m_arMtAttackMotion[m_nAttackCount + (uint8)m_eAttackType], m_arAttackPlayRate[m_nAttackCount + (uint8)m_eAttackType]);
	m_nAttackCount++;
	m_nAttackCount = m_nAttackCount % 3;
}

void ATPSCharacter::UseSkillFunction(int nKeyboardIndex)
{
	if (UseSkill(nKeyboardIndex-1, m_fSkillDamage))
	{
		m_pSkillAnimMontage = m_sSkillInfo.mtMotion;
		m_pWeapon->SetSkillDamage(m_fSkillDamage);
		m_pAnimInstance->OnCompletedBinding.AddDynamic(this, &ATPSCharacter::AllReset);
		m_pAnimInstance->OnInterruptedBinding.AddDynamic(this, &ATPSCharacter::AllReset);
		m_pAnimInstance->PlayMontage(m_pSkillAnimMontage, 1.0f);
	}
}

void ATPSCharacter::MakeRangeDamage(FVector vMakeLocation)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> eObjectType;
	eObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_EngineTraceChannel1));
	eObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	TArray<AActor*> arNullActor;
	FHitResult sHitResult{};
	UKismetSystemLibrary::SphereTraceSingleForObjects(this, GetActorLocation(), GetActorLocation(), 150.0f, eObjectType, false, arNullActor, EDrawDebugTrace::None, sHitResult, true, FLinearColor::Red, FLinearColor::Green, 0.5f);
	
	if (Cast<IEnemyInterface>(sHitResult.GetActor()))
		IEnemyInterface::Execute_TakeDamageCaculate(sHitResult.GetActor(), m_pWeapon->GetSkillDamageCaculate(), TEXT(""), sHitResult.Location, UKismetMathLibrary::MakeRotFromX(sHitResult.Normal));

}

void ATPSCharacter::TimelineUpdate()
{
	UpdateCooltime(m_cCurve->GetFloatValue(m_timelineCoolTime.GetPlaybackPosition()));
}

void ATPSCharacter::TimelineFinished()
{
	FinishedCooltime();
	if (EndCoolTimeEventBinding.IsBound())
		EndCoolTimeEventBinding.Broadcast();
	EndCoolTimeEventBinding.Clear();
}

void ATPSCharacter::Cooltime(float fCoolTime, FText tMessage)
{
	m_WidgetManager->ShowCooltimeLine(fCoolTime, tMessage);
	SetOtherActionLimit(true);
	m_timelineCoolTime.SetPlayRate(1.0f / fCoolTime);
	m_timelineCoolTime.PlayFromStart();
}

void ATPSCharacter::FinishedCooltime()
{
	m_WidgetManager->CloseCooltimeLine();
	SetInputMode(false,false);
	SetOtherActionLimit(false);
}

void ATPSCharacter::UpdateCooltime(float fCoolTime)
{
	m_WidgetManager->UpdateCooltime(fCoolTime);
}

void ATPSCharacter::CooltimeStop()
{
	m_timelineCoolTime.Stop();
	EndCoolTimeEventBinding.Clear();
	m_WidgetManager->CloseCooltimeLine();
}

void ATPSCharacter::Interaction()
{
	if (InteractionEventBinding.IsBound())
		InteractionEventBinding.Broadcast();
}

void ATPSCharacter::SkillPressQ()
{
	UseSkillFunction(1);
}

void ATPSCharacter::SkillPressE()
{
	UseSkillFunction(2);
}

void ATPSCharacter::SkillPressR()
{
	UseSkillFunction(3);
}

void ATPSCharacter::OpenInventory()
{
	m_WidgetManager->OpenCloseInterface(EInterfaceType::Inventory,false, true);
}

void ATPSCharacter::OpenCharacterState()
{
	m_WidgetManager->OpenCloseInterface(EInterfaceType::PlayerStateInterface, false, true);
}

void ATPSCharacter::UseFirstItem()
{
	UseItemFunction(1);
}

void ATPSCharacter::UseSecondItem()
{
	UseItemFunction(2);
}

void ATPSCharacter::UseThirdItem()
{
	UseItemFunction(3);
}
