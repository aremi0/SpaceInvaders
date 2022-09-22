## SpaceInvaders!  
<p>My version of the legendary videogame Space Invaders!</p>
<p><sub>I tried to make it as faithful as possible.</sub></p>

---  

### Developed in C++ with Visual Studio Community 2022 through raylib graphic library.  
- [raylib library 4.2 (OpenGL 3.3)](https://github.com/raysan5/raylib)  
---  

## Description
- [General game rules](#general).
- [Game Views](#views).
- [Gameplay elements](#gameplay).
- [Collision detector](#collision).
- [Enemies AI](#ai).

### <a name="general"></a> General game rules
[Game rules](https://www.classicgaming.cc/classics/space-invaders/play-guide)

### <a name="views"></a> Views
The game was developed following a Views model, each one dedicated to a specific execution status.  
Right now there are 5 different views sequenced in the following order; Logo, Title, Gameplay, Options, Ending, of which, currently, only the
Gameplay and Logo ones are functional and animated.  
The idea behind each screen is:
1. **Logo:** a short animated (skippable) introduction to the *'Menu View'*.
2. **Menu:** will allow to start a new game, to acces settings menu, etc.
3. **Options:** settings menu.
4. **Gameplay:** the very protagonist, which already allows you to play a new game.
5. **Pause:** will be accessible only by the Gameplay View to pause the current game.
6. **Ending:** will appear in case of Game Over event.

### <a name="gameplay"></a> Gameplay elements
The game is composed of several elements:
- **Player:** an object in the form of a Tank that will allow the user to interact (play) with the game.
- **Enemy:** a grid of 11x5 of three type of alien (Squid, Crab and Octopus) plus a Boss (UFO, that will appear randomly).
- **Bullet:** an object used by player and enemies to destroy each other.  
    There are four types of bullet; *player bullet* and three types of enemies bullet: *slower*, *faster*, *powerful*.
    Currently in a frame are allowed (through a settable control mechanism) to be at most 2 player's bullet and 3 enemies bullet.
- **Bunker:** in every game, as in the original game, there will be three destructible bunker, that is an object composed of 8 different slices, each one with it's own graphic, animation and resistence to the bullet.

### <a name="collision"></a> Collision detector
There is collision detection between:
1. Player with enemies bullets.
2. Enemies with player bullets.
3. Player bullets with roof of gameplay window.
4. Enemies bullets with floor of gameplay window.
5. Player bullets with enemies bullets.
6. Player bullets with bunker.
7. Enemies bullets with bunker.

### <a name="ai"></a> Enemies AI
The enemies will act as a team (the 11x5 grid) and not indiviually with 3 different attack strategy, based of the player status: 
All these following evaluation will be executed each 10 frames (time to see if player is moving or not).
Enemies will shot their bullet with a settable rateo of fire.

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
  
  
### Gallery
<img src="https://i.postimg.cc/CxmNxdDd/1.png" alt="drawing" width="400"/>