// Fill out your copyright notice in the Description page of Project Settings.


#include "WarpGate.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "TPSCharacter.h"
#include "InteractionType.h"
#include "WarpLocation.h"

// Sets default values
AWarpGate::AWarpGate():
	m_ParticleSystem{},
	m_CapsuleComponent{},
	m_bWarpActivate{},
	m_pPlayer{},
	m_pWarpLocation{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	m_CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	m_CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AWarpGate::OnOverlapBegin);
	m_CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &AWarpGate::OnOverlapEnd);
	RootComponent = m_CapsuleComponent;

	m_ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));


	m_bWarpActivate = true;

}

// Called when the game starts or when spawned
void AWarpGate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWarpGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWarpGate::SetWarpActivate(bool bIsActive)
{
	m_CapsuleComponent->SetGenerateOverlapEvents(bIsActive);
}

void AWarpGate::Warp()
{
	m_pPlayer->SetActorRotation(m_pWarpLocation->GetRotation());
	m_pPlayer->SetFollowCameraRotation(m_pWarpLocation->GetLocation());
	m_pPlayer->SetActorLocation(m_pWarpLocation->GetLocation(),false,nullptr,ETeleportType::TeleportPhysics);
}

void AWarpGate::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	m_pPlayer = Cast<ATPSCharacter>(OtherActor);
	if (m_pPlayer)
	{
		m_pPlayer->OpenInteractionUI(EInteractionType::Move);
		m_pPlayer->InteractionEventBinding.AddDynamic(this, &AWarpGate::Warp);
	}
}

void AWarpGate::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	m_pPlayer->CloseInteractionUI();
	m_pPlayer->InteractionEventBinding.Remove(this, TEXT("Warp"));
	m_pPlayer = nullptr;
}

