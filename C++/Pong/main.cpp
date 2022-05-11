#include "include/raylib.h" 		// Raylib
#include "include/cusfunc.h" 		// My custom Functions
#include <vector> 					// Vectors
#include <iostream> 				// For Debuging

const int width = 700;
const int height = 500;
int timer;
int countdown = 8 * 60;
bool collision;

int playerSpeed = 5;
std::vector<int> playersY;

class Ball
{
public:
	int Size = 7;
	int Speed = 3;
	Vector2 Pos = {width / 2 - (Size / 2), height / 2 - (Size / 2)};
	Vector2 Vel = {rb2n(-1, 1), rb2n(-1, 1)};
};
Ball ball;
std::vector<Ball> balls;

std::vector<int> score;

int main()														// Int main()
{
	// Initialization
    //--------------------------------------------------------------------------------------
	InitWindow(width, height, "Game Name");						// Initialize the window

    SetExitKey(KEY_NULL); 										// Set exit key to Nothing

	for (int i = 0; i < 2; i++)
	{
	playersY.push_back(height / 2 - 25);
	score.push_back(0);
	}
	balls.push_back(ball);
	//--------------------------------------------------------------------------------------

	// Game loop
    while (!WindowShouldClose())
    {
		// Input
		//--------------------------------------------------------------------------------------
		if (IsKeyPressed(KEY_G)) ball.Speed += 1;

		if (IsKeyDown(KEY_W)) playersY[0] -= playerSpeed + ball.Speed;
		if (IsKeyDown(KEY_S)) playersY[0] += playerSpeed + ball.Speed;

		if (IsKeyDown(KEY_UP)) playersY[1] -= playerSpeed + ball.Speed;
		if (IsKeyDown(KEY_DOWN)) playersY[1] += playerSpeed + ball.Speed;
		//--------------------------------------------------------------------------------------

		// Logic
		//--------------------------------------------------------------------------------------
		timer += 1;
		balls.push_back(ball);

		countdown -= 1;
		// Speed up ball
		if (!countdown)
		{
			for (int i = 0; i < balls.size(); i++)
			{
				balls[i].Speed += 1;
			}
			countdown = 1 * 60;
		}

		// Player don't go off the map
		for (int i = 0; i < 2; i++)
		{
			if (playersY[i] + 70 > height) playersY[i] = height - 70;
			if (playersY[i] < 0) playersY[i] = 0;
		}

		// Ball Speed
		for (int i = 0; i < balls.size(); i++)
		{
			balls[i].Pos.x += balls[i].Vel.x * balls[i].Speed;
			balls[i].Pos.y += balls[i].Vel.y * balls[i].Speed;
		}

		// Score
		for (int i = 0; i < balls.size(); i++)
		{
			if (balls[i].Pos.x + balls[i].Size >= width) score[0] += 1;
			if (balls[i].Pos.x - balls[i].Size <= 0) score[1] += 1;
		}

		// Hit wall and roof
		for (int i = 0; i < balls.size(); i++)
		{
			if (balls[i].Pos.x + balls[i].Size >= width || balls[i].Pos.x - balls[i].Size <= 0)
			{
			// Reset when geting points
				balls[i].Vel.x *= -1;
			//ballPos = {float(width / 2 - (ballSize / 2)), float(height / 2 - (ballSize / 2))};
			//ballVel = {float(rb2n(-1, 1)), float(rb2n(-1, 1))};
			//playersY = {height / 2 - 25, height / 2 - 25};
			//ballSpeed = 3;
			//countdown = 8 * 60;
			//timer = 0;
			}
			if (balls[i].Pos.y + balls[i].Size>= height || balls[i].Pos.y - balls[i].Size <= 0) balls[i].Vel.y *= -1;
		}

		// Collision
		for (int i = 0; i < balls.size(); i++)
		{
			collision = false;
			for (int i = 0; i < 2; i++)
			{
				if(CheckCollisionRecs({float(((width - 30) * i) + (20 * !i)), float(playersY[i]), 10, 70},
									{balls[i].Pos.x - balls[i].Size / 2, balls[i].Pos.y - balls[i].Size / 2, float(balls[i].Size), float(balls[i].Size)})) collision = true;
			}

			if (collision) balls[i].Vel.x *= -1;
		}
		//--------------------------------------------------------------------------------------

		// Draw
		//--------------------------------------------------------------------------------------
		BeginDrawing();											// Begin to draw

        	ClearBackground(BLACK);								// Clear screen

			// Draw Players
			DrawRectangle(20, playersY[0], 10, 70, WHITE);
			DrawRectangle(width - 30, playersY[1], 10, 70, WHITE);

			// Draw Ball
			for (int i = 0; i < balls.size(); i++)
				DrawCircle(balls[i].Pos.x, balls[i].Pos.y, balls[i].Size, WHITE);
			
			// Draw Score
			DrawRectangle(width / 2 - 10, 15, 10, 40, BLUE);
			DrawText(FormatText("%i", score[0]), width / 2 - MeasureText(FormatText("%i", score[0]), 40) - 30, 17, 40, BLUE);
			DrawText(FormatText("%i", score[1]), width / 2 + 20, 17, 40, BLUE);
			
			// Extra
			//DrawText(FormatText("%i", ballSpeed), width - 40, 50, 30, WHITE);
			//DrawText(FormatText("%i", timer / 60), 10, 10, 30, WHITE);
			DrawFPS(10, 10);

		EndDrawing();											// Stop to draw
		//--------------------------------------------------------------------------------------
	}

	// De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); 												// Close window and OpenGL context
    //--------------------------------------------------------------------------------------
	return 0; 													// return 0 because C++
}