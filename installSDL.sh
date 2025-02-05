#!/bin/bash

set -e

modules=("SDL" "SDL_image" "SDL_ttf" "SDL_mixer" "SDL_net")

for module in "${modules[@]}"; do
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
    if [ -f "/usr/local/lib/lib${module}.so" ] || [ -f "/usr/local/lib/lib${module}.a" ]; then
        echo "✅ $module installed successfully!"
    else
        echo "❌ ERROR: $module does not seem to be installed correctly!"
    fi

    cd ..
done

echo "🧹 Removing source directories..."
rm -rf SDL SDL_image SDL_ttf SDL_mixer SDL_net

echo "🔍 Final verification of installed SDL libraries:"

for module in "${modules[@]}"; do
    echo -n "📌 $module: "
    if ldconfig -p | grep -i "lib${module}.so" >/dev/null || [ -f "/usr/local/lib/lib${module}.so" ] || [ -f "/usr/local/lib/lib${module}.a" ]; then
        echo "✅ Found"
    else
        echo "❌ Not found"
    fi
done

echo "🎉 Installation completed successfully!"
