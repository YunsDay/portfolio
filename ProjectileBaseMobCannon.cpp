// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBaseMobCannon.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "CharacterInterface.h"


// Sets default values
AProjectileBaseMobCannon::AProjectileBaseMobCannon():
	m_ProjectileMovementComponent{},
	m_SphereComponent{},
	m_StaticMeshComponent{},
	m_classRangeDamage{},
	m_fDamage{},
	m_fExplosionDamage{},
	m_fExplosionRangeMagnification{},
	m_bIsCharacterKnockDown{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProijectileMovement"));
	m_ProjectileMovementComponent->InitialSpeed = 1000.0f;
	m_ProjectileMovementComponent->MaxSpeed = 1000.0f;
	m_ProjectileMovementComponent->bRotationFollowsVelocity = true;

	m_SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = m_SphereComponent;
	m_SphereComponent->OnComponentHit.AddDynamic(this, &AProjectileBaseMobCannon::OnComponentHit);

	m_StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	m_StaticMeshComponent->SetupAttachment(reinterpret_cast<USceneComponent*>(m_SphereComponent));

}

// Called when the game starts or when spawned
void AProjectileBaseMobCannon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectileBaseMobCannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBaseMobCannon::Init(float fDamage, float fExplosionDamage, float fExplosionRangeMagnification, bool bPlayerKnockDown)
{
	m_fDamage = fDamage;
	m_fExplosionDamage = fExplosionDamage;
	m_fExplosionRangeMagnification = fExplosionRangeMagnification;
	m_bIsCharacterKnockDown = bPlayerKnockDown;
}

void AProjectileBaseMobCannon::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Cast<ICharacterInterface>(OtherActor))
		ICharacterInterface::Execute_CharacterTakeDamage(OtherActor, m_fDamage, m_bIsCharacterKnockDown);
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Cast<UParticleSystem>(StaticLoadObject(UParticleSystem::StaticClass(), NULL, TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"))), Hit.Location, UKismetMathLibrary::MakeRotFromX(Hit.ImpactNormal));
	GetWorld()->SpawnActor<AEnemyRangeDamage>(m_classRangeDamage, Hit.Location, UKismetMathLibrary::MakeRotFromX(Hit.ImpactNormal))->Init(m_fExplosionDamage,m_fExplosionRangeMagnification,false);

	Destroy();
}

