## SpaceInvaders!  
  
My version of the legendary videogame Space Invaders!  
<sub>I tried to make it as faithful as possible.</sub>  
---  

### Developed in C++ with Visual Studio Community 2022 through raylib graphic library.  
- [raylib library 4.2 (OpenGL 3.3)](https://github.com/raysan5/raylib)  
---  

## Description
- General game's rules.
- Game's views.
- Gameplay's elements.
- Collisions detector.
- Enemies AI.

### general game's rules
[Game's rules](https://www.classicgaming.cc/classics/space-invaders/play-guide)

### Views
The game was developed following a Views model, each one dedicated to a specific execution status.  
Right now there are 5 different views sequenced in the following order; Logo, Title, Gameplay, Options, Ending, of which, currently, only the
Gameplay and Logo ones are functional and animated.  
The idea behind each screen is:
1. *Logo:* a short animated (skippable) introduction to the Title View.
2. *Title:* that will be renamed as MainMenu and will allow to start a new game, change settings, ...
3. *Options:* settings menu.
4. *Gameplay:* the very protagonist, which already allows you to play.
5. *Pause:* that will accessible only by the Gameplay View and will be added later.
6. *Ending:* the gameover view.

### gameplay's elements  
The gameplay view is composed of several elements:
- Player: an object that will allow to the user to interact and play the game.
- Enemy: a grid of 11x5 alien plus a Boss that will appear randomly in upper section of the window.
- Bullet: an object can be shotted by the player and all the enemies in the lower row of their grid. There 4 types of bullet; player's bullet on it's own and
3 types of enemies bullet: slower, faster, powerful. in each frame there are allowed to be at most 2 player's bullet and 3 enemies bullet (modifiable through a constant).
- Bunker: in each start game there will be 3 bunker, that is an object composed of 8 different slices, each one with it's own graphic and resistence to the bullet.

### collision detector  
There is collision between:
1. player with enemies's bullets.
2. enemies with player's bullets.
3. player's bullets with roof of the gameplay's window.
4. enemies's bullets with the floor of the gameplay's window.
5. player's bullets with enemies's bullets.
6. player's bllets with bunker.
7. enemies's bullets with bunker.

### enemies AI
The enemies will act as a team (the 11x5 grid) and not indiviually with 3 different attack strategy, based of the player status: 
All these following evaluation will be executed each 10 frames (time to see if player is moving or not).
#### Strategy 1: static_AI  
If currently the player is still in the same position will be triggered this stragety. If the player is in the fire range of one column of the enemies grid so
the enemy in the bottom of the column will be selected to shot a _random_ bullet type (faster, slower, powerful).
If is currently not in range will be returned 0;  

#### Strategy 2: predictive_AI
Otherwise, if the player is currently moving his direction will be stored and this strategy will be triggered.  
Starting from the player current position, his movement speed and his movement direction we are able to calculate the position quite accurately of the player
in the time taken by a faster type bullet of the bottom row of the enemy in the column which the player will be in fire range.
If, on the other hand, the player at that time will be out of the fire range of the entire grid will be returned 0.  

#### Strategy 3: "let's wait"_AI
If 0 was returned by the 2 strategies above that means the player is currently out of any fire range so in the meanwhile will be selected an enemy
to shot the powerful bullet at one random player's bunker.  
  
  
  
  
TODO update and corrent this readme.  