// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimatedIronman.h"
#include "CPPRelay.h"

TMap<FString, uint8> UCPPRelay::players;
TMap<FString, uint8> UCPPRelay::movement;

void UCPPRelay::SelectCharacter(FString uId, uint8 character) {
	TArray<uint8> data;
	data.Add(0x0);
	data.Add(character);
	UCPPRelay::players.Add(uId, character);
	UBluetoothRelay::SendData(data);
}

TArray<bool> UCPPRelay::GetSelectedCharacters() {
	TArray<uint8> valis;
	TArray<bool> characters;
	players.GenerateValueArray(valis);
	if (characters.Num() < 4) {
		for (int pad = 0; pad < 4; pad++) {
			characters.Add(false);
		}
	}
	for (uint8 ff : valis) {
		if (ff > -1) {
			characters[ff] = true;
		}
	}
	return characters;
}

uint8 UCPPRelay::GetCharacter(FString uId) {
	uint8 *item = players.Find(uId);
	return item == nullptr ? -1 : *item;
}

int32 UCPPRelay::GetTime() {
	return FDateTime::Now().GetMillisecond();
}

TArray<uint8> UCPPRelay::GetMovements() {
	TArray<uint8> moving;
	for (int pad = 0; pad < 4; pad++) {
		moving.Add(0);
	}
	TArray<FString> valis;
	players.GenerateKeyArray(valis);
	for (FString pp : valis) {
		uint8* mm = movement.Find(pp);
		moving[*players.Find(pp)] = mm == nullptr ? 0 : *mm;
	}
	return moving;
}