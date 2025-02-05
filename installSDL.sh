#!/bin/bash

set -e

modules=("SDL" "SDL_image" "SDL_ttf" "SDL_mixer" "SDL_net" "libnoise")

for module in "${modules[@]}"; do
    if [ "$module" == "libnoise" ]; then
        if ls /usr/local/lib/libnoise* >/dev/null 2>&1; then
            echo "✅ libnoise already installed, skipping installation."
        else
            echo "🔄 Cloning libnoise repository..."
            git clone git@github.com:qknight/libnoise.git
            cd libnoise

            echo "⚙️ Configuring CMake for libnoise..."
            cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

            echo "🚀 Building libnoise..."
            cmake --build build

            echo "📦 Installing libnoise..."
            sudo cmake --install build --prefix /usr/local

            echo "✅ Checking installation of libnoise..."
            if ls /usr/local/lib/libnoise* >/dev/null 2>&1; then
                echo "✅ libnoise installed successfully!"
            else
                echo "❌ ERROR: libnoise does not seem to be installed correctly!"
            fi

            cd ..
        fi
    else
        if ls /usr/local/lib/libSDL3*${module#SDL}* >/dev/null 2>&1; then
            echo "✅ $module already installed, skipping installation."
        else
            echo "🔄 Cloning repository: $module..."
            git clone https://github.com/libsdl-org/$module.git
            cd $module

            echo "⚙️ Configuring CMake for $module..."
            cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

            echo "🚀 Building $module..."
            cmake --build build

            echo "📦 Installing $module..."
            sudo cmake --install build --prefix /usr/local

            echo "✅ Checking installation of $module..."
            if ls /usr/local/lib/libSDL3*${module#SDL}* >/dev/null 2>&1; then
                echo "✅ $module installed successfully!"
            else
                echo "❌ ERROR: $module does not seem to be installed correctly!"
            fi

            cd ..
        fi
    fi
done

echo "🧹 Removing source directories..."
rm -rf SDL SDL_image SDL_ttf SDL_mixer SDL_net libnoise

echo "🔍 Final verification of installed SDL and libnoise libraries:"

for module in "${modules[@]}"; do
    if [ "$module" == "libnoise" ]; then
        echo -n "📌 libnoise: "
        if ls /usr/local/lib/libnoise* >/dev/null 2>&1; then
            echo "✅ Found"
        else
            echo "❌ Not found"
        fi
    else
        echo -n "📌 $module: "
        if ls /usr/local/lib/libSDL3*${module#SDL}* >/dev/null 2>&1; then
            echo "✅ Found"
        else
            echo "❌ Not found"
        fi
    fi
done

echo "🎉 Installation completed successfully!"
