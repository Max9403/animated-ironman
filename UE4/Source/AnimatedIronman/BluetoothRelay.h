// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnimatedIronmanGameMode.h"
#if PLATFORM_ANDROID
#include <jni.h>
#endif
#include "java/Holder.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BluetoothRelay.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ANIMATEDIRONMAN_API UBluetoothRelay : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	private:
		static void CallStaticVoidJava(const char * function);

	public:
		UFUNCTION(BlueprintCallable, Category = "Bluetooth")
			static void SendData(TArray<uint8> data, bool needsDetach = true);
		UFUNCTION(BlueprintCallable, Category = "Bluetooth")
			static void EnableBluetooth();
		UFUNCTION(BlueprintCallable, Category = "Bluetooth")
			static void MakeDiscoverable(int32 length);
		UFUNCTION(BlueprintCallable, Category = "Bluetooth")
			static void StartDiscovery();
		UFUNCTION(BlueprintCallable, Category = "Bluetooth")
			static void StopDiscovery();
		UFUNCTION(BlueprintCallable, Category = "Bluetooth")
			static void SetServer(bool server);
		UFUNCTION(BlueprintCallable, Category = "Bluetooth")
			static void StartServer();
		UFUNCTION(BlueprintCallable, Category = "Bluetooth")
			static bool Connect(FString name);
		UFUNCTION(BlueprintCallable, Category = "Bluetooth")
			static TArray<FString> GetDevices();

		UFUNCTION(BlueprintCallable, Category = "Bluetooth")
			static bool IsServer();
		UFUNCTION(BlueprintCallable, Category = "Bluetooth")
			static bool IsConnected();
		UFUNCTION(BlueprintCallable, Category = "Bluetooth")
			static void Disconnect();
};
