// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "Commands/MoveCommand.h"
#include "TBPlayerController.h"

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGameManager::OnActorClicked(AActor* Actor, FKey button)
{
	//Still executing a command?
	if (CurrentCommand.IsValid() && CurrentCommand->IsExecuting())
	{
		return;
	}

	AGameSlot* Slot = Cast<AGameSlot>(Actor);

	//Clicked on a non slot actor?
	if (!Slot)
	{
		return;
	}

	//Player click?
	if (!ThePlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("No Player Unit Detected!"));
		return;
	}

	//Empty slot? Then It's a move command for the player
	if (Slot->Unit == nullptr)
	{
		//Move from player position to clicked position
		TSharedRef<MoveCommand> Cmd = MakeShared<MoveCommand>(ThePlayer->Slot->GridPosition, Slot->GridPosition);
		CommandPool.Add(Cmd);
		Cmd->Execute();
		CurrentCommand = Cmd;
	}

}

bool AGameManager::UndoLastMove()
{
	int movesLeft = CommandPool.Num();

	if (movesLeft <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Can not Revert ")));
		return false;
	}

	TSharedRef<Command> Cmd = CommandPool.Pop();
	Cmd->Revert();

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Reverting...")));

	return true;
}

void AGameManager::CreateLevelActors(FSLevelInfo& Info)
{
	ThePlayer = nullptr;

	for (auto UnitInfo : Info.Units)
	{
		if (AGameSlot* Slot = GameGrid->GetSlot(UnitInfo.StartPosition))
		{
			Slot->SpawnUnitHere(UnitInfo.UnitClass);

			if (Slot->Unit && Slot->Unit->IsControlledByThePlayer())
			{
				ThePlayer = Slot->Unit;
			}
		}
	}
}


// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	if (auto PlayerController = GWorld->GetFirstPlayerController<ATBPlayerController>())
	{
		PlayerController->GameManager = this;
	}

	if (Levels.IsValidIndex(CurrentLevel))
	{
		CreateLevelActors(Levels[CurrentLevel]);
	}
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentCommand.IsValid())
	{
		CurrentCommand->Update(DeltaTime);
	}
}

