#include "include/raylib.h" 		// Raylib
#include "include/cusfunc.h" 		// My custom Functions
#include <vector> 					// Vectors
#include <iostream> 				// For Debuging

const int width = 700;
const int height = 500;

float y = 200;
float vel = 0;

int points = 0;
int obsSpawnRate = 300;


Texture2D frame;
Color bg = WHITE;
Color text = BLACK;
int timer = 0; 
int scroll = 0;
int spawnTimer = 0;
int walkframe = 2;
int scrollSpeed = 4;
bool isOnWayDown = false;
bool Collition = false;
bool night = false;
bool game_over = false;
int groundOffset = 0;
int groundCopyOffset;

class Obsticle
{
public:
	Vector2 pos;
	int type;
};
Obsticle obsticle;
std::vector<Obsticle> obs;

Texture2D makeFrame(Image sprite, Rectangle crop)
{
	Image img = ImageCopy(sprite); 								// Copy sprite to idle
	ImageCrop(&img, crop); 										// Crop idle

	Texture2D frame = LoadTextureFromImage(img); 				// Turn Image into texture
    UnloadImage(img); 											// Unload image

	return frame;  												// return the texture
}
void unGameOver(std::vector<Obsticle> &obs, Texture2D ground)
{
	obs.clear();
	game_over = false;
	y = 200;
	vel = 0;
	points = 0;
	obsSpawnRate = 300;
	bg = WHITE;
	text = BLACK;
	timer = 0; 
	scroll = 0;
	spawnTimer = 0;
	scrollSpeed = 4;
	isOnWayDown = false;
	Collition = false;
	night = false;
	groundOffset = 0;
	groundCopyOffset = ground.width;
}

