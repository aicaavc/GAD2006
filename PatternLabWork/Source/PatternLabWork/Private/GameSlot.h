// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "UnitBase.h"
#include "GameSlot.generated.h"

USTRUCT(Blueprintable)
struct FSGridPosition
{
	GENERATED_USTRUCT_BODY();

	FSGridPosition() {}
	FSGridPosition(int col, int row) : Col(col), Row(row) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Col;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Row;
};

UENUM(Blueprintable)
enum EGridState
{
	GS_Default,
	GS_Highlighted,
	GS_Offensive,
	GS_Supportive,
};

UCLASS()
class AGameSlot : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameSlot();

	UPROPERTY(EditAnywhere)
	UBoxComponent* Box;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Plane;

	FSGridPosition GridPosition;

	EGridState GridState;

	UPROPERTY(VisibleAnywhere)
	AUnitBase* Unit;

	void SpawnUnitHere(TSubclassOf<AUnitBase>& UnitClass);
	void OnGridClicked(AActor* TouchedActor, FKey ButtonPressed);

	UFUNCTION()
	void SetState(EGridState NewState);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
