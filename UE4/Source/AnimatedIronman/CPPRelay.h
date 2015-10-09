// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "BluetoothRelay.h"
#include "CPPRelay.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATEDIRONMAN_API UCPPRelay : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
		UFUNCTION(BlueprintCallable, Category = "Character")
		static void SelectCharacter(FString uId, uint8 character);
		
		UFUNCTION(BlueprintCallable, Category = "Character")
		static TArray<bool> GetSelectedCharacters();

		UFUNCTION(BlueprintCallable, Category = "Character")
		static uint8 GetCharacter(FString uId);

		UFUNCTION(BlueprintCallable, Category = "Data")
		static int32 GetTime();

		UFUNCTION(BlueprintCallable, Category = "Data")
		static TArray<uint8> GetMovements();

		static TMap<FString, uint8> players;
		static TMap<FString, uint8> movement;
	
};
