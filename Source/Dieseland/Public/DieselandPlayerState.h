// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "DieselandPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DIESELAND_API ADieselandPlayerState : public APlayerState
{
	GENERATED_UCLASS_BODY()

public:
	/** replicate team colors. Updated the players mesh colors appropriately */
	UFUNCTION()
	void OnRep_TeamColor();

	/** get current team */
	UFUNCTION(BlueprintCallable, Category = Networking)
	int32 GetTeamNum() const;

	/**
	* Set new team and update pawn. Also updates player character team colors.
	*
	* @param	NewTeamNumber	Team we want to be on.
	*/
	UFUNCTION(BlueprintCallable, Category = Networking)
	void SetTeamNum(int32 NewTeamNumber);

	/** Set the mesh colors based on the current teamnum variable */
	UFUNCTION(BlueprintCallable, Category = Networking)
	void UpdateTeamColors();

protected:
	/** team number */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_TeamColor)
	int32 TeamNumber;

	
};
