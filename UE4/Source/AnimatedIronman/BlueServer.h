// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#if PLATFORM_ANDROID
	#include<iostream>    //cout
	#include<stdio.h> //printf
	#include<string.h>    //strlen
	#include<string>  //string
	#include<sys/socket.h>    //socket
	#include<arpa/inet.h> //inet_addr
	#include<netdb.h> //hostent
#endif
#include "Networking.h"


/**
 * 
 */
class ANIMATEDIRONMAN_API FBlueServer : public FRunnable, FSingleThreadRunnable
{
	volatile FRunnableThread * thread;

	bool connected = false;
	volatile int sockfd = 0;

#if PLATFORM_ANDROID
	int  portno = 23453, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
#endif
	public:

		FBlueServer();
		virtual ~FBlueServer();

		/**
		* Initializes the runnable object.
		*
		* This method is called in the context of the thread object that aggregates this, not the
		* thread that passes this runnable to a new thread.
		*
		* @return True if initialization was successful, false otherwise
		* @see Run, Stop, Exit
		*/
		virtual bool Init();

		/**
		* Runs the runnable object.
		*
		* This is where all per object thread work is done. This is only called if the initialization was successful.
		*
		* @return The exit code of the runnable object
		* @see Init, Stop, Exit
		*/
		virtual uint32 Run();

		/**
		* Stops the runnable object.
		*
		* This is called if a thread is requested to terminate early.
		* @see Init, Run, Exit
		*/
		virtual void Stop();

		/**
		* Exits the runnable object.
		*
		* Called in the context of the aggregating thread to perform any cleanup.
		* @see Init, Run, Stop
		*/
		virtual void Exit();

		/* Tick function. */
		virtual void Tick();

		int Send(int sockfd, uint8 buffer[], int size) volatile;

		int getSocketId() volatile;

		/**
		* Gets single thread interface pointer used for ticking this runnable when multi-threading is disabled.
		* If the interface is not implemented, this runnable will not be ticked when FPlatformProcess::SupportsMultithreading() is false.
		*
		* @return Pointer to the single thread interface or nullptr if not implemented.
		*/
		virtual class FSingleThreadRunnable* GetSingleThreadInterface()
		{
			return this;
		}
		
};
