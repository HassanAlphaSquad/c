/*******************************************************************************************
 *
 *   raylib - classic game: snake
 *
 *   Sample game developed by Ian Eito, Albert Martos and Ramon Santamaria
 *
 *   This game has been created using raylib v1.3 (www.raylib.com)
 *   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
 *
 *   Copyright (c) 2015 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static const int screenWidth = 3300;
static const int screenHeight = 1800;

static int framesCounter = 0;
static bool gameOver = false;
static bool pause = false;

static bool allowMove = false;
static Vector2 offset = {0};
static int counterTail = 0;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);		   // Initialize game
static void UpdateGame(void);	   // Update game (one frame)
static void DrawGame(void);		   // Draw game (one frame)
static void UnloadGame(void);	   // Unload game
static void UpdateDrawFrame(void); // Update and Draw (one frame)

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
	// Initialization (Note windowTitle is unused on Android)
	//---------------------------------------------------------
	InitWindow(screenWidth, screenHeight, "classic game: snake");

	InitGame();

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
	SetTargetFPS(60);
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		// Update and Draw
		//----------------------------------------------------------------------------------
		UpdateDrawFrame();
		//----------------------------------------------------------------------------------
	}
#endif
	// De-Initialization
	//--------------------------------------------------------------------------------------
	UnloadGame(); // Unload loaded data (textures, sounds, models...)

	CloseWindow(); // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}

//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

// Initialize game variables
void InitGame(void)
{
	framesCounter = 0;
	gameOver = false;
	pause = false;

	counterTail = 1;
	allowMove = false;
}

// Update game (one frame)

// Update game (one frame)
void UpdateGame(void)
{
	if (!gameOver)
	{
		if (IsKeyPressed('P'))
			pause = !pause;

		if (!pause)
		{
			framesCounter++;
		}
	}
	else
	{
		if (IsKeyPressed(KEY_ENTER))
		{
			InitGame();
			gameOver = false;
		}
	}
}

float a = 0;

float zoom = 1.0f; 

float mercurySpeed = 37;  // Mercury's speed in km/s (example)
float venusSpeed =47;
float earthSpeed = 31.34;
float marsSpeed = 27.64;
float jupiterSpeed = 25.00 ;
float saturnSpeed = 21.41 ;
float uranusSpeed = 18.54;
float neptuneSpeed = 16.26;

// Draw game (one frame)
void DrawGame(void)
{
    float scroll = GetMouseWheelMove(); 
    zoom += scroll * 0.1f; 
    if (zoom < 0.1f) zoom = 0.1f;
    if (zoom > 5.0f) zoom = 5.0f;
	BeginDrawing();

	ClearBackground(RAYWHITE);

	const float sunRadius = 100;
	const float moonRadius = 50;
	// const float mercuryOrbit = 150;
	// const float venusOrbit = 250;
	// const float earthOrbit = 350;
	// const float marsOrbit = 450;
	// const float jupiterOrbit = 550;

	// const float mercuryOrbit = 150;  // Base value
	// const float venusOrbit = mercuryOrbit * (250.0 / 150.0);  // Relative factor
	// const float earthOrbit = mercuryOrbit * (350.0 / 150.0);  // Relative factor
	// const float marsOrbit = mercuryOrbit * (450.0 / 150.0);   // Relative factor
	// const float jupiterOrbit = mercuryOrbit * (550.0 / 150.0); // Relative factor
	// const float saturnOrbit = jupiterOrbit * (750.0 / 550.0); // Relative factor
	// const float uranusOrbit = saturnOrbit * (1000.0 / 750.0); // Relative factor
	// const float neptuneOrbit = uranusOrbit * (1300.0 / 1000.0); // Relative factor
const float mercuryOrbit = 150;  // Base orbit (arbitrary units)
	// float mercurySpeed = 47.87;  // Mercury's speed in km/s (example)

	const float venusOrbit = mercuryOrbit + 100;  // Relative orbit
	// float venusSpeed = mercurySpeed * sqrt(mercuryOrbit / venusOrbit);

	const float earthOrbit = mercuryOrbit + 200;
	// float earthSpeed = mercurySpeed * sqrt(mercuryOrbit / earthOrbit);

	const float marsOrbit = mercuryOrbit + 300;
	// float marsSpeed = mercurySpeed * sqrt(mercuryOrbit / marsOrbit);

	const float jupiterOrbit = mercuryOrbit + 400;
	// float jupiterSpeed = mercurySpeed * sqrt(mercuryOrbit / jupiterOrbit);

	const float saturnOrbit = jupiterOrbit + 200;
	// float saturnSpeed = mercurySpeed * sqrt(mercuryOrbit / saturnOrbit);

	const float uranusOrbit = saturnOrbit + 250;
	// float uranusSpeed = mercurySpeed * sqrt(mercuryOrbit / uranusOrbit);

	const float neptuneOrbit = uranusOrbit + 300;
	// float neptuneSpeed = mercurySpeed * sqrt(mercuryOrbit / neptuneOrbit);

	

	rlPushMatrix();
	 rlScalef(zoom, zoom, 1.0f);
		rlTranslatef(screenWidth / 2.0f, screenHeight/2.0f,0);
		DrawCircle(0, 0, 80, YELLOW);

		rlPushMatrix();
			rlRotatef(mercurySpeed, 0, 0, 1.0f);
			mercurySpeed+=1.0;
			rlTranslatef(mercuryOrbit, 0, 0);
			DrawCircle(0, 0, 10, BROWN);	
		rlPopMatrix();

		rlPushMatrix();
			rlRotatef(venusSpeed, 0, 0, 1.0f);
			venusSpeed+=2.5;
			rlTranslatef(venusOrbit, 0, 0);
			DrawCircle(0, 0, 18, ORANGE);	
		rlPopMatrix();

		rlPushMatrix();
			rlRotatef(earthSpeed, 0, 0, 1.0f);
			earthSpeed+=1.5;
			rlTranslatef(earthOrbit, 0, 0);
			DrawCircle(0, 0, 13, GREEN);
			rlPushMatrix();
				rlRotatef(a, 0, 0, 1.0f);
				a+=1.0;
				rlTranslatef(moonRadius, 0, 0);
				DrawCircle(0, 0, 5, BLACK);
			rlPopMatrix();	
		rlPopMatrix();

		rlPushMatrix();
			rlRotatef(marsSpeed, 0, 0, 1.0f);
			marsSpeed+=4.0;
			rlTranslatef(marsOrbit, 0, 0);
			DrawCircle(0, 0, 11, RED);	
		rlPopMatrix();

		rlPushMatrix();
			rlRotatef(jupiterSpeed, 0, 0, 1.0f);
			jupiterSpeed+=3.0;
			rlTranslatef(jupiterOrbit, 0, 0);
			DrawCircle(0, 0, 25, GRAY);	
		rlPopMatrix();

		rlPushMatrix();
			rlRotatef(saturnSpeed, 0, 0, 1.0f);
			saturnSpeed+=2.0;
			rlTranslatef(saturnOrbit, 0, 0);
			DrawCircle(0, 0, 25, GRAY);	
		rlPopMatrix();

		rlPushMatrix();
			rlRotatef(uranusSpeed, 0, 0, 1.0f);
			uranusSpeed+=1.0;
			rlTranslatef(uranusOrbit, 0, 0);
			DrawCircle(0, 0, 25, GRAY);	
		rlPopMatrix();

		rlPushMatrix();
			rlRotatef(neptuneSpeed, 0, 0, 1.0f);
			neptuneSpeed+=0.5;
			rlTranslatef(neptuneOrbit, 0, 0);
			DrawCircle(0, 0, 25, GRAY);	
		rlPopMatrix();

	rlPopMatrix();

	if (!gameOver)
	{
		if (pause)
			DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
	}
	else
		DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);

	EndDrawing();
}

// Unload game variables
void UnloadGame(void)
{
	// TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
	UpdateGame();
	DrawGame();
}