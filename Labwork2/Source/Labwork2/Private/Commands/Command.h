// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameSlot.h"

/**
 * 
 */
class Command
{
public:
	Command();
	~Command();

	virtual void Execute() = 0;
	virtual void Revert() = 0;
	virtual void Update(float DeltaTime) {};
	virtual bool IsExecuting() { return false; };
};
