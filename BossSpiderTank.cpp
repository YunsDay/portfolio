// Fill out your copyright notice in the Description page of Project Settings.


#include "BossSpiderTank.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "BossAIController.h"
#include "Components/BoxComponent.h"
#include "CharacterInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "TPSCharacter.h"

// Sets default values
ABossSpiderTank::ABossSpiderTank():
	m_pBeAttackedParticle{},
	m_fBeAttackedParticleScale{},
	m_bAwake{},
	m_bIsActive{},
	m_bIsDeath{},
	m_fMaxHp{},
	m_fCurrentHp{},
	m_pPlayer{},
	m_vSpawnCannonLocation{},
	m_eStateType{},
	m_mtAttackMotion{},
	m_bFlipFlop{},
	m_rTargetRotate{},
	m_BoxComponent{},
	m_BlockPawnBox{},
	m_classGranade{},
	m_classCannonBullet{},
	m_classRangeDamage{},
	m_fRushAttackDamage{},
	m_fEarthQuakeDamage{},
	m_fGranadeDamage{},
	m_fGranadeExplosionDamage{},
	m_fCannonDamage{},
	m_fWhirlWindDamage{},
	m_cCurve{},
	m_cWhirlWindCurve{},
	m_rCurrentRotate{},
	m_rWhirlWindRotate{},
	m_nAttackMotionMaxIndex{},
	m_nLongAttackMotionMaxIndex{},
	m_classLazer{}
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ABossAIController::StaticClass();

	m_BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	m_BoxComponent->SetupAttachment(reinterpret_cast<USceneComponent*>(GetMesh()));
	m_BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABossSpiderTank::OnOverlapBegin);

	m_BlockPawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockBoxCollision"));
	m_BlockPawnBox->SetupAttachment(reinterpret_cast<USceneComponent*>(GetMesh()));
	m_BlockPawnBox->AttachTo(GetMesh(),TEXT("main_body"));

	
}

// Called when the game starts or when spawned
void ABossSpiderTank::BeginPlay()
{
	Super::BeginPlay();

	

	m_fCurrentHp = m_fMaxHp;
	m_eStateType = EEnemyStateType::None;

	if (m_cCurve)
	{
		FOnTimelineFloat TimelineCallback;
		

		TimelineCallback.BindUFunction(this, TEXT("TimelineUpdate"));
		

		m_TimelineRotation.AddInterpFloat(m_cCurve, TimelineCallback);
		
	}

	if (m_cWhirlWindCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEvent TimelineFinishedCallback;
		TimelineCallback.BindUFunction(this, TEXT("TimelineWhirlWindUpdate"));
		TimelineFinishedCallback.BindUFunction(this, TEXT("TimelineWhirlWindFinished"));

		m_TimelineWhirlWind.AddInterpFloat(m_cWhirlWindCurve, TimelineCallback);
		m_TimelineWhirlWind.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}
	
}

void ABossSpiderTank::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	DeathEventBinding.Clear();
}

// Called every frame
void ABossSpiderTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_TimelineRotation.TickTimeline(DeltaTime);
	m_TimelineWhirlWind.TickTimeline(DeltaTime);
}

// Called to bind functionality to input
void ABossSpiderTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABossSpiderTank::TakeDamageCaculate_Implementation(float fDamage, FName tHitBoneName, FVector vHitLocation, FRotator rHitRotator)
{
			
	if (m_bIsActive && m_pBeAttackedParticle)
	{
		m_fCurrentHp -= fDamage;
		UGameplayStatics::SpawnEmitterAtLocation(this, m_pBeAttackedParticle, vHitLocation, FRotator(0.0f), FVector(m_fBeAttackedParticleScale));
	}
	if (m_fCurrentHp <= 0.0f)
		Death();
}

void ABossSpiderTank::StartMachanic()
{
	Awake();

	//Delay
	FTimerHandle sWaitHandle{};
	float fWaitTime{};
	fWaitTime = 5.5f;
	GetWorld()->GetTimerManager().SetTimer(sWaitHandle, FTimerDelegate::CreateLambda([&]()
	{
		SetStateType(EEnemyStateType::Alert);

	}), fWaitTime, false);

	
}

float ABossSpiderTank::Attack()
{
	if(!m_bIsDeath)
		return PlayAnimMontage(m_mtAttackMotion[FMath::RandRange(0, m_nAttackMotionMaxIndex)]);

	return 0.5f;
}

float ABossSpiderTank::LongDistanceAttack()
{
	if (!m_bIsDeath)
		return PlayAnimMontage(m_mtLongAttackMotion[FMath::RandRange(0, m_nLongAttackMotionMaxIndex)]);

	return 0.5f;
}

void ABossSpiderTank::FireCannon()
{

	if (m_bFlipFlop)
	{
		m_vSpawnCannonLocation = GetMesh()->GetSocketLocation(TEXT("cannonSocket"));
		m_bFlipFlop = false;
	}
	else
	{
		m_vSpawnCannonLocation = GetMesh()->GetSocketLocation(TEXT("cannonSocket2"));
		m_bFlipFlop = true;
	}
	FRotator rRotator;
	rRotator = UKismetMathLibrary::MakeRotFromX(GetMesh()->GetForwardVector());
	rRotator.Pitch = UKismetMathLibrary::FindLookAtRotation(m_vSpawnCannonLocation , m_pPlayer->GetActorLocation()).Pitch;
	rRotator.Yaw += 90;
	GetWorld()->SpawnActor<AProjectileSpiderTankCannon>(m_classCannonBullet, m_vSpawnCannonLocation, rRotator)->Init(m_fCannonDamage);
}

