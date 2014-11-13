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

	UFUNCTION(BlueprintCallable, Category = Networking)
	void SetKillNum(int32 NewKillNumber);

	UFUNCTION(BlueprintCallable, Category = Networking)
		void SetUsername(const FString& NewName);

	UFUNCTION(BlueprintCallable, Category = Networking)
		void SetCharacterName(const FString& NewName);

	UFUNCTION(Server, Reliable, WithValidation, Category = Networking)
	void ServerSetTeamNum(int32 NewTeamNumber);

	UFUNCTION(Server, Reliable, WithValidation, Category = Networking)
	void ServerSetKillNum(int32 NewKillNumber);

	UFUNCTION(Server, Reliable, WithValidation, Category = Networking)
		void ServerSetUsername(const FString& NewName);

	UFUNCTION(Server, Reliable, WithValidation, Category = Networking)
		void ServerSetCharacterName(const FString& NewName);

	/** Set the mesh colors based on the current teamnum variable */
	UFUNCTION(BlueprintCallable, Category = Networking)
	void UpdateTeamColors();

	/** team number */
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_TeamColor)
	int32 TeamNumber;

	/** number of kills */
	UPROPERTY(Replicated)
	int32 Kills;

	/** Name of player */
	UPROPERTY(Replicated)
	FString Username;

	/** Name of player's character */
	UPROPERTY(Replicated)
	FString CharacterName;

	/**
	* Set the team
	*
	* @param	InController	The controller to initialize state with
	*/
	virtual void ClientInitialize(class AController* InController) override;

	
};
