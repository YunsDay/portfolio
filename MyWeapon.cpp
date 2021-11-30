// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "TPSCharacter.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EnemyInterface.h"
#include "Engine/EngineTypes.h"

// Sets default values
AMyWeapon::AMyWeapon():
	m_pStaticMesh{},
	m_pOwnerRef{},
	m_fDamage{},
	m_fSkillDamage{},
	m_fCriticalDamage{},
	m_fCriticalChancePercent{},
	m_fMotionMagnification{},
	DT_SkillRow{},
	m_eWeaponType{},
	m_tWeaponAttackMotionRow{},
	m_AttackCapsuleComponent{},
	m_SkillCapsuleComponent{},
	m_CircleTracePoint{},
	m_DefaultSceneComponent{},
	m_sCircleTraceHitResult{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	m_DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultScene"));
	RootComponent = m_DefaultSceneComponent;

	m_pStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	m_pStaticMesh->SetupAttachment(reinterpret_cast<USceneComponent*>(m_DefaultSceneComponent));


	m_CircleTracePoint = CreateDefaultSubobject<USceneComponent>(TEXT("CircleTracePoint"));
	m_CircleTracePoint->SetupAttachment(reinterpret_cast<USceneComponent*>(m_pStaticMesh));

	m_AttackCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackCollision"));
	m_AttackCapsuleComponent->SetupAttachment(reinterpret_cast<USceneComponent*>(m_DefaultSceneComponent));
	m_AttackCapsuleComponent->SetGenerateOverlapEvents(false);
	m_AttackCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AMyWeapon::OnOverlapBegin);


	m_SkillCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SkillCollision"));
	m_SkillCapsuleComponent->SetupAttachment(reinterpret_cast<USceneComponent*>(m_DefaultSceneComponent));
	m_SkillCapsuleComponent->SetGenerateOverlapEvents(false);
	m_SkillCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AMyWeapon::OnOverlapSkillBegin);

}

// Called when the game starts or when spawned
void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AMyWeapon::GetFinalDamageCaculate()
{
	float fDamage{};
	float fCriticalDamage{};
	float fCriticalChance{};

	m_pOwnerRef->GetDamageCaculate(fDamage, fCriticalDamage, fCriticalChance);

	if (FMath::FRandRange(0.0f, 100.0f) < fCriticalChance)
		return (fDamage * m_fMotionMagnification) * (1 + (fCriticalDamage / 100.0f));
	else
		return fDamage * m_fMotionMagnification;
}

void AMyWeapon::SetGenerateOverlapEvent(bool bOnOff)
{
	m_AttackCapsuleComponent->SetGenerateOverlapEvents(bOnOff);
}

void AMyWeapon::GetWeaponState(float& pDamage, float& pCriticalDamage, float& pCriticalChancePercent)
{
	pDamage = m_fDamage;
	pCriticalDamage = m_fCriticalDamage;
	pCriticalChancePercent = m_fCriticalChancePercent;
}

void AMyWeapon::SetSkillDamage(float fSkillDamage)
{
	m_fSkillDamage = fSkillDamage;
}

void AMyWeapon::SetMotionMagnification(float fMotionMagnification)
{
	m_fMotionMagnification = fMotionMagnification;
}

void AMyWeapon::SetGenerateSkill(bool bOnOff)
{
	m_SkillCapsuleComponent->SetGenerateOverlapEvents(bOnOff);
}

float AMyWeapon::GetSkillDamageCaculate()
{
	float fDamage{};
	float fCriticalDamage{};
	float fCriticalChance{};

	m_pOwnerRef->GetDamageCaculate(fDamage, fCriticalDamage, fCriticalChance);

	if (FMath::FRandRange(0.0f, 100.0f) < fCriticalChance)
		return (fDamage + m_fSkillDamage) * (1 + (fCriticalDamage / 100.0f));
	else
		return fDamage + m_fSkillDamage;

}

FName AMyWeapon::GetSkillRow(int nIndex)
{
	return DT_SkillRow[nIndex];
}

void AMyWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	GiveDamageToEnemy(OtherActor, GetFinalDamageCaculate());

}

void AMyWeapon::OnOverlapSkillBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GiveDamageToEnemy(OtherActor, GetSkillDamageCaculate());
}

void AMyWeapon::CircleTraceOnHit()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> eObjectType;
	eObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_EngineTraceChannel1));
	eObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	TArray<AActor*> arNullActor;

	UKismetSystemLibrary::SphereTraceSingleForObjects(this, m_CircleTracePoint->GetComponentLocation(), m_CircleTracePoint->GetComponentLocation(), 100.0f, eObjectType, false, arNullActor, EDrawDebugTrace::None, m_sCircleTraceHitResult, true, FLinearColor::Red, FLinearColor::Green, 1.0f);
	
}

void AMyWeapon::GiveDamageToEnemy(AActor* OtherActor, float fDamage)
{
	CircleTraceOnHit();
	if (Cast<IEnemyInterface>(OtherActor))
		IEnemyInterface::Execute_TakeDamageCaculate(OtherActor, fDamage, m_sCircleTraceHitResult.BoneName, m_sCircleTraceHitResult.Location, UKismetMathLibrary::MakeRotFromX(m_sCircleTraceHitResult.Normal));
}
