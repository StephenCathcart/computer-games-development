##CSC3222 Gaming Simulations

###Overview
The game uses a custom entity component system instead of deep object hierarchies. 

Components are just bags of data such as position, velocity, value etc. We can then attach these different types of components to an "Entity", which represents something in the game world, such as the player, the enemy, the score etc. 

Lastly the "GameSystems" are used to give the entities logic depending on what components they are made up of. For example, the "RenderSystem" will only be interested in entities that have a "RenderComponent" where as the "PhysicsSystem" will only concern itself with entities that have "Position" and "Velocity" components attached to them.

Data can be shared between systems using an event messaging system. For example, if the InputSystem detects the space bar has been pressed, it will send a KeyMessage containing the active key to all systems which they are then feel free to ignore it or act upon it.

All resources (meshes, shaders, audio, textures, fonts etc) are held and managed by a "GameResourceManager" which all systems share.

The "GameEntityManager" and the "GameSystemManager" manage a vector of entities and systems, respectively. 

The game is fully functional with both a controller and the keyboard.

A list of systems active in this demo:
- InputSystem (Checks for controller and keyboard events)
- LogicSystem (Updates the scoreboard and plays sounds)
- CollisionSystem (Checks for collisions between player and enemies / pickups)
- PhysicsSystem (Updates velocity and position components)
- RenderingSystem (Draws all render entities to screen / displays debug infomation )

###Screenshot


###References
- Menu theme song: Klendathu Drop - Starship Troopers Soundtrack
- Game theme song: Naoki Sato - Sadame
- 3d models: angryfly @ http://www.turbosquid.com
- Menu background image: http://wallpaperzoo.com/space-cat-wallpapers-4437.html
- Game background: http://desktopwallpapersfree.net/planet-space-36000
- All sound effects: www.freesfx.co.uk
- Fonts: http://www.dafont.com/

NOTE: Works with Xbox 360 Controller

###Video
http://youtu.be/Ij78JLLUGVo