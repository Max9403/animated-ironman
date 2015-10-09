#include "AnimatedIronman.h"
#include "Holder.h"

#if PLATFORM_ANDROID
JavaVM *Holder::jvm = nullptr;
jclass Holder::theClass;
#endif
TMap<FString, FString> Holder::devices;