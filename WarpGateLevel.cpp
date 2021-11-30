// Fill out your copyright notice in the Description page of Project Settings.


#include "WarpGateLevel.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "TPSCharacter.h"
#include "InteractionType.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWarpGateLevel::AWarpGateLevel():
	m_ParticleSystem{},
	m_CapsuleComponent{},
	m_pParticleTemplate{},
	m_bWarpActivate{},
	m_tOpenLevelName{},
	m_pPlayer{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	m_CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AWarpGateLevel::OnOverlapBegin);
	m_CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &AWarpGateLevel::OnOverlapEnd);
	m_CapsuleComponent->SetGenerateOverlapEvents(false);
	RootComponent = m_CapsuleComponent;

	m_ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	m_ParticleSystem->SetRelativeLocation(FVector(0.0f,0.0f,0.0f));

}

// Called when the game starts or when spawned
void AWarpGateLevel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWarpGateLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWarpGateLevel::SetWarpActivate(bool bIsActive)
{
	if (m_pParticleTemplate)
		m_ParticleSystem->SetTemplate(m_pParticleTemplate);
	m_CapsuleComponent->SetGenerateOverlapEvents(bIsActive);
}

void AWarpGateLevel::Warp()
{
	UGameplayStatics::OpenLevel(this, m_tOpenLevelName);
}

void AWarpGateLevel::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	m_pPlayer = Cast<ATPSCharacter>(OtherActor);
	if (m_pPlayer)
	{
		m_pPlayer->OpenInteractionUI(EInteractionType::Move);
		m_pPlayer->InteractionEventBinding.AddDynamic(this, &AWarpGateLevel::Warp);
	}
}

void AWarpGateLevel::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	m_pPlayer->CloseInteractionUI();
	m_pPlayer->InteractionEventBinding.Remove(this, TEXT("Warp"));
	m_pPlayer = nullptr;
}

