// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NetGameInstance.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Components/TextRenderComponent.h"
#include "Net/UnrealNetwork.h"
#include "NetBaseCharacter.generated.h"

UENUM(BlueprintType)
enum class EBodyPart : uint8
{
    BP_Face = 0,
    BP_Hair = 1,
    BP_Chest = 2,
    BP_Hands = 3,
    BP_Legs = 4,
    BP_Beard = 5,
    BP_EyeBrows = 6,
    BP_BodyType = 7,
    BP_COUNT = 8,
};

USTRUCT(BlueprintType)
struct FSMeshAssetList : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<USkeletalMesh*> ListSkeletal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UStaticMesh*> ListStatic;
};


UCLASS()
class ANetBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ANetBaseCharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintPure)
    FString GetCustomizationData();
    void ParseCustomizationData(FString Data);

    UFUNCTION(BlueprintCallable)
    void ChangeBodyPart(EBodyPart index, int value, bool DirectSet);

    //UFUNCTION(BlueprintCallable)

    UFUNCTION(Server, Reliable)
    void SubmitPlayerInfoToServer(FSPlayerInfo Info);

    UFUNCTION(BlueprintImplementableEvent)
    void OnPlayerInfoChanged();

    //Timer 
    UFUNCTION()
    void CheckPlayerState();
    UFUNCTION()
    void CheckPlayerInfo();
    UPROPERTY()
    FText PartNickName;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UTextRenderComponent* TextRenderer;
    UPROPERTY()
    USkeletalMeshComponent* PartFace;
    UPROPERTY()
    UStaticMeshComponent* PartHair;
    UPROPERTY()
    UStaticMeshComponent* PartBeard;
    UPROPERTY()
    UStaticMeshComponent* PartEyeBrows;
    UPROPERTY()
    UStaticMeshComponent* PartEyes;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USkeletalMeshComponent* PartHands;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USkeletalMeshComponent* PartLegs;
    UPROPERTY(BlueprintReadWrite)
    FString IpAdress;
    UPROPERTY(BlueprintReadWrite)
    FString PlayerNickName;
    bool PlayerInfoReceived;

private:

    int BodyPartIndices[(int)EBodyPart::BP_COUNT];

    void UpdateBodyParts();

    static FSMeshAssetList* GetBodyPartList(EBodyPart part, bool isFemale);

    FTimerHandle ClientDataCheckTimer;
};