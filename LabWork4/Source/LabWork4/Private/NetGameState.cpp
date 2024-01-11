#include "NetGameState.h"
#include "NetPlayerState.h"
#include "NetGameMode.h"
#include "NetBaseCharacter.h"
#include "Net/UnrealNetwork.h"

// Constructor initializes default values for the game state

ANetGameState::ANetGameState() :
	WinningPlayer(-1),
	Timer(30.0f),
	RemainingTime(Timer)
{

}

// Specifies replicated properties for network replication
void ANetGameState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANetGameState, WinningPlayer);
	DOREPLIFETIME_CONDITION(ANetGameState, RemainingTime, COND_OwnerOnly);
}

// Called when the WinningPlayer variable is replicated to clients
void ANetGameState::OnRep_Winner()
{
	// If there is a winning player, trigger the victory event
	if (WinningPlayer >= 0)
	{
		OnVictory();
	}
}

// Implementation of the TriggerRestart function to be replicated to clients
void ANetGameState::TriggerRestart_Implementation()
{
	OnRestart();
}

// Retrieves a player state based on the given PlayerIndex
ANetPlayerState* ANetGameState::GetPlayerStateByIndex(int PlayerIndex)
{
	for (APlayerState* PS : PlayerArray)
	{
		ANetPlayerState* State = Cast<ANetPlayerState>(PS);
		// If the player state is found and matches the provided index, return it
		if (State && State->PlayerIndex == PlayerIndex)
		{
			return State;
		}
	}

	return nullptr;
}

//Debug
void ANetGameState::StartTimer()
{	
	RemainingTime = Timer;
	GetWorldTimerManager().SetTimer(TimerHandle_Timer, this, &ANetGameState::UpdateTimer, 1.0f, true);
}
// Stops the game timer (replicated to clients)
void ANetGameState::StopTimer_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Timer);
}

void ANetGameState::UpdateTimer()
{
	UpdateTimerDisplay();

	// Decrement the remaining time, and trigger TimeOver if it reaches zero

	if (RemainingTime > 0)
	{
		RemainingTime -= 1.0f;
	}
	else
	{
		ANetGameMode* GMode = Cast<ANetGameMode>(GetWorld()->GetAuthGameMode());
		if (GMode)
		{
			GMode->TimeOver();
		}
	}
}
// Called when RemainingTime is replicated to clients
void ANetGameState::OnRep_RemainingTime()
{
	UpdateTimerDisplay();
}

