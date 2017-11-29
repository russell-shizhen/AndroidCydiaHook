#include <jni.h>
#include <string.h>
#include "hook_example.h"

JNIEXPORT jint JNICALL
Java_com_example_shizzhan_sampleapp_MainActivity_testPassword(JNIEnv *env, jobject instance, jstring myString) {

    const char *password = (*env)->GetStringUTFChars(env, myString, 0);

    int ret = testPassword(password);

    LOGI("In JNI method, password is: %s", password);

    return ret;
}

JNIEXPORT jint JNICALL
Java_com_example_shizzhan_sampleapp_MainActivity_testStrncmp(JNIEnv *env, jobject instance,
                                                             jstring s1_, jstring s2_, jint size) {
    const char *s1 = (*env)->GetStringUTFChars(env, s1_, 0);
    const char *s2 = (*env)->GetStringUTFChars(env, s2_, 0);

    int cmpResult = strncmp(s1, s2, (size_t)size);

    (*env)->ReleaseStringUTFChars(env, s1_, s1);
    (*env)->ReleaseStringUTFChars(env, s2_, s2);

    return cmpResult;
}