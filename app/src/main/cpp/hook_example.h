//
// Created by Zhang Shizhen on 28/11/17.
//

#ifndef ANDROIDCYDIAHOOK_HOOK_EXAMPLE_H
#define ANDROIDCYDIAHOOK_HOOK_EXAMPLE_H

#include <android/log.h>

#define TAG "HOOK_EXAMPLE"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

const int testPassword(const char *myString);

#endif //ANDROIDCYDIAHOOK_HOOK_EXAMPLE_H
