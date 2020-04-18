# Chip8
This is a chip8 emulator. It is designed as a hobby project first so I did not put in much effort to make this a perfect emulation by any means. However, it should run most roms properly and uses the SDL2 lib to display the virtual screen to the user.

![Chip8 Picture](https://github.com/bankent1/chip8/blob/master/chip8-pic.png)
![Chip8 Space Invaders Menu](https://github.com/bankent1/chip8/blob/master/chip8-space-invaders.png)
![Chip8 Space Invaders](https://github.com/bankent1/chip8/blob/master/chip8-si2.png)

## Features
This program allows you to run any chip8 rom you have on your machine. Simply provide the path to the rom when you run the command. The chip8 also allows you to adjust the speed at which it executes instructions. This is usefull as some ROMS will work better at slower or faster speeds.
  
There is currently no sound supported because sound adds a lot of complication to emulation and this projects was meant to be a simple hobby project for me. However, every other instruction (aside one which is not important for most roms) was implemented.

The emulator can also be run in step-mode. This allows the user to step one instruction at a time. This is mainly a debugging feature, but I think it can be cool to see the processor think at a human understandable speed.

## Dependencies
- [SDL2](https://www.libsdl.org/)
- Linux (or linux VM)
- make
- gcc

## How to Build
This project was build on linux and currently only supports linux. There is not too much stopping it from being used on other platforms, so I may revisit it in the future to be cross platform. As of right now, you will need a linux machine (or vm) and the SDL2 dependency. SDL2 is used for the frontend rendering.
  
To build: `make build`  
  
This will generetate a `chip8` binary to execute.

## Usage
Run `chip8 --help` to see all options available to you.
To simply run a ROM you have: `chip8 <path-to-rom>`

## Keymapping
The chip8 supports a hex-based keyboard (0-F).  
```
Host       Chip8  
0    ->    0  
1    ->    1  
2    ->    2  
3    ->    3  
4    ->    4  
5    ->    5  
6    ->    6  
7    ->    7  
8    ->    8  
9    ->    9  
q    ->    0xa  
w    ->    0xb  
e    ->    0xc  
r    ->    0xd  
t    ->    0xe  
y    ->    0xf 
```
