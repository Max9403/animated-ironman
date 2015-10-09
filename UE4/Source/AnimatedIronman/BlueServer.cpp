// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimatedIronman.h"
#include "BlueServer.h"

bool connected = false; 
volatile int sockfd = 0;

#if PLATFORM_ANDROID
int  portno = 23453, n;
struct sockaddr_in serv_addr;
struct hostent *server;
#endif

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
	/*socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
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
	socket->Send(0x0, 1 ,data);*/

#if PLATFORM_ANDROID
	portno = 23453;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		connected = false;
	}
	server = gethostbyname("127.0.0.1");
	if (server == NULL) {
		connected = false;
	}
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		connected = false;
	}

	UE_LOG(LogTemp, Warning, TEXT("SOCKET: %d"), sockfd)
	connected = true;
#endif
	/*printf("%s\n", buffer);
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	portno = 23453;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		return false;
	}
	server = gethostbyname("127.0.0.1");
	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		return false;
	}
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		return false;
	printf("Please enter the message: ");
	char buffer[1];
	bzero(buffer, 1);
	n = write(sockfd, buffer, strlen(buffer));
	if (n < 0) {
		return false;
	}
	printf("%s\n", buffer);
	close(sockfd);*/
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
	if (sockfd) {

#if PLATFORM_ANDROID
			
			
#endif
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
int FBlueServer::getSocketId() volatile {
	return sockfd;
}

int FBlueServer::Send(int sockfd, uint8 buffer[], int size) volatile {
#if PLATFORM_ANDROID
	UE_LOG(LogTemp, Warning, TEXT("WRITING"))
	return write(sockfd, buffer, size);
#else
	return -1;
#endif
}


