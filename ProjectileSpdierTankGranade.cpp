// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileSpdierTankGranade.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundWave.h"
#include "CharacterInterface.h"


// Sets default values
AProjectileSpdierTankGranade::AProjectileSpdierTankGranade():
	m_ProjectileMovementComponent{},
	m_SphereComponent{},
	m_StaticMeshComponent{},
	m_ParticleSystemComponent{},
	m_fDamage{},
	m_fExplosionScale{},
	m_fExplosionDamage{},
	m_classRangeDamage{},
	m_pExplosionParticle{},
	m_pExplosionSoundWave{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProijectileMovement"));
	m_ProjectileMovementComponent->InitialSpeed = 1000.0f;
	m_ProjectileMovementComponent->MaxSpeed = 1000.0f;
	m_ProjectileMovementComponent->bRotationFollowsVelocity = true;

	m_SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = m_SphereComponent;
	m_SphereComponent->OnComponentHit.AddDynamic(this, &AProjectileSpdierTankGranade::OnComponentHit);

	m_StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	m_StaticMeshComponent->SetupAttachment(reinterpret_cast<USceneComponent*>(m_SphereComponent));


	m_ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("GranadeTrail"));
	m_ParticleSystemComponent->SetupAttachment(reinterpret_cast<USceneComponent*>(m_SphereComponent));

}

void AProjectileSpdierTankGranade::Init(float fDamage, float fExplosionScale, float fExplosionDamage)
{
	m_fDamage = fDamage;
	m_fExplosionScale = fExplosionScale;
	m_fExplosionDamage = fExplosionDamage;
}

void AProjectileSpdierTankGranade::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Cast<ICharacterInterface>(OtherActor))
		ICharacterInterface::Execute_CharacterTakeDamage(OtherActor, m_fDamage, false);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_pExplosionParticle,Hit.Location, UKismetMathLibrary::MakeRotFromX(Hit.ImpactNormal));
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), m_pExplosionSoundWave,Hit.Location);
	GetWorld()->SpawnActor<AEnemyRangeDamage>(m_classRangeDamage, Hit.Location, UKismetMathLibrary::MakeRotFromX(Hit.ImpactNormal))->Init(m_fExplosionDamage, m_fExplosionScale,true);
	
	Destroy();
}

// Called when the game starts or when spawned
void AProjectileSpdierTankGranade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileSpdierTankGranade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

