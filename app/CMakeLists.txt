# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(MODULE_NAME fooSo)

include_directories(src/main/cpp)

add_library( ${MODULE_NAME}
             SHARED
             src/main/jni/fooSo.c
             src/main/cpp/hook_example.c )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( log-lib log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( ${MODULE_NAME}
                       ${log-lib})



set(HOOK_MODULE hook.cy)

include_directories(src/main/cpp)

add_library( ${HOOK_MODULE}
             SHARED
             src/main/cpp/hook.cpp )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( ${HOOK_MODULE}
                       ${log-lib}
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libsubstrate-dvm.so
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libsubstrate.so)

