#include <jni.h>
#include "substrate.h"
#include <android/log.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define TAG "SHIZHEN_HOOKTEST"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

MSConfig(MSFilterLibrary, "libfooSo.so");
MSConfig(MSFilterLibrary, "libc.so");
MSConfig(MSFilterLibrary, "libdvm.so");

void performHook(const char *targetSo, const char *symbol, void *replace, void **original);

bool (*_dvmLoadNativeCode)(char* pathName, void* classLoader, char** detail);
bool fake_dvmLoadNativeCode(char* soPath, void* classLoader, char** detail)
{
    LOGD("fake_dvmLoadNativeCode soPath:%s", soPath);
    return _dvmLoadNativeCode(soPath,classLoader,detail);
}

const int (*original_testPassword)(const char *myString);
const int faked_testPassword(const char *myString)
{
    LOGE("testPassword() is hooked!!!!!!, will always return ZERO !!!!!!");

    return 0;
}

static int (*original_strncmp)(const char*, const char*, size_t);
int faked_strncmp(const char* a, const char * b, size_t n)
{
    LOGE("strncmp() is hooked!!!!!!");
    if(strcmp("apple", a) == 0) {
        LOGE("strncmp() is hooked!!!!!!, will always return ZERO is the string is \"apple\" !!!!!!");
        return 0;
    }

    return original_strncmp(a, b, n);
}

void* find_symbol(const char* libraryname, const char* symbolname)
{
    void *imagehandle = dlopen(libraryname, RTLD_GLOBAL | RTLD_NOW);
    if (imagehandle != NULL) {
        void * sym = dlsym(imagehandle, symbolname);
        if (sym != NULL) {
            LOGE("symbol (%s) is found at address %p (%p) in lib %s", symbolname, sym, &sym, libraryname);
            return sym;
        } else {
            LOGE("find_symbol() can't find symblo (%s).", symbolname);
            return NULL;
        }
    } else {
        LOGE("dlopen error: %s, when opening lib %s",dlerror(), libraryname);
        return NULL;
    }
}

// Substrate entry point
MSInitialize{

    // Example 1: hook user defined .so
    // Hook testPassword
    const char *fooSoPath = "/data/data/com.example.shizzhan.sampleapp/lib/libfooSo.so";
    performHook(fooSoPath, "testPassword", (void*)&faked_testPassword, (void**)&original_testPassword);

    // Example 2: hook libc.so. Hooking libc functions may cause the system hang. So, don't play around libc hookings.
    // Hook strncmp
    performHook("/system/lib/libc.so", "strncmp", (void*)&faked_strncmp, (void**)&original_strncmp);

    // Example 3: hook libdvm.so
    // Hook dvmLoadNativeCode
    performHook("/system/lib/libdvm.so", "_Z17dvmLoadNativeCodePKcP6ObjectPPc", (void*)&fake_dvmLoadNativeCode, (void**)&_dvmLoadNativeCode);
}

void performHook(const char *targetSoPath, const char *symbol, void *replace, void **original) {

    // MSGetImageByName only work when the .so has already been loaded.
    MSImageRef image = MSGetImageByName(targetSoPath);

    void *symAddress;

    if (image != NULL) {
        LOGE("===>>>>>> MSGetImageByName (%s) succeed, symbol address: %p", targetSoPath, image);
        symAddress = MSFindSymbol(image, symbol);
        LOGE("===>>>>>> MSFindSymbol (%s) succeed, symbol address: %p", symbol, symAddress);
        MSHookFunction(symAddress, replace, original);
    } else { // When the .so has not been loaded, need to use below code to find the symbol.
        LOGE("Image not found, trying with find_symbol");
        // The following will work, as it will forcefully load given library.
        symAddress = find_symbol(targetSoPath, symbol);
        if (symAddress != NULL) {
            LOGE("===>>>>>> find_symbol (%s) succeed, symbol address: %p", symbol, symAddress);
            MSHookFunction(symAddress, replace, original);
            LOGE("===>>>>>> find_symbol (%s) succeed, symbol address: %p", symbol, symAddress);
        } else {
            LOGE("===>>>>>> find_symbol failed");
        }
    }
}