#include "include/raylib.h" 		// Raylib
#include "include/cusfunc.h" 		// My custom Functions
#include <vector> 					// Vectors
#include <iostream> 				// For Debuging

int width = 7;
int height = 6;
const int holeSize = 60;
const int padding = 75;
const int spacing = 5;
const int erp = 100;

int winner = 0;

const int initWidth = width * (holeSize + spacing) + padding * 2 + erp;
const int initHeight = height * (holeSize + spacing) + padding * 2;

std::vector<int> boardStarte;

int checkForNabors(int i, int type, int way)
{
	int n;
	for (int j = 5; j <= 8; j++)
	{
		if (j == 5) n = 1;
		else n = j;
		for (int k = 0; k <= 1; k++)
		{
			if (k) n *= -1;

			if (i + n < boardStarte.size() && i + n >= 0)
			{
				if (boardStarte.at(i + n) == type)
				{
					if (way == n || way == 0)
						return checkForNabors(i + n, type, n) + 1;
				}
			}
		}
	}
	return 0;
}

int main()														// Int main()
{
	// Initialization
    //--------------------------------------------------------------------------------------
	InitWindow(initWidth, initHeight, "Four in a row");			// Initialize the window

	SetTargetFPS(60); 											// 60 FPS
    SetExitKey(KEY_NULL); 										// Set exit key to Nothing

	// Board starte (Vector)
	for (int i = 0; i < height * width; i++)
	{
		boardStarte.push_back(0);
	}
	//--------------------------------------------------------------------------------------

	int x, y, at;
	int turn = 0;
	Rectangle rect;

	// Game loop
    while (!WindowShouldClose())								// Detect window close
    {
		// Input
		//--------------------------------------------------------------------------------------
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))			// Left Click
		{
			if (!winner)
			{
				x = padding + (holeSize + spacing) / 2;
				y = padding + (holeSize + spacing) / 2;
				at = 0;

				for (int h = 0; h < height; h++)
				{	
					for (int w = 0; w < width; w++)
					{
						for (int i = 0; i < holeSize; i++)
						{
							for (int j = 0; j < holeSize; j++)
							{
								if (GetMousePosition().x == w * (holeSize + spacing) + padding + i && 
									GetMousePosition().y == h * (holeSize + spacing) + padding + j)
								{
									int num = 0;
									bool placed = false;
									for (int n = 0; n < height; n++)
									{
										if (boardStarte.at(at % 7 + num) == 1 || boardStarte.at(at % 7 + num) == 2)
										{
											if (at % 7 + num - 7 >= 0)
											{
												boardStarte.at(at % 7 + num - 7) = turn + 1;
												turn = !turn;
											}
											placed = true;
											break;
										}
										num += 7;
									}
									if (!placed)
									{
										boardStarte.at(at % 7 + height * width - 7) = turn + 1;
										turn = !turn;
									}
									break;
								}
							}
						}

						x += holeSize + spacing;
						at++;
					}
					y += holeSize + spacing;
					x = padding + (holeSize + spacing) / 2;
				}
			}
			// Reset button
			for (int h = 0; h < 50; h++)
			{
				for (int w = 0; w < 50; w++)
				{
					if (GetMousePosition().x == GetScreenWidth() - 75 + w &&
					    GetMousePosition().y == 25 + h)
					{
						for (int i = 0; i < height * width; i++)
						{
							boardStarte[i] = 0; // Clear Board
							turn = 0;
							winner = 0;
						}
					}
				}
			}
		}
		//--------------------------------------------------------------------------------------

		// Logic
		//--------------------------------------------------------------------------------------
		for (int i = 0; i < width * height; i++)
		{

			if (checkForNabors(i, 1, 0) >= 4) winner = 1;
			if (checkForNabors(i, 2, 0) >= 4) winner = 2;
		}
		//--------------------------------------------------------------------------------------

		// Draw
		//--------------------------------------------------------------------------------------
		BeginDrawing();											// Begin to draw

        	ClearBackground(WHITE);								// Clear screen

			// Board
			DrawRectangle(padding - 5, padding- 5, width * (holeSize + spacing) + 10, height * (holeSize + spacing) + 10, BLUE); // Board (with no holes)

			x = padding + (holeSize + spacing) / 2;
			y = padding + (holeSize + spacing) / 2;
			at = 0;

			for (int h = 0; h < height; h++)
			{	
				for (int w = 0; w < width; w++)
				{
					if (boardStarte[at] == 1) DrawCircle(x, y, holeSize / 2, RED); // Player 1 colour
					else if (boardStarte[at] == 2) DrawCircle(x, y, holeSize / 2, YELLOW); // Player 2 colour
					else DrawCircle(x, y, holeSize / 2, WHITE); // should be same colour as background

					x += holeSize + spacing;
					at++;
				}
				y += holeSize + spacing;
				x = padding + (holeSize + spacing) / 2;
			}

			// Reset Buttons
			rect = {float(GetScreenWidth() - 75), 25, 50, 50};
			DrawRectangleRounded(rect, 0.1F, 5, BLUE);
			DrawCircleSector({float(GetScreenWidth() - 50), 50}, 16, 270, 540, 20, BLACK);
			DrawCircleSector({float(GetScreenWidth() - 50), 50}, 12, 270, 540, 20, BLUE);
			DrawPoly({float(GetScreenWidth() - 50), 37}, 3, 8, 90, BLACK);

			// Extra
			std::string winnertext = " is the winner!";
			if (winner == 1) winnertext = "Player 1" + winnertext;
			if (winner == 2) winnertext = "Player 2" + winnertext;
			if (winnertext.length() > 15) DrawText(stc(winnertext), GetScreenWidth() / 2 - MeasureText(stc(winnertext), 20) / 2, 20, 20, BLACK);

			Color psturn;
			std::string text = "Player";

			if (turn)
			{
				psturn = YELLOW;
				text = text + " 2 Turn";
			}
			else
			{
				psturn = RED;
				text = text + " 1 Turn";
			}

			rect = {float(GetScreenWidth() - 150), float(GetScreenHeight() - 145), 137, 75};
			DrawRectangleRounded(rect, 0.1F, 5, BLUE);
			DrawCircle(GetScreenWidth() - 79, GetScreenHeight() - 97, 18, psturn);
			DrawText(stc(text), GetScreenWidth() - 145, GetScreenHeight() - 140, 19, WHITE);

			DrawFPS(10, 10);									// FPS counter

		EndDrawing();											// Stop to draw
		//--------------------------------------------------------------------------------------
	}

	// De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); 												// Close window and OpenGL context
    //--------------------------------------------------------------------------------------
	return 0; 													// return 0 because C/C++
}