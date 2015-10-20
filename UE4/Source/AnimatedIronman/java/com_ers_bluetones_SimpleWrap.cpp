#include "AnimatedIronman.h"
#include "com_ers_bluetones_SimpleWrap.h"


#if PLATFORM_ANDROID
#ifdef __cplusplus
extern "C" {
#endif
	JNIEXPORT void JNICALL Java_com_ers_bluetones_BlueService_initNative(JNIEnv* envo, jobject obj, jclass classy) {
		int status = envo->GetJavaVM(&Holder::jvm);
		if (status != 0) {
			UE_LOG(LogTemp, Warning, TEXT("WE FAILED"))
		}
		UE_LOG(LogTemp, Warning, TEXT("ATTACH %d <> %d"), status, Holder::jvm)
		Holder::theClass = (jclass)envo->NewGlobalRef(classy);
	}

	JNIEXPORT void JNICALL Java_com_ers_bluetones_BlueService_foundDevice
		(JNIEnv * env, jobject ob, jstring devName, jstring devId) {
			const char *nativeName = env->GetStringUTFChars(devName, JNI_FALSE);
			const char *nativeId = env->GetStringUTFChars(devId, JNI_FALSE);
			UE_LOG(LogTemp, Warning, TEXT("FOUND DEVICE %s"), ANSI_TO_TCHAR(nativeName))
				Holder::devices.Add(FString(nativeName), FString(nativeId));
			env->ReleaseStringUTFChars(devName, nativeName);
			env->ReleaseStringUTFChars(devId, nativeId);
		
	}
	JNIEXPORT void JNICALL Java_com_ers_bluetones_BlueService_receiveData
		(JNIEnv * env, jclass clazz, jstring device, jobject data){
		const char* devi = env->GetStringUTFChars(device, JNI_FALSE);
		jbyte *buffy = (jbyte*)(env->GetDirectBufferAddress(data));
		switch(buffy[0]) {
			case 0:
				UCPPRelay::players.Add(FString(devi), buffy[1]);
				break;
			case 1:
				UE_LOG(LogTemp, Warning, TEXT("%s mov %d"), ANSI_TO_TCHAR(devi), buffy[1])
				UCPPRelay::movement.Add(FString(devi), buffy[1]);
				break;
		}
		env->ReleaseStringUTFChars(device, devi);
	}


	JNIEXPORT void JNICALL Java_com_ers_bluetones_BlueService_syncData
		(JNIEnv *, jclass){
		for (auto& elem : UCPPRelay::players) {
			UE_LOG(LogTemp, Warning, TEXT("%s : %d"), *elem.Key, elem.Value)
			TArray<uint8> data;
			data.Add(0x0);
			data.Add(elem.Value);
			UBluetoothRelay::SendData(data, false);
		}
	}
#ifdef __cplusplus
}
#endif
#endif
