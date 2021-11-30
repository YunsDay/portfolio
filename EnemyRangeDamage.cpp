// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRangeDamage.h"
#include "Components/SphereComponent.h"
#include "CharacterInterface.h"

// Sets default values
AEnemyRangeDamage::AEnemyRangeDamage() :
	m_SphereComponent{},
	m_fDamage{},
	m_fRange{},
	m_bIsPlayerKnockDown{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RangeSphere"));
	m_SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyRangeDamage::OnOverlapBegin);

}

void AEnemyRangeDamage::Init(float fDamage, float fMagnification, bool bPlayerKnockDown)
{
	m_fDamage = fDamage;
	m_fRange = fMagnification;
	m_bIsPlayerKnockDown = bPlayerKnockDown;
	this->SetActorScale3D(FVector(fMagnification, fMagnification, fMagnification));
	
	FTimerHandle sWaitHandle{};
	float fWaitTime{};
	fWaitTime = 0.5f;
	GetWorld()->GetTimerManager().SetTimer(sWaitHandle, FTimerDelegate::CreateLambda([&]()
	{
		Destroy();
	}), fWaitTime, false); 
}

void AEnemyRangeDamage::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ICharacterInterface>(OtherActor))
		ICharacterInterface::Execute_CharacterTakeDamage(OtherActor, m_fDamage ,m_bIsPlayerKnockDown);
}

// Called when the game starts or when spawned
void AEnemyRangeDamage::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AEnemyRangeDamage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

