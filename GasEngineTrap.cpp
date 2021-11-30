// Fill out your copyright notice in the Description page of Project Settings.


#include "GasEngineTrap.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AGasEngineTrap::AGasEngineTrap():
	m_GasEngineStaticMesh{},
	m_FirstPartStaticMesh{},
	m_SecondPartStaticMesh{},
	m_ThirdPartStaticMesh{},
	m_FourthPartStaticMesh{},
	m_ParticleSystem{},
	m_fFirstPartSpeed{},
	m_fSecondPartSpeed{},
	m_fThirdPartSpeed{},
	m_fFourthPartSpeed{},
	m_bShotDown{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_GasEngineStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GasEngineStaticMesh"));
	RootComponent = m_GasEngineStaticMesh;

	m_FirstPartStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FirstPart"));
	m_FirstPartStaticMesh->SetupAttachment(reinterpret_cast<USceneComponent*>(m_GasEngineStaticMesh));
		
	m_SecondPartStaticMesh= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondPart"));
	m_SecondPartStaticMesh->SetupAttachment(reinterpret_cast<USceneComponent*>(m_GasEngineStaticMesh));
	
	m_ThirdPartStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ThirdPart"));
	m_ThirdPartStaticMesh->SetupAttachment(reinterpret_cast<USceneComponent*>(m_GasEngineStaticMesh));
	
	m_FourthPartStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FourthPart"));
	m_FourthPartStaticMesh->SetupAttachment(reinterpret_cast<USceneComponent*>(m_GasEngineStaticMesh));
	
	m_ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	m_ParticleSystem->SetupAttachment(reinterpret_cast<USceneComponent*>(m_GasEngineStaticMesh));

}

// Called when the game starts or when spawned
void AGasEngineTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGasEngineTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!m_bShotDown)
	{
		m_FirstPartStaticMesh->AddLocalRotation(UKismetMathLibrary::MakeRotator(m_fFirstPartSpeed,0.0f,0.0f));
		m_SecondPartStaticMesh->AddLocalRotation(UKismetMathLibrary::MakeRotator(m_fSecondPartSpeed, 0.0f, 0.0f));
		m_ThirdPartStaticMesh->AddLocalRotation(UKismetMathLibrary::MakeRotator(m_fThirdPartSpeed, 0.0f, 0.0f));
		m_FourthPartStaticMesh->AddLocalRotation(UKismetMathLibrary::MakeRotator(m_fFourthPartSpeed, 0.0f, 0.0f));
	}
}

void AGasEngineTrap::ShotDownEngine()
{
	m_bShotDown = true;
	m_ParticleSystem->SetTemplate(nullptr);
}

void AGasEngineTrap::SetRotatorSpeed(float fRotatorSpeedOne, float fRotatorSpeedTwo, float fRotatorSpeedThree, float fRotatorSpeedFour, float fParticleTime)
{
	m_fFirstPartSpeed = fRotatorSpeedOne;
	m_fSecondPartSpeed = fRotatorSpeedTwo;
	m_fThirdPartSpeed = fRotatorSpeedThree;
	m_fFourthPartSpeed = fRotatorSpeedFour;
	m_ParticleSystem->CustomTimeDilation = fParticleTime;
}

