# CSED451

## Introduction
This 3D adaptation of the "Wind Runner" game is developed with C++ and OpenGL Shading Language (GLSL). In this game, players control a character with three motion poses, capable of changing three view modes (a third-person perspective, a third-person orthogonal view, and a first-person perspective view), as well as jumping to collect stars for scoring points while avoiding obstacles like fireballs, holes, and mushrooms. In the current development stage, wireframe rendering is utilized for object visualization.

## How to Play

1. Press "1"  to switch the game perspective to a third-person perspective view.
2. Press "2" to switch the game perspective to a third-person orthogonal view.
3. Press "3"  to switch the game perspective to a first-person perspective view.
4. Press the spacebar to make the character jump over holes or onto higher terrains. Please note that the character can only jump once during a jump.
5. Press the spacebar to make the character have a small jump for either stepping on or jumping over mushroom obstacles when the character is near the mushrooms. If the character passes a mushroom obstacle from the side without a jump, the game will end. 
6. Press the spacebar to make the character jump or keep going to avoid fireballs. If the character touches a fireball, the game will end.
7. Press the spacebar to make the character jump or keep going to collect stars. Each star collected will increase your score by 5 points.
8. End the game by pressing the "q", "Q", or "esc" buttons on your keyboard. 
9. The final score will be displayed on the screen. 
10. Have fun playing the 3D game. 

## Command Keys

| Keyboard | Description      

| :-------- | :------- |  
| `spacebar` | Charater jump |  
| `esc, Q, q,` | Quit game |  

## Environment
C++,
OpenGL 4.6, GLSL
Visual Studio 2019

## Directory layout 
├── bin                   
├── include                    
├── lib                    
├── meshes                 
├── Angel.h                  
├── Game.cpp
├── Game.h
├── InitShader.cpp
├── Model.h
├── Object.cpp
├── Object.h
├── fshader.glsl
├── main.cpp
├── mat.h
├── vec.h
├── vshader.glsl
└── README.md
