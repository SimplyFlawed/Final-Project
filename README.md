# Knight's Descent

CS3113 Introduction to Game Programming Final Project

## About

It is a Legend of Zelda-style, top-down RPG game.

## Controls
* WASD/Arrow keys -> Move player
* Space -> Attack
* Enter -> Start game
* Q -> Quit game
* R -> Restart game

## Features
### Title Screen:
* Logo (AI-Generated)
* "Press Enter to Start" text

### Game:
* Looping background music ([Overdose 8-bit Cover](https://youtu.be/JPMpsnd1PJs?si=YQCTVkF3nW2lmrKe) by 二酸化炭素, [Original](https://youtu.be/H08YWE4CIFQ?si=h1BIijthHe7moe5v) by Natori)
* Sound Effects: Attacking, Enemy Death, Player Death, Win, Game Over, Entering Door
* Restart feature
* Win after defeating the Spirit
* Lose after running out of lives
* Entities have a following shadow entity
* Camera doesn't leave the map in both x & y directions
* Lives counter stays at top left
* Always renders text using texture shaders
* Fade-in effect between scenes

### Player:
* Can attack
* Can't attack while walking
* Has a sword entity
* Walking animation
* Attacking animation
* Uses bottom half of the sprite for player-map collision 


### Enemies:
#### Snake
* Horizontal and vertical roaming AI
* Walking animation 

#### Reptile
* Has agro range
* Unique attack pattern: Walk, Rush, Rest
* Walking animation

#### Spirit Boss
* Switches between moving toward and away from the player
* Idle animation
* Hurt animation
* 3 HP
* Delay between hits

### Levels:
* 3 Levels
* Dungeon level uses lighting shaders
* Dungeon and Boss Room have collidable spikes on the ground
* Maps were made using the [Tiled](https://www.mapeditor.org/) program

# Assets Used
* [Tiny Town](https://kenney.nl/assets/tiny-town) by Kenny
* [Tiny Duneon](https://kenney.nl/assets/tiny-dungeon) by Kenny
* [Ninja Adventure](https://pixel-boy.itch.io/ninja-adventure-asset-pack) by Pixel-Boy and AAA
* A random sound effect from Pokemon Emerald