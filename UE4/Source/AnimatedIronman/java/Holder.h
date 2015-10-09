#pragma once

#if PLATFORM_ANDROID
#include <jni.h>
#endif

class Holder {
public:
#if PLATFORM_ANDROID
	static JavaVM *jvm;
	static jclass theClass;
#endif
	static TMap<FString, FString> devices;
};