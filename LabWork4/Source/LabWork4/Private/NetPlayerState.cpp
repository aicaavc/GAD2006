// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayerState.h"
#include "NetBaseCharacter.h"
#include "Net/UnrealNetwork.h"

// Call the parent class implementation to include its replicated properties

void ANetPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	// Replicate the 'Data' and 'PlayerIndex' properties for this player state
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANetPlayerState, Data);
	DOREPLIFETIME(ANetPlayerState, PlayerIndex);
}

void ANetPlayerState::OnRep_PlayerInfo()
{
	// Attempt to cast the player's pawn to ANetBaseCharacter
	ANetBaseCharacter* Char = Cast<ANetBaseCharacter>(GetPawn());
	if (Char)
	{
		Char->PlayerInfoReceived = true;
	}
	else
	{
		// If the pawn is not yet set, set a timer to retry after a short delay
		FTimerHandle PlayerInfoUpdateTimer;
		GWorld->GetTimerManager().SetTimer(PlayerInfoUpdateTimer, this, &ANetPlayerState::OnRep_PlayerInfo, 0.25f, false);
	}
}