int main()														// Int main()
{
	// Initialization
    //--------------------------------------------------------------------------------------
	InitWindow(width, height, "Dino Game");						// Initialize the window

	SetTargetFPS(60); 											// 60 FPS
    SetExitKey(KEY_NULL); 										// Set exit key to Nothing
	//--------------------------------------------------------------------------------------

	// Load images
	//--------------------------------------------------------------------------------------
	Image sprite = LoadImage("resources/sprite.png"); 			// Load Sprite Sheet

	// Player
	Texture2D idleFrame = makeFrame(sprite, {1, 1, 44, 47}); 	// Make Idle Frame
	Texture2D walkFrame1 = makeFrame(sprite, {46, 1, 44, 47}); 	// Make Walk Frame 1
	Texture2D walkFrame2 = makeFrame(sprite, {91, 1, 44, 47}); 	// Make Walk Frame 2
	Texture2D crouchFrame1 = makeFrame(sprite, {136, 1, 59, 30}); // Make Crouch Frame 1
	Texture2D crouchFrame2 = makeFrame(sprite, {196, 1, 59, 30}); // Make Crouch Frame 2
	Texture2D deadFrame = makeFrame(sprite, {256, 1, 44, 47}); 	// Make Dead Frame

	// Cactus
	std::vector<Texture2D> obsticles;							// obsticles Vector

	obsticles.push_back(makeFrame(sprite, {301, 1, 17, 36})); 	// Make Cactus 1 (Small)
	obsticles.push_back(makeFrame(sprite, {319, 1, 17, 36})); 	// Make Cactus 2 (Small)
	obsticles.push_back(makeFrame(sprite, {337, 1, 17, 36})); 	// Make Cactus 3 (Small)
	obsticles.push_back(makeFrame(sprite, {355, 1, 17, 36})); 	// Make Cactus 4 (Small)
	obsticles.push_back(makeFrame(sprite, {373, 1, 17, 36})); 	// Make Cactus 5 (Small)
	obsticles.push_back(makeFrame(sprite, {391, 1, 17, 36})); 	// Make Cactus 6 (Small)
	obsticles.push_back(makeFrame(sprite, {409, 1, 25, 49})); 	// Make Cactus 7 (Big)
	obsticles.push_back(makeFrame(sprite, {435, 1, 24, 49})); 	// Make Cactus 8 (Big)
	obsticles.push_back(makeFrame(sprite, {460, 1, 25, 49})); 	// Make Cactus 9 (Big)
	obsticles.push_back(makeFrame(sprite, {486, 1, 24, 49})); 	// Make Cactus 10 (Big)
	obsticles.push_back(makeFrame(sprite, {511, 1, 51, 49})); 	// Make Cactus 11 (Giant)

	// Birds
	obsticles.push_back(makeFrame(sprite, {563, 1, 46, 40}));	// Make Bird Frame 1
	obsticles.push_back(makeFrame(sprite, {610, 1, 46, 40})); 	// Make Bird Frame 2

	// Extra
	Texture2D ground = makeFrame(sprite, {1, 51, 1200, 15}); 	// Make Ground
	Texture2D restart = makeFrame(sprite, {657, 1, 36, 32}); 	// Make Restart button

	UnloadImage(sprite); 										// Unload Sprite Sheet
	//--------------------------------------------------------------------------------------

	groundCopyOffset = ground.width;

	// Game loop
    while (!WindowShouldClose())
    {
		// Input
		//--------------------------------------------------------------------------------------
		if (IsKeyDown(KEY_DOWN))								// Press DOWN
		{
			if (walkframe == 2)	walkframe = 0;
			if (game_over) unGameOver(obs, ground);
			if (!isOnWayDown)
			{
				vel -= 10;
				isOnWayDown = true;
			}
		}
		else if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP))	// Press SPACE or UP
		{
			if (walkframe == 2) walkframe = 0;
			if (game_over) unGameOver(obs, ground);
			if (y == 200)
			{
				vel = 10;
			}
		}
		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && game_over)
		{
			for (int h = 0; h < restart.height * 1.5; h++)
			{
				for (int w = 0; w < restart.width * 1.5; w++)
				{
					if(GetMouseX() == width / 2 - restart.width * 1.5 / 2 + w && GetMouseY() == height / 2 - restart.height * 1.5 / 2 + h)
					{
						unGameOver(obs, ground);
					}
				}
			}
		}
		//--------------------------------------------------------------------------------------

		// Logic
		//--------------------------------------------------------------------------------------
		if (walkframe != 2 && !game_over)
		{
			timer++; 											// Timer
			points = -scroll / 20; 								// Points based on scroll length

			if (timer % 300 + 1 == 1) scrollSpeed++; 			// Increese scroll speed

			// Spawn obsticles
			spawnTimer += scrollSpeed;
			if (spawnTimer > obsSpawnRate)
			{
				spawnTimer = 0;
				obsSpawnRate += 5;
				int rand;
				obs.push_back(obsticle);

				if (timer > 3000) rand = random(0, 12);
				else rand = random(0, 10);

				obs[obs.size() - 1].type = rand;
				obs[obs.size() - 1].pos.x = width;
				obs[obs.size() - 1].pos.y = 250 - obsticles[rand].height;

				if (rand > 10)
				{
					obs[obs.size() - 1].pos.y -= random(0, 2) * obsticles[rand].height;
				}
			}

			// Scroll and delete obsticles
			for (int i = 0; i < obs.size(); i++)
			{
				obs[i].pos.x -= scrollSpeed;
				if (obs[i].pos.x < -60) obs.erase(obs.begin());
			}

			scroll -= scrollSpeed;								// Scroll

			if (timer % 7 == 1 && walkframe < 2) walkframe = !walkframe; // Move Animation

			// Gravity
			y -= vel;
			if (y < 200) vel -= 0.5;
			else
			{
				vel = 0;
				y = 200;
				isOnWayDown = false;
			}

			// Scroll Ground
			if (-scroll > ground.width + groundOffset) groundOffset += ground.width * 2;
			if (-scroll > ground.width + groundCopyOffset) groundCopyOffset += ground.width * 2;

			if (timer % 1000 + 1 == 1)
			{
				if (night)
				{
					bg = BLACK;
					text = WHITE;
				}
				else
				{
					bg = WHITE;
					text = BLACK;
				}
				
				night = !night;
			}
		}
		//--------------------------------------------------------------------------------------

		// Collition
		//--------------------------------------------------------------------------------------
		if (!game_over)
		{
			Collition = false;
			for (int i = 0; i < obs.size(); i++)
			{
				int num = 0;
				if (IsKeyDown(KEY_DOWN) && y == 200) num = 18;

				if (isColliding({20, y + num, float(idleFrame.width), float(idleFrame.height)}, 
								{obs[i].pos.x, obs[i].pos.y, float(obsticles[obs[i].type].width), float(obsticles[obs[i].type].height)}))
									Collition = true;
			}
			if (Collition)
			{
				game_over = true;
			}
		}
		//--------------------------------------------------------------------------------------

		// Draw
		//--------------------------------------------------------------------------------------
		BeginDrawing();											// Begin to draw

        	ClearBackground(bg); 								// Clear screen

			// Ground
			DrawTexture(ground, scroll + groundOffset, 240, WHITE);
			DrawTexture(ground, scroll + groundCopyOffset, 240, WHITE);

			// Obsticle
			for (int i = 0; i < obs.size(); i++)
			{
				if (obs[i].type > 10) 
				{
					if (walkframe) DrawTexture(obsticles[11], obs[i].pos.x, obs[i].pos.y, WHITE);
					else DrawTexture(obsticles[12], obs[i].pos.x, obs[i].pos.y, WHITE);
				}
				else DrawTexture(obsticles[obs[i].type], obs[i].pos.x, obs[i].pos.y, WHITE);
			}

			// Player
			if (walkframe == 2) frame = idleFrame;
			else if (game_over) frame = deadFrame;
			else
			{
				if (IsKeyDown(KEY_DOWN) && y == 200)
				{
					if (walkframe) frame = crouchFrame1;
					else frame = crouchFrame2;
				}
				else
				{
					if (walkframe) frame = walkFrame1;
					else frame = walkFrame2;
				}
			}
			if (IsKeyDown(KEY_DOWN) && y == 200 && !game_over) DrawTexture(frame, 20, y + 18, WHITE);
			else DrawTexture(frame, 20, y, WHITE);
			
			// Extra
			DrawFPS(10, 10);
			DrawText(itc(points), width - MeasureText(itc(points), 20) - 10, 10, 20, text);

			// Restart button
			if (game_over)
			{
				DrawTextureEx(restart, {float(width / 2 - restart.width * 1.5 / 2), float(height / 2 - restart.height * 1.5 / 2)}, 0.0f, 1.5f, WHITE);
			}

		EndDrawing();											// Stop to draw
		//--------------------------------------------------------------------------------------
	}

	// De-Initialization
    //--------------------------------------------------------------------------------------

	// Unload textures
	UnloadTexture(idleFrame);
	UnloadTexture(walkFrame1);
	UnloadTexture(walkFrame2);
	UnloadTexture(crouchFrame1);
	UnloadTexture(crouchFrame2);
	UnloadTexture(deadFrame);
	UnloadTexture(ground);
	UnloadTexture(restart);

	// Unload Cactus textures from vector
	for (int i = 0; i < obsticles.size(); i++)
	{
		UnloadTexture(obsticles[i]);
	}
	obsticles.clear();

    CloseWindow(); 												// Close window and OpenGL context
    //--------------------------------------------------------------------------------------
	return 0; 													// return 0 because C++
}