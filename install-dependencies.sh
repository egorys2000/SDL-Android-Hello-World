#!/bin/bash -x

# Define variables for library names and versions
SDL_VERSION="2.28.2"
SDL_IMAGE_VERSION="2.6.3"
SDL_TTF_VERSION="2.20.2"
HARFBUZZ_VERSION="8.1.1"
FREETYPE_VERSION="27"

cd YOURPROJECT/app/jni/

remove_directory() {
    if [ -d "$1" ]; then
        rm -rf "$1"
    fi
}

# Remove existing libraries if they exist
remove_directory "SDL2"
remove_directory "SDL2_image"
remove_directory "SDL2_ttf"

# Download and extract SDL2
SDL_URL="https://www.libsdl.org/release/SDL2-$SDL_VERSION.zip"
curl -o "SDL2-$SDL_VERSION.zip" -L "$SDL_URL"
unzip -q "SDL2-$SDL_VERSION.zip" -d "SDL2_temp"
mv SDL2_temp/* SDL2
rm -rf SDL2_temp
rm "SDL2-$SDL_VERSION.zip"

# Download and extract SDL_image
SDL_IMAGE_URL="https://www.libsdl.org/projects/SDL_image/release/SDL2_image-$SDL_IMAGE_VERSION.zip"
curl -o "SDL2_image-$SDL_IMAGE_VERSION.zip" -L "$SDL_IMAGE_URL"
unzip -q "SDL2_image-$SDL_IMAGE_VERSION.zip" -d "SDL2_image_temp"
mv SDL2_image_temp/* SDL2_image
rm -rf SDL2_image_temp/
rm "SDL2_image-$SDL_IMAGE_VERSION.zip"

# Download and extract SDL_ttf
SDL_TTF_URL="https://github.com/libsdl-org/SDL_ttf/archive/refs/tags/release-$SDL_TTF_VERSION.zip"
curl -o "SDL2_ttf-$SDL_TTF_VERSION.zip" -L "$SDL_TTF_URL"
unzip -q "SDL2_ttf-$SDL_TTF_VERSION.zip" -d "SDL2_ttf_temp"
mv SDL2_ttf_temp/* SDL2_ttf
rm -rf SDL2_ttf_temp
rm "SDL2_ttf-$SDL_TTF_VERSION.zip"

cd "SDL2_ttf/external"

# Download and extract FreeType
FREETYPE_URL="https://download.savannah.gnu.org/releases/freetype/ft$FREETYPE_VERSION.zip"
curl -o "freetype.zip" -L "$FREETYPE_URL"
unzip -q "freetype.zip" -d "freetype_temp"
mv freetype_temp/* freetype
rm -rf freetype_temp
rm "freetype.zip"

# Download and extract HarfBuzz
HARFBUZZ_URL="https://github.com/harfbuzz/harfbuzz/archive/refs/tags/$HARFBUZZ_VERSION.zip"
curl -o "harfbuzz.zip" -L "$HARFBUZZ_URL"
unzip -q "harfbuzz.zip" -d "harfbuzz_temp"
mv harfbuzz_temp/* harfbuzz
rm -rf harfbuzz_temp
rm "harfbuzz.zip"
