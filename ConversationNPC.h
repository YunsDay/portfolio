// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicNPC.h"
#include "ConversationNPC.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API AConversationNPC : public ABasicNPC
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AConversationNPC();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FText> m_arConversationText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_nConversationTextMaxIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATPSCharacter* m_pPlayer;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UBoxComponent* m_BoxComponent;

public:
	UFUNCTION()
		void ConversationStart();
	UFUNCTION()
		void ConversationEnd();

private:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
