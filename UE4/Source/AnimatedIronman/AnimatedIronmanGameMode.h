// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BlueServer.h"
#include "GameFramework/GameMode.h"
#include "AnimatedIronmanGameMode.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ANIMATEDIRONMAN_API AAnimatedIronmanGameMode : public AGameMode
{
	GENERATED_BODY()

	public:
		//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bluetooth")
		volatile FBlueServer bluetooth;
		volatile int sockfd;

		
		//UFUNCTION(BlueprintCallable, Category = "ServerConnection")
		//static void ServerConnection() {

			/*FSocket* socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
			FString address = TEXT("127.0.0.1");
			int32 port = 23453;
			FIPv4Address ip;
			FIPv4Address::Parse(address, ip);


			TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
			addr->SetIp(ip.GetValue());
			addr->SetPort(port);
			bool connected = socket->Connect(*addr);
			//!StartTCPReceiver("BluetoothSocketListener", "127.0.0.1", 23453);
			int data;
			socket->Send(0x0, 1, data);*/
			//close(sockfd);
		//}
};
