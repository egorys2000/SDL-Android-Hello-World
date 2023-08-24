LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL
SDL_IMAGE_PATH := ../SDL_image  # Path to SDL_Image library
SDL_TTF_PATH := ../SDL_ttf  # Path to SDL_TTF library

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SDL_IMAGE_PATH)/include  # Include SDL_Image header files
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SDL_TTF_PATH)/include  # Include SDL_TTF header files

# Add your application source files here...
LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/*.cpp)

LOCAL_SHARED_LIBRARIES := SDL2
LOCAL_SHARED_LIBRARIES += SDL2_image  # Link against SDL_Image
LOCAL_SHARED_LIBRARIES += SDL2_ttf  # Link against SDL_TTF

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -landroid

include $(BUILD_SHARED_LIBRARY)
