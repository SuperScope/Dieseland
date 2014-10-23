// Fill out your copyright notice in the Description page of Project Settings.

#include "Dieseland.h"
#include "SpotlightArray.h"


ASpotlightArray::ASpotlightArray(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	SetRemoteRoleForBackwardsCompat(ENetRole::ROLE_SimulatedProxy);
}


