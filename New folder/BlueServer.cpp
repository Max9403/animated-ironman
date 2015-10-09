// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimatedIronman.h"
#include "BlueServer.h"

bool connected = false; 
FSocket* socket = nullptr;

FBlueServer::FBlueServer()
{
	thread = FRunnableThread::Create(this, TEXT("BluetoothWorker"), 0, TPri_Normal);
}

FBlueServer::~FBlueServer()
{
	delete thread;
	thread = nullptr;
}

bool FBlueServer::Init() {
	socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
	FString address = TEXT("127.0.0.1");
	int32 port = 23453;
	FIPv4Address ip;
	FIPv4Address::Parse(address, ip);


	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.GetValue());
	addr->SetPort(port);
	connected = socket->Connect(*addr);
	//!StartTCPReceiver("BluetoothSocketListener", "127.0.0.1", 23453);
	int data;
	socket->Send(0x0, 1 ,data);
	return connected;
}

/**
* Runs the runnable object.
*
* This is where all per object thread work is done. This is only called if the initialization was successful.
*
* @return The exit code of the runnable object
* @see Init, Stop, Exit
*/
uint32 FBlueServer::Run(){
	if (socket != nullptr) {
		//socket->Recv
	}
	return 0;
}

/**
* Stops the runnable object.
*
* This is called if a thread is requested to terminate early.
* @see Init, Run, Exit
*/
void FBlueServer::Stop(){

}

/**
* Exits the runnable object.
*
* Called in the context of the aggregating thread to perform any cleanup.
* @see Init, Run, Stop
*/
void FBlueServer::Exit() {

}

/* Tick function. */
void FBlueServer::Tick(){

}


