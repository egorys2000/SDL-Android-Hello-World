#!/bin/bash -x

# Define variables for library names and versions
SDL_VERSION="2.28.2"
SDL_IMAGE_VERSION="2.6.3"
SDL_TTF_VERSION="2.20.2"
HARFBUZZ_VERSION="2.9.1"
FREETYPE_VERSION="2.13.0"

remove_directory() {
    if [ -d "$1" ]; then
    echo "Removing directory: $1"
        rm -rf "$1"
    fi
}

remove_directory YOURPROJECT/.gradle
remove_directory YOURPROJECT/app/build

cd YOURPROJECT/app/jni/

# Remove existing libraries if they exist
for dir in SDL*; do
    remove_directory "$dir"
done

# Download and extract SDL2
SDL_URL="https://www.libsdl.org/release/SDL2-$SDL_VERSION.zip"
curl -o "SDL2-$SDL_VERSION.zip" -L "$SDL_URL"
unzip -q "SDL2-$SDL_VERSION.zip" -d "SDL_temp"
mv SDL_temp/* SDL
rm -rf SDL_temp
rm "SDL2-$SDL_VERSION.zip"

# Download and extract SDL_image
SDL_IMAGE_URL="https://www.libsdl.org/projects/SDL_image/release/SDL2_image-$SDL_IMAGE_VERSION.zip"
curl -o "SDL2_image-$SDL_IMAGE_VERSION.zip" -L "$SDL_IMAGE_URL"
unzip -q "SDL2_image-$SDL_IMAGE_VERSION.zip" -d "SDL_image_temp"
mv SDL_image_temp/* SDL_image
rm -rf SDL_image_temp
rm "SDL2_image-$SDL_IMAGE_VERSION.zip"

# Download and extract SDL_ttf
SDL_TTF_URL="https://github.com/libsdl-org/SDL_ttf/archive/refs/tags/release-$SDL_TTF_VERSION.zip"
curl -o "SDL2_ttf-$SDL_TTF_VERSION.zip" -L "$SDL_TTF_URL"
unzip -q "SDL2_ttf-$SDL_TTF_VERSION.zip" -d "SDL_ttf_temp"
mv SDL_ttf_temp/* SDL_ttf
rm -rf SDL_ttf_temp
rm "SDL2_ttf-$SDL_TTF_VERSION.zip"

cd "SDL_ttf/external"

# Download and extract FreeType
FREETYPE_URL="https://github.com/libsdl-org/freetype/archive/fd08bbe13c52e305f037448e805e7e3c6ea1b1e4.zip"
curl -o "freetype.zip" -L "$FREETYPE_URL"
unzip -q "freetype.zip" -d "freetype_temp"
mv freetype_temp/*/* freetype
rm -rf freetype_temp
rm "freetype.zip"

# Download and extract HarfBuzz
HARFBUZZ_URL="https://github.com/libsdl-org/harfbuzz/archive/43931e3e596c04044861770b831c8f9452e2d3b0.zip"
curl -o "harfbuzz.zip" -L "$HARFBUZZ_URL"
unzip -q "harfbuzz.zip" -d "harfbuzz_temp"
mv harfbuzz_temp/*/* harfbuzz
rm -rf harfbuzz_temp
rm "harfbuzz.zip"