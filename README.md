# OLC Game Jame "Change"

This project is my entry to the olc GameJam with the theme "Change"

This project is built with [OlcPixelGameEngine V3](https://github.com/OneLoneCoder/olcPixelGameEngine3).

## How to build

Configure and generate Makefile

`cmake -S . -B build`

Compile 

`cmake --build build`

Run

`./build/main` 
or
`cmake --build build && ./build/main`


### Build for Web

Configure and generate Makefile

`emcmake cmake -S . -B build-web`
`cmake --build build-web`
`npx http-server`

At `localhost:8080/build/Main.html` the game should appear.

## Problem Solving

