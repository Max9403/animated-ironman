// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimatedIronman.h"
#include "BluetoothRelay.h"

static bool serving = false;

void UBluetoothRelay::CallStaticVoidJava(const char * function) {
#if PLATFORM_ANDROID
		if (Holder::jvm != nullptr) {
				JNIEnv *env;
				jint rs = Holder::jvm->AttachCurrentThread(&env, NULL);
				if (rs == JNI_OK) {
					//jclass clazz = env->FindClass("com/ers/bluetones/SimpleWrap2");
					if (Holder::theClass != nullptr) {
						jmethodID ided = env->GetStaticMethodID(Holder::theClass, function, "()V");
						env->CallStaticVoidMethod(Holder::theClass, ided);
					}
				}
			if (env->ExceptionCheck()) {
				env->ExceptionDescribe();
			}

			Holder::jvm->DetachCurrentThread();
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("ATTACH NOPE %d"), Holder::jvm)
		}
#endif
}


void UBluetoothRelay::SendData(TArray<uint8> data, bool needsDetach) {
#if PLATFORM_ANDROID
	if (Holder::jvm != nullptr) {
		JNIEnv *env;
		jint rs = Holder::jvm->AttachCurrentThread(&env, NULL);
		if (rs == JNI_OK) {
			//jclass clazz = env->FindClass("com/ers/bluetones/SimpleWrap2");
			if (Holder::theClass != nullptr) {
				jbyteArray seq = env->NewByteArray(data.Num());
				jbyte subData[data.Num()];
				for(int ii = 0; ii < data.Num(); ii++) {
					subData[ii] = data[ii];
				}
				env->SetByteArrayRegion(seq, 0, data.Num(), subData);
				jmethodID ided = env->GetStaticMethodID(Holder::theClass, "sendData", "([B)V");
				env->CallStaticVoidMethod(Holder::theClass, ided, seq);
			}
		}
		if (env->ExceptionCheck()) {
			env->ExceptionDescribe();
		}
		if(needsDetach) {
			Holder::jvm->DetachCurrentThread();
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ATTACH NOPE %d"), Holder::jvm)
	}
#endif
	//mode->bluetooth.Send(mode->bluetooth.getSocketId(), data.GetData(), size);
}

void UBluetoothRelay::EnableBluetooth() {
	UBluetoothRelay::CallStaticVoidJava("enableBluetooth");
}

void UBluetoothRelay::MakeDiscoverable(int32 length) {

#if PLATFORM_ANDROID
		if (Holder::jvm != nullptr) {
				JNIEnv *env;
				jint rs = Holder::jvm->AttachCurrentThread(&env, NULL);
				if (rs == JNI_OK) {
					//jclass clazz = env->FindClass("com/ers/bluetones/SimpleWrap2");
					if (Holder::theClass != nullptr) {
						jmethodID ided = env->GetStaticMethodID(Holder::theClass, "makeDiscoverable", "(I)V");
						jint time = length;
						env->CallStaticVoidMethod(Holder::theClass, ided, time);
					}
				}
			if (env->ExceptionCheck()) {
				env->ExceptionDescribe();
			}

			Holder::jvm->DetachCurrentThread();
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("ATTACH NOPE %d"), Holder::jvm)
		}
#endif
}

void UBluetoothRelay::StartDiscovery() {
	Holder::devices.Empty();
	UBluetoothRelay::CallStaticVoidJava("searchDevices");
}

void UBluetoothRelay::StopDiscovery() {
	UBluetoothRelay::CallStaticVoidJava("cancelSearch");
}

void UBluetoothRelay::SetServer(bool server) {
	serving = server;
#if PLATFORM_ANDROID
		if (Holder::jvm != nullptr) {
				JNIEnv *env;
				jint rs = Holder::jvm->AttachCurrentThread(&env, NULL);
				if (rs == JNI_OK) {
					//jclass clazz = env->FindClass("com/ers/bluetones/SimpleWrap2");
					if (Holder::theClass != nullptr) {
						jmethodID ided = env->GetStaticMethodID(Holder::theClass, "setServer", "(Z)V");
						jboolean time = server;
						env->CallStaticVoidMethod(Holder::theClass, ided, time);
					}
				}
			if (env->ExceptionCheck()) {
				env->ExceptionDescribe();
			}

			Holder::jvm->DetachCurrentThread();
}
		else {
			UE_LOG(LogTemp, Warning, TEXT("ATTACH NOPE %d"), Holder::jvm)
		}
#endif
}

void UBluetoothRelay::StartServer() {
	UBluetoothRelay::CallStaticVoidJava("setupServer");
}

void UBluetoothRelay::Disconnect() {
	UBluetoothRelay::CallStaticVoidJava("disconnect");
}

bool UBluetoothRelay::Connect(FString name) {
	FString device = *Holder::devices.Find(name);
	bool result = false;

#if PLATFORM_ANDROID
	if (Holder::jvm != nullptr) {
		JNIEnv *env;
		jint rs = Holder::jvm->AttachCurrentThread(&env, NULL);
		if (rs == JNI_OK) {
			//jclass clazz = env->FindClass("com/ers/bluetones/SimpleWrap2");
			if (Holder::theClass != nullptr) {
				jmethodID ided = env->GetStaticMethodID(Holder::theClass, "connectDevice", "(Ljava/lang/String;)Z");
				jstring time = env->NewStringUTF(TCHAR_TO_ANSI(*device));
				result = env->CallStaticBooleanMethod(Holder::theClass, ided, time);
			}
		}
		if (env->ExceptionCheck()) {
			env->ExceptionDescribe();
		}

		Holder::jvm->DetachCurrentThread();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ATTACH NOPE %d"), Holder::jvm)
	}
#endif
	return result;
}

TArray<FString> UBluetoothRelay::GetDevices() {
	TArray<FString> names;
	Holder::devices.GenerateKeyArray(names);
	return names;
}

bool UBluetoothRelay::IsServer() {
	return serving;
}

bool UBluetoothRelay::IsConnected() {
	bool result = false;
#if PLATFORM_ANDROID
	if (Holder::jvm != nullptr) {
		JNIEnv *env;
		jint rs = Holder::jvm->AttachCurrentThread(&env, NULL);
		if (rs == JNI_OK) {
			//jclass clazz = env->FindClass("com/ers/bluetones/SimpleWrap2");
			if (Holder::theClass != nullptr) {
				jmethodID ided = env->GetStaticMethodID(Holder::theClass, "isConnected", "()Z");
				
				result = env->CallStaticBooleanMethod(Holder::theClass, ided);
			}
		}
		if (env->ExceptionCheck()) {
			env->ExceptionDescribe();
		}

		Holder::jvm->DetachCurrentThread();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ATTACH NOPE %d"), Holder::jvm)
	}
#endif
	return result;
}