// Fill out your copyright notice in the Description page of Project Settings.


#include "COABaseChar.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACOABaseChar::ACOABaseChar():
	bDead(false),
	Health(100.0f),
	HealingRate(0.0f),
	WalkSpeed(300.0f)

{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

}

// Called when the game starts or when spawned
void ACOABaseChar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACOABaseChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bDead)
	{
		Health = FMath::Min(MaxHealth, Health + HealingRate + DeltaTime);
	}

}