void ABossSpiderTank::FireGranade()
{
	FRotator rRotator{};

	
	rRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), m_pPlayer->GetActorLocation());

	rRotator.Roll = FMath::FRandRange(50.0f, 70.0f);
	rRotator.Pitch = FMath::FRandRange(50.0f, 70.0f);
	GetWorld()->SpawnActor<AProjectileSpdierTankGranade>(m_classGranade,GetMesh()->GetSocketLocation(TEXT("lGranadeLauncher")), rRotator)->Init(m_fGranadeDamage, 2.0f ,m_fGranadeExplosionDamage);
}

void ABossSpiderTank::RushAttack()
{
	m_BoxComponent->SetGenerateOverlapEvents(true);
	m_BlockPawnBox->SetGenerateOverlapEvents(false);
	
}

void ABossSpiderTank::RushEnd()
{
	m_BoxComponent->SetGenerateOverlapEvents(false);
	m_BlockPawnBox->SetGenerateOverlapEvents(true);
}

void ABossSpiderTank::EarthQuake()
{
	GetWorld()->SpawnActor<AEnemyRangeDamage>(m_classRangeDamage,GetMesh()->GetSocketLocation(TEXT("EarthQuakeLocation")),UKismetMathLibrary::MakeRotFromX(GetMesh()->GetSocketLocation(TEXT("EarthQuakeLocation"))))->Init(m_fEarthQuakeDamage,6.0f, true);
}

void ABossSpiderTank::WhirlWind()
{
	m_rCurrentRotate = GetActorRotation();
	m_rWhirlWindRotate = GetActorRotation();
	m_rWhirlWindRotate.Yaw += 400.0f;
	m_TimelineWhirlWind.PlayFromStart();
}

void ABossSpiderTank::WhirlWindJudge()
{
	GetWorld()->SpawnActor<AEnemyRangeDamage>(m_classRangeDamage, GetMesh()->GetSocketLocation(TEXT("EarthQuakeLocation")), UKismetMathLibrary::MakeRotFromX(GetRootComponent()->GetComponentLocation()))->Init(m_fWhirlWindDamage, 5.0f, true);
}

void ABossSpiderTank::LazerAttack()
{
	FRotator rRotator;
	rRotator = UKismetMathLibrary::MakeRotFromX(GetMesh()->GetForwardVector());
	rRotator.Pitch = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetSocketLocation(TEXT("cannonSocket2")),m_pPlayer->GetActorLocation()).Pitch;
	rRotator.Yaw += 90;

	GetWorld()->SpawnActor<AProjectileSpdierTankGranade>(m_classLazer, GetMesh()->GetSocketLocation(TEXT("cannonSocket2")), rRotator);
	GetWorld()->SpawnActor<AProjectileSpdierTankGranade>(m_classLazer, GetMesh()->GetSocketLocation(TEXT("cannonSocket")), rRotator);
}

void ABossSpiderTank::Death()
{
	GetMesh()->SetGenerateOverlapEvents(false);
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("LightParam"), 0.0f);

	FTimerHandle sWaitHandle{};
	float fWaitTime{};
	fWaitTime = PlayAnimMontage(m_mtDeathMotion);
	m_bIsDeath = true;

	GetWorld()->GetTimerManager().SetTimer(sWaitHandle, FTimerDelegate::CreateLambda([&]()
	{	
		if (DeathEventBinding.IsBound())
			DeathEventBinding.Broadcast();

	}), fWaitTime, false);
	

	m_eStateType = EEnemyStateType::Death;
}

void ABossSpiderTank::Rotate()
{
	SetActorRotation(FRotator(GetActorRotation().Pitch, (int)GetActorRotation().Yaw % 360, GetActorRotation().Roll));
	m_rCurrentRotate = GetActorRotation();
	m_rTargetRotate = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),m_pPlayer->GetActorLocation());
	m_TimelineRotation.PlayFromStart();
}

float ABossSpiderTank::CaculateSpawnCannonRotationYaw()
{
	FRotator rRotation;
	float fReturnValue;
	rRotation = UKismetMathLibrary::MakeRotFromX(GetMesh()->GetForwardVector());
	
	fReturnValue = rRotation.Yaw + 90.0f + FMath::FRandRange(-60.0f, 60.0f);

	return fReturnValue; 

}

void ABossSpiderTank::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ICharacterInterface>(OtherActor))
		ICharacterInterface::Execute_CharacterTakeDamage(OtherActor, m_fRushAttackDamage, true);
}

void ABossSpiderTank::Awake()
{
	m_bAwake = true;
	Cast<ABossAIController>(GetController())->RunMachanic();
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("LightParam"),1682.0f);
	m_bIsActive = true;

}

void ABossSpiderTank::SetPlayer(UPARAM(ref) ATPSCharacter* pPlayerRef)
{
	m_pPlayer = pPlayerRef;
}

void ABossSpiderTank::TimelineUpdate()
{
	SetActorRotation(FRotator(0.0f, UKismetMathLibrary::RLerp(m_rCurrentRotate, m_rTargetRotate, m_cCurve->GetFloatValue(m_TimelineRotation.GetPlaybackPosition()),false).Yaw, 0.0f));
}

void ABossSpiderTank::TimelineWhirlWindUpdate()
{
	SetActorRotation(UKismetMathLibrary::RLerp(m_rCurrentRotate,m_rWhirlWindRotate,m_cWhirlWindCurve->GetFloatValue(m_TimelineWhirlWind.GetPlaybackPosition()),false));

}

void ABossSpiderTank::TimelineWhirlWindFinished()
{
	SetActorRotation(FRotator(GetActorRotation().Pitch, (int)GetActorRotation().Yaw % 360, GetActorRotation().Roll));
}

void ABossSpiderTank::SetStateType(EEnemyStateType eStateType)
{
	m_eStateType = eStateType;
}
