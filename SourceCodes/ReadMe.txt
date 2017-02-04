These sourcecodes are just parts of original projects. I don't want everything 
to be shown (Just now at least).

BallCtrl - C# - Visual Studio - Unity - Hobby project - .apk package included
	Controls the movement of the throwable object in the "Throw" -application build (.apk).
	Key Features:
		CurveMotion()   Detects whether the dragging finger is dragging the object in a curve motion.
				The curve is then applied if such movement occurs,
		2D->3D		Class reads users input touches and processes them in 2D, but as soon as the user lets go,
				The object gets a forward facing velocity calculated from the velocity in the 2D plane.
		Generic 	Everything in this code can be accessed without modifying the code itself, i.e. any part
				of updates can be performed without modifying the existing code.

CameraControl - C# - Visual Studio - Unity - Hobby project - .exe package not included
	Controls the movement of a camera in a 3D space. Purpose is to follow a car
	Key Features:
		Camera's facing direction is calculated from the car's velocity vector
		fixed distance - configurable
		No camera rotation along any other than desired axis (opposed to what just fixed positioning/Rotation in relation to car).

GameBoard - C++ - QtCreator - School - .exe package not included
	Provides a logical solution to a QML counterpart in 2D shooting game.
	Key Features:
		Allows QML UI to be completely independent of the logical grid, in which all the computation happens.
		free to scale solution, generic code allows the board to be at any size (2D)
		Number of enemy-, teammate or head role characters is customizable without affecting the gameboard code.

Controller- C++ - QtCreator - School - .exe package not included
	Allows the project to be done in a M(odel)V(iew)C(ontroller) -manner
	Provides an interface between the QML GUI and the C++ logic (E.g. GameBoard.cpp)
	Key Features:
		Generic, neither the logic, nor the UI effect this component. 
		Allows (When threaded) the QML GUI to be responsive, and lets the processing happen "Behind the curtains"