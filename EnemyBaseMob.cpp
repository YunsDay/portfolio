// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseMob.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MobSpawner.h"
#include "Components/SphereComponent.h"
#include "TPSCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AEnemyBaseMob::AEnemyBaseMob() :
	m_SphereComponent{},
	m_eState{},
	m_bIsDeath{},
	m_fMaxHp{},
	m_fCurrentHp{},
	m_fLimitDistance{},
	m_vSpawnPoint{},
	m_pPlayer{},
	m_rTargetRotate{},
	m_TimelineRotation{},
	m_cCurve{},
	m_nDeathPoseRandomIndex{},
	m_nMaxDeathPoseIndex{},
	m_animAttack{},
	m_pSpawner{},
	m_pBeAttackedParticle{},
	m_fBeAttackedParticleScale{},
	m_arRandomGiveItem{},
	m_nGiveItemArraySize{}
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ScaneCollision"));
	m_SphereComponent->SetupAttachment(reinterpret_cast<USceneComponent*>(GetMesh()));
	

}

void AEnemyBaseMob::TakeDamageCaculate_Implementation(float fDamage, FName tHitBoneName, FVector vHitLocation, FRotator rHitRotator)
{

	if (m_pBeAttackedParticle)
		UGameplayStatics::SpawnEmitterAtLocation(this, m_pBeAttackedParticle , vHitLocation , FRotator::ZeroRotator,FVector(m_fBeAttackedParticleScale));
		

	if (!m_bIsDeath)
		m_fCurrentHp -= fDamage;
	if (m_fCurrentHp <= 0)
		Death();
}

// Called when the game starts or when spawned
void AEnemyBaseMob::BeginPlay()
{
	Super::BeginPlay();
	m_SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBaseMob::OnOverlapBegin);
	m_SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AEnemyBaseMob::OnOverlapEnd);
	m_fCurrentHp = m_fMaxHp;
	m_vSpawnPoint = GetCharacterMovement()->GetActorLocation();
	m_eState = EEnemyStateType::Idle;

	if (m_cCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEvent TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, TEXT("TimelineUpdate"));
		TimelineFinishedCallback.BindUFunction(this, TEXT("TimelineFinished"));

		m_TimelineRotation.AddInterpFloat(m_cCurve, TimelineCallback);
		m_TimelineRotation.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}
}

void AEnemyBaseMob::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	m_pSpawner = nullptr;
	m_pPlayer = nullptr;
	DeathEventBinding.Clear();
}

// Called every frame
void AEnemyBaseMob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_TimelineRotation.TickTimeline(DeltaTime);
	ReturnSpawnPointCaculate();
}

// Called to bind functionality to input
void AEnemyBaseMob::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyBaseMob::InitSpawn(float fHp, float fLimitDistance, class AMobSpawner* pSpawner, FVector vSpawnPoint)
{
	m_fMaxHp = fHp;
	m_fCurrentHp = fHp;
	m_fLimitDistance = fLimitDistance;
	m_pSpawner = pSpawner;
	m_vSpawnPoint = vSpawnPoint;
}

void AEnemyBaseMob::StartAlert()
{
	m_eState = EEnemyStateType::Alert;
}

void AEnemyBaseMob::ReturnToSpawnPoint()
{
	m_fCurrentHp = m_fMaxHp;
	//GetCharacterMovement()->StopActiveMovement();
	m_eState = EEnemyStateType::None;

}

void AEnemyBaseMob::ReturnSpawnPointCaculate()
{
	if (m_eState == EEnemyStateType::Alert && UKismetMathLibrary::Vector_Distance(m_vSpawnPoint, GetActorLocation()) > m_fLimitDistance)
		ReturnToSpawnPoint();
}

void AEnemyBaseMob::Rotate(float fPlayRate, FRotator rTargetRotate)
{
	m_TimelineRotation.SetPlayRate(fPlayRate);
	m_rTargetRotate = rTargetRotate;
	m_TimelineRotation.PlayFromStart();
}

void AEnemyBaseMob::GiveItem()
{
	int nRandomIndex{};
	nRandomIndex = FMath::RandRange(0,m_nGiveItemArraySize-1);
	if (m_pPlayer)
	{
		for (int i = 1; i <= m_arRandomGiveItem[nRandomIndex].nAmount; i++)
		{
			m_pPlayer->AddItemFromDataTable(m_arRandomGiveItem[nRandomIndex].tItemDataTableRow);
		}
	}

}

void AEnemyBaseMob::Death()
{
	m_eState = EEnemyStateType::Death;
	GetMesh()->SetGenerateOverlapEvents(false);
	m_nDeathPoseRandomIndex = FMath::RandRange(0, m_nMaxDeathPoseIndex);
	m_bIsDeath = true;
	
	GiveItem();

	if (DeathEventBinding.IsBound())
		DeathEventBinding.Broadcast();

	FTimerHandle sWaitHandle{};
	float fWaitTime{};
	fWaitTime = 3.0f;
	GetWorld()->GetTimerManager().SetTimer(sWaitHandle, FTimerDelegate::CreateLambda([&]()
	{
		Destroy();

	}), fWaitTime, false);

	
}

void AEnemyBaseMob::Attack_Implementation()
{
}

void AEnemyBaseMob::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	m_pPlayer=Cast<ATPSCharacter>(OtherActor);
	if (Cast<ATPSCharacter>(OtherActor))
		StartAlert();
}

void AEnemyBaseMob::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AEnemyBaseMob::TimelineUpdate()
{
	SetActorRotation(FRotator(0.0f, UKismetMathLibrary::RLerp(GetActorRotation(), m_rTargetRotate, m_cCurve->GetFloatValue(m_TimelineRotation.GetPlaybackPosition()), true).Yaw, 0.0f));
}

void AEnemyBaseMob::TimelineFinished()
{
}
