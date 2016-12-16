CprE 557 Final
Jonathan Schlueter


I originally set out to create a solar system simulation directly in OpenGL.

Once this basic simulation was complete, I wanted to add a spaceship that the user could fly, and destroy asteroids.

In addition, I wanted to compare the functionality and ease of use of OpenGL and Unity3d. To that end, I decided to recreate the
solar system in Unity3d, and further develop it.

The features I added to the Unity3d simulation were a flyable spaceship and asteroids.

The user can fly the spaceship around the Unity scene, and destroy asteroids with fired rockets.
Planets can also be fired at, but will not be destroyed.

The Y-position of the spaceship is locked. This enables the spaceship to always remain in the same plane as the planets.

--Controls--
OpenGL:
'p':			Pause
't':			Toggle To-Scale mode (easily viewable solar system)
'r':			Toggle local planetary rotation
'+/-':			Increase/Decrease simulation speed
L/R/ Mouse: 	Rotate/Zoom the scene

Unity3d:
'W/A/S/D': 	Fly the spaceship through the scene
Space:		Fire rocket at asteroids and planets