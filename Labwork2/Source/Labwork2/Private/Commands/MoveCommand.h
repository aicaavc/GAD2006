// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commands/Command.h"

/**
 * 
 */
class MoveCommand : public Command
{
public:
	MoveCommand(FSGridPosition Src, FSGridPosition Dst);
	~MoveCommand();
	virtual void Execute() override;

	virtual void Revert() override;


public:
	FSGridPosition Source, Destination;
};
