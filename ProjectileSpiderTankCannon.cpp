// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileSpiderTankCannon.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "CharacterInterface.h"

// Sets default values
AProjectileSpiderTankCannon::AProjectileSpiderTankCannon():
	m_ProjectileMovementComponent{},
	m_SphereComponent{},
	m_StaticMeshComponent{},
	m_fDamage{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProijectileMovement"));
	m_ProjectileMovementComponent->InitialSpeed = 1000.0f;
	m_ProjectileMovementComponent->MaxSpeed = 1000.0f;
	m_ProjectileMovementComponent->bRotationFollowsVelocity = true;

	m_SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = m_SphereComponent;
	m_SphereComponent->OnComponentHit.AddDynamic(this, &AProjectileSpiderTankCannon::OnComponentHit);

	m_StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	m_StaticMeshComponent->SetupAttachment(reinterpret_cast<USceneComponent*>(m_SphereComponent));

}

void AProjectileSpiderTankCannon::Init(float fDamage)
{
	m_fDamage = fDamage;
}

void AProjectileSpiderTankCannon::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Cast<ICharacterInterface>(OtherActor))
		ICharacterInterface::Execute_CharacterTakeDamage(OtherActor, m_fDamage, false);
	Destroy();
}

// Called when the game starts or when spawned
void AProjectileSpiderTankCannon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileSpiderTankCannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

