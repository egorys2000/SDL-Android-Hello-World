# SDL Android Hello World
Template for building SDL on Android + SDL supporting libraries (SDL_image, SDL_ttf)

## Requirements
Make sure JDK, Android SDK and NDK are installed. Check that your JAVA_HOME, ANDROID_HOME, ANDROID_NDK_HOME system paths are set.

This project was tested with
- JDK 14.0.2
- SDK 26.1.1
- NDK 21.4.7075529

## Installation
Run `./install-dependencies.sh`. This does the following:
- Install SDL, SDl_image, SDL_ttf source code and move it to YOURPROJECT/app/jni/.
- Install externals for SDL_ttf

Before building check, that you have the following folders installed:
- [SDL/](https://github.com/libsdl-org/SDL), [SDL_image/](https://github.com/libsdl-org/SDL_image), [SDL_ttf/](https://github.com/libsdl-org/SDL_ttf) folders in YOURPROJECT/app/jni/
- SDl_ttf/external/ contains freetype/ and harfbuzz/ linked [here](https://github.com/libsdl-org/SDL_ttf/tree/main/external)

If you find some of these folders missing, you should download them manually from official repositories

## Building for Android
Run `YOURPROJECT/gradlew installDebug` with connected Android device. Don't forget to enable developer mode and USB Debugging on it.

For any further details please reffer to official SDL Android build [instructions](https://github.com/libsdl-org/SDL/blob/main/docs/README-android.md)

