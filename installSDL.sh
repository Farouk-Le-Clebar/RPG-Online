#!/bin/bash

git clone git@github.com:libsdl-org/SDL.git

cd SDL
cmake -S . -B build 
cmake --build build
sudo cmake --install build --prefix /usr/local

cd ..
git clone git@github.com:libsdl-org/SDL_image.git
cd SDL_image
cmake -S . -B build 
cmake --build build
sudo cmake --install build --prefix /usr/local

cd ..
git clone git@github.com:libsdl-org/SDL_ttf.git
cd SDL_ttf
cmake -S . -B build 
cmake --build build
sudo cmake --install build --prefix /usr/local

cd ..
git clone git@github.com:libsdl-org/SDL_mixer.git
cd SDL_mixer
cmake -S . -B build 
cmake --build build
sudo cmake --install build --prefix /usr/local

cd ..
git clone git@github.com:libsdl-org/SDL_net.git
cd SDL_net
cmake -S . -B build 
cmake --build build
sudo cmake --install build --prefix /usr/local

cd ..
rm -rf SDL SDL_image SDL_ttf SDL_mixer SDL_net
