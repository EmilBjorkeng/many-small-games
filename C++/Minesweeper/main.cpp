#include "include/raylib.h"
#include <stdlib.h>
#include <vector>
#include <ctime>
#include <sstream>
#include <cstring>
#include <fstream>

using namespace std;

bool fhr = false;
int random(int start, int end) {
	if (!fhr)
	{
		srand(time(0));
		fhr = true;
	}
	int random = start + rand() % (end - start + 1);
	return random;
}
int sti(string str)
{
	char buffer [50];
	strcpy(buffer, str.c_str());

	return atoi(buffer);
}

class Screen
{
public:
    const Color backgroundColor { 255, 255, 255, 255 }; // RGB Alpha
	const Color textColor = DARKGRAY;
	const Color fieldColor { 200, 200, 200, 255 };
	const Color hoverColor { 152, 152, 152, 255 };
	const Color RevealedColor { 170, 170, 180, 255 };

	int mineAmt = 15;

    int width = 10;
    int height = 10;
	const int menu = 25;
    const int squareSize = 50;
    const int spacing = 2;
	const int topbar = 75 + menu;

	int windowW = width * (squareSize + spacing) + spacing;
	int windowH = height * (squareSize + spacing) + spacing + topbar;

	int customWidth = 16;
	int customHeight = 16;
	int customMineAmt = 27;

	const int faceSize = 40;
	int faceX = windowW / 2 - (faceSize / 2);
	const int faceY = 10;

	bool gameStarted = false;
	bool gameOver = false;
	bool victory = false;
	bool options = false;
	int timer = 0;

	bool mlp = false;
	bool mrp = false;

	bool flagOnOff = false;

	// Options
	bool rightClickToPlaceFlag;
	bool numberClick;

	bool easyMode;
	bool normalMode;
	bool hardMode;
	bool customMode;
};
Screen screen;

class Board
{
public:
	int x, y;
	int type = 0; // 0 (Empty), 1 (Mine), 2-9 (Numbers)
	int state = 0; // 0 (nothing), 1 (revealed), 2 (flag)

	int mouseOverlaping(vector<Board> field) {
		for (int i = 0; i < field.size(); i++)
		{				
			for (int h = 0; h < screen.squareSize; h++)
			{
				for (int w = 0; w < screen.squareSize; w++)
				{
					if (GetMouseY() == field.at(i).y + h && GetMouseX() == field.at(i).x + w)
					{
						return i;
					}
				}
			}
		}
		return -1;
	}

	void createBoard(vector<Board>& field, Board &board)
	{
		int a = screen.spacing;
		int b = screen.spacing + screen.topbar;
		int n = 0;

		for (int i = 0; i < screen.width * screen.height; i++)
		{
			n += 1;

			field.push_back(board);
			field.at(i).x = a;
			field.at(i).y = b;

			a += (screen.squareSize + screen.spacing);

			if (n == screen.width)
			{
				n = 0;
				a = screen.spacing;
				b += (screen.squareSize + screen.spacing);
			}
		}
	}
    void createMines(vector<Board>& field, int notX, int notY)
	{
		bool loop = true;
		int r;

		while (loop)
		{
			loop = false;
			r = random(0, field.size() - 1);

			for (int i = 0; i < field.size(); i++)
			{
				if (field.at(i).type != 1) continue;

				// If the is already a mine there
				if (field.at(r).x == field.at(i).x && field.at(r).y == field.at(i).y) loop = true;

				// If its where you clicked
				if (field.at(r).x == notX && field.at(r).y == notY) loop = true;
			}
		}
		field.at(r).type = 1;
	}

	void calculateNumbers(vector<Board>& field)
	{
		int n;

		for (int i = 0; i < field.size(); i++)
		{
			if (field.at(i).type == 1)
			{
				for (int j = 0; j <= 7; j++)
				{
					if (j == 0) n = 1;
					if (j == 1) n = -1;
					if (j == 2) n = screen.width;
					if (j == 3) n = screen.width * -1;

					if (j == 4) n = screen.width + 1;
					if (j == 5) n = screen.width - 1;
					if (j == 6) n = (screen.width + 1) * -1;
					if (j == 7) n = (screen.width - 1) * -1;

					if (i + n < field.size() && i + n >= 0)
					{
						if (field.at(i).y == field.at(i + n).y || j > 1)
						{
							if (field.at(i).y + (screen.squareSize + screen.spacing) == field.at(i + n).y || j < 4 || j > 5)
							{
								if (field.at(i).y - (screen.squareSize + screen.spacing) == field.at(i + n).y || j < 6)
								{
									if(field.at(i + n).type >= 2) field.at(i + n).type += 1;
									if(field.at(i + n).type == 0) field.at(i + n).type = 2;
								}
							}
						}
					}
				}
			}
		}
	}

	void revealNabors(vector<Board>& field, int i)
	{
		int n;

		for (int j = 0; j <= 7; j++)
		{
			if (j == 0) n = 1;
			if (j == 1) n = -1;
			if (j == 2) n = screen.width;
			if (j == 3) n = screen.width * -1;

			if (j == 4) n = screen.width + 1;
			if (j == 5) n = screen.width - 1;
			if (j == 6) n = (screen.width + 1) * -1;
			if (j == 7) n = (screen.width - 1) * -1;

			if (i + n < field.size() && i + n >= 0)
			{
				if (field.at(i).y == field.at(i + n).y || j > 1)
				{
					if (field.at(i).y + (screen.squareSize + screen.spacing) == field.at(i + n).y || j < 4 || j > 5)
					{
						if (field.at(i).y - (screen.squareSize + screen.spacing) == field.at(i + n).y || j < 6)
						{
							if(field.at(i + n).type == 0 && field.at(i + n).state == 0)
							{
								field.at(i + n).state = 1;
								revealNabors(field, i + n);
							}
							if(field.at(i + n).type > 1 && field.at(i + n).state == 0) field.at(i + n).state = 1;
						}
					}
				}
			}
		}
	}

	int flagsNearAmt(vector<Board>& field, int i)
	{
		int n;
		int num = 0;

		for (int j = 0; j <= 7; j++)
		{
			if (j == 0) n = 1;
			if (j == 1) n = -1;
			if (j == 2) n = screen.width;
			if (j == 3) n = screen.width * -1;

			if (j == 4) n = screen.width + 1;
			if (j == 5) n = screen.width - 1;
			if (j == 6) n = (screen.width + 1) * -1;
			if (j == 7) n = (screen.width - 1) * -1;

			if (i + n < field.size() && i + n >= 0)
			{
				if (field.at(i).y == field.at(i + n).y || j > 1)
				{
					if (field.at(i).y + (screen.squareSize + screen.spacing) == field.at(i + n).y || j < 4 || j > 5)
					{
						if (field.at(i).y - (screen.squareSize + screen.spacing) == field.at(i + n).y || j < 6)
						{
							if(field.at(i + n).state == 2) num += 1;
						}
					}
				}
			}
		}
		return num;
	}
	void revealNear(vector<Board>& field, int i)
	{
		int n;
		for (int j = 0; j <= 7; j++)
		{
			if (j == 0) n = 1;
			if (j == 1) n = -1;
			if (j == 2) n = screen.width;
			if (j == 3) n = screen.width * -1;

			if (j == 4) n = screen.width + 1;
			if (j == 5) n = screen.width - 1;
			if (j == 6) n = (screen.width + 1) * -1;
			if (j == 7) n = (screen.width - 1) * -1;

			if (i + n < field.size() && i + n >= 0)
			{
				if (field.at(i).y == field.at(i + n).y || j > 1)
				{
					if (field.at(i).y + (screen.squareSize + screen.spacing) == field.at(i + n).y || j < 4 || j > 5)
					{
						if (field.at(i).y - (screen.squareSize + screen.spacing) == field.at(i + n).y || j < 6)
						{
							if(field.at(i + n).state == 0) field.at(i + n).state = 1;
						}
					}
				}
			}
		}
	}

	int amoutOf(vector<Board> field, int num, bool state)
	{
		int amt = 0;

		if (!state)
		{
			for (int i = 0; i < field.size(); i++)
			{
				if (field.at(i).type == num) amt +=1;
			}
		}
		else
		{
			for (int i = 0; i < field.size(); i++)
			{
				if (field.at(i).state == num) amt +=1;
			}
		}
		return amt;
	}
	void clear(vector<Board>& field)
	{
		for (int i = 0; i < field.size(); i++)
		{
			field.at(i).state = 0;
			field.at(i).type = 0;
		}
	}
};
Board board;
vector<Board> field;

vector<vector<int> > optionsNum; // {x, y, fontsize/height, width, Active}
vector<char *> optionsText;
vector<bool *> optionsVarables;

void addOptions(char* text, int x, int y, int height, int width, bool active, bool &variable)
{
	if (active) variable = true;
	vector<int> v1;
	v1.push_back(x); v1.push_back(y); v1.push_back(height); v1.push_back(width);
	optionsNum.push_back(v1);
	optionsText.push_back(text);
	optionsVarables.push_back(&variable);
	v1.clear();
	v1.resize(0);
}

int main() {
	vector<string> data;

	InitWindow(screen.windowW, screen.windowH, "Minesweeper"); // Width, Height, Title

	SetTargetFPS(60); // 60 FPS
    SetExitKey(KEY_NULL); // Set exit key to Nothing

	// Create board
	board.createBoard(field, board);

	// Load From File
	ifstream ifile;
	ifile.open("data.txt");
	if (ifile) {
		string st;
		int i = 0;

		while (true)
		{
			ifile >> st;
			data.push_back(st);

			if (ifile.eof())
				break;
			i++;
		}
		ifile.close();
	}
	else {
		ofstream File("data.txt");
		File << "Best time (Easy): 0\n";
		File << "Best time (normal): 0\n";
		File << "Best time (hard): 0\n";
		File << "Custom Width: 16\n";
		File << "Custom Height: 16\n";
		File << "Custom Mine Amout: 27\n";
		File << "Right Click for flag: 1\n";
		File << "Click on numbers to reveal: 1\n";
		File.close();

		for (int i = 0; i < 34; i++)
			data.push_back("0");
		
		data.at(14) = "16";
		data.at(17) = "16";
		data.at(21) = "27";
		data.at(26) = "1";
		data.at(32) = "1";
	}

	// Load images (flag)
	Image flag = LoadImage("assets/flag.png"); // Load image (flag)
	ImageResize(&flag, screen.squareSize, screen.squareSize); // Resize

	Texture2D flagtexture = LoadTextureFromImage(flag); // Turn to texure
    UnloadImage(flag); // Load to RAM

	Image wrongflag = LoadImage("assets/flag_wrong.png"); // Load image (flag wrong)
	ImageResize(&wrongflag, screen.squareSize, screen.squareSize); // Resize

	Texture2D wrongflagtexture = LoadTextureFromImage(wrongflag); // Turn to texure
    UnloadImage(wrongflag); // Load to RAM


	// Load images (Face)
	Image face = LoadImage("assets/face.png"); // Load image (face)
	ImageResize(&face, screen.faceSize, screen.faceSize); // Resize

	Texture2D facetexture = LoadTextureFromImage(face); // Turn to texure
    UnloadImage(face); // Load to RAM

	Image deadface = LoadImage("assets/face_dead.png"); // Load image (face dead)
	ImageResize(&deadface, screen.faceSize, screen.faceSize); // Resize

	Texture2D deadfacetexture = LoadTextureFromImage(deadface); // Turn to texure
    UnloadImage(deadface); // Load to RAM

	Image oface = LoadImage("assets/face_o.png"); // Load image (face o)
	ImageResize(&oface, screen.faceSize, screen.faceSize); // Resize

	Texture2D ofacetexture = LoadTextureFromImage(oface); // Turn to texure
    UnloadImage(oface); // Load to RAM

	Image coolface = LoadImage("assets/face_cool.png"); // Load image (face cool)
	ImageResize(&coolface, screen.faceSize, screen.faceSize); // Resize

	Texture2D coolfacetexture = LoadTextureFromImage(coolface); // Turn to texure
    UnloadImage(coolface); // Load to RAM

	// Add options
	addOptions("Easy Mode", 50, 70, 20, 250, true, screen.easyMode);
	addOptions("Normal Mode", 50, 92, 20, 250, false, screen.normalMode);
	addOptions("Hard Mode", 50, 114, 20, 250, false, screen.hardMode);
	addOptions("Custom Mode", 50, 136, 20, 250, false, screen.customMode);

	addOptions("Right click for flag", 50, 192, 20, 250, sti(data.at(26)), screen.rightClickToPlaceFlag);
	addOptions("Click on numbers to reveal", 50, 214, 20, 250, sti(data.at(32)), screen.numberClick);

	screen.customWidth = sti(data.at(14));
	screen.customHeight = sti(data.at(17));
	screen.customMineAmt = sti(data.at(21));
	// Game loop
    while (!WindowShouldClose())
    {
		if (!screen.options)
		{
			if (screen.gameStarted && !screen.gameOver && !screen.victory) screen.timer += 1; // Timer

			// Game Over
			for (int i = 0; i < field.size(); i++)
			{
				if (field.at(i).state == 1 && field.at(i).type == 1)
				{
					screen.gameOver = true;
					for (int j = 0; j < field.size(); j++)
					{
						if (field.at(j).type == 1 && field.at(j).state != 2) field.at(j).state = 1;
					}
				}
			}

			// Victory
			if (!screen.gameOver)
			{
				if (board.amoutOf(field, 1, true) == field.size() - screen.mineAmt )
				{
					screen.victory = true;
					for (int i = 0; i < field.size(); i++)
					{
						if (field.at(i).state == 0) field.at(i).state = 2;
					}
					ofstream File("data.txt");
					// Easy
					if (screen.timer < sti(data.at(3)) || sti(data.at(3)) == 0)
					{
						File << FormatText("Best time (Easy): %i\n", screen.timer);
						File << FormatText("Best time (normal): %i\n", sti(data.at(7)));
						File << FormatText("Best time (hard) %i\n", sti(data.at(11)));
					}
					// Normal
					if (screen.timer < sti(data.at(7)) || sti(data.at(3)) == 0)
					{
						File << FormatText("Best time (Easy): %i\n", sti(data.at(3)));
						File << FormatText("Best time (normal): %i\n", screen.timer);
						File << FormatText("Best time (hard) %i\n", sti(data.at(11)));
					}
					// Hard
					if (screen.timer < sti(data.at(11)) || sti(data.at(3)) == 0)
					{
						File << FormatText("Best time (Easy): %i\n", sti(data.at(3)));
						File << FormatText("Best time (normal): %i\n", sti(data.at(7)));
						File << FormatText("Best time (hard) %i\n", screen.timer);
					}
					// Non
					else
					{
						File << FormatText("Best time (Easy): %i\n", sti(data.at(3)));
						File << FormatText("Best time (normal): %i\n", sti(data.at(7)));
						File << FormatText("Best time (hard) %i\n", sti(data.at(11)));
					}
					File << FormatText("Custom Width: %i\n", sti(data.at(14)));
					File << FormatText("Custom Height: %i\n", sti(data.at(17)));
					File << FormatText("Custom Mine Amout: %i\n", sti(data.at(21)));
					File << FormatText("Right Click for flag: %i\n", sti(data.at(26)));
					File << FormatText("Click on numbers to reveal: %i\n", sti(data.at(32)));
					File.close();
				}
			}
		}

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) screen.mlp = true;
		if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) screen.mrp = true;

		// Mouse Pressed (Left)
		if (screen.mlp && IsMouseButtonUp(MOUSE_LEFT_BUTTON))
		{
			screen.mlp = false;

			if (!screen.options)
			{
				if(!screen.gameOver && ! screen.victory)
				{
					if (board.mouseOverlaping(field) != -1)
					{
						if(screen.rightClickToPlaceFlag || !screen.flagOnOff)
						{
							if (field.at(board.mouseOverlaping(field)).state == 0)
							{
								if (!screen.gameStarted)
								{
									screen.gameStarted = true;

									for (int i = 0; i < screen.mineAmt; i++)
										board.createMines(field, field.at(board.mouseOverlaping(field)).x, field.at(board.mouseOverlaping(field)).y);

									board.calculateNumbers(field);
								}
								if (board.amoutOf(field, 1, false)  - board.amoutOf(field, 2, true) <= 0)
								{
									for (int i = 0; i < field.size(); i++)
									{
										if (field.at(i).state == 0) field.at(i).state = 1;
									}
								}
								else 
								{
									field.at(board.mouseOverlaping(field)).state = 1;

									if (field.at(board.mouseOverlaping(field)).type == 0)
										board.revealNabors(field, board.mouseOverlaping(field));
								}
							}
						}
						// Flag
						else
						{
							if (field.at(board.mouseOverlaping(field)).state == 2) field.at(board.mouseOverlaping(field)).state = 0;
							else if (field.at(board.mouseOverlaping(field)).state == 0) field.at(board.mouseOverlaping(field)).state = 2;
						}
						// click Number
						if (screen.numberClick)
						{
							if (field.at(board.mouseOverlaping(field)).type > 1 && (!screen.flagOnOff || screen.rightClickToPlaceFlag))
							{
								if (board.flagsNearAmt(field, board.mouseOverlaping(field)) >= field.at(board.mouseOverlaping(field)).type - 1)
								{
									board.revealNear(field, board.mouseOverlaping(field));
								}
						}
						}
					}
				}
				// Face (Reset)
				for (int h = 0; h < screen.faceSize; h++)
				{
					for (int w = 0; w < screen.faceSize; w++)
					{
						if (GetMouseY() == screen.faceY + screen.menu + h && GetMouseX() == screen.faceX + w)
						{
							screen.timer = 0;
							board.clear(field);
							screen.gameStarted = false;
							screen.gameOver = false;
							screen.victory = false;
							break;
						}
					}
				}
				// Flag on / off
				if(!screen.rightClickToPlaceFlag)
				{
					for (int h = 0; h < screen.faceSize; h++)
					{
						for (int w = 0; w < screen.faceSize; w++)
						{
							if (GetMouseY() == screen.faceY + screen.menu + h && GetMouseX() == screen.faceX + w + 60)
							{
								if(screen.flagOnOff) screen.flagOnOff = false;
								else screen.flagOnOff = true;
							}
						}
					}
				}
			}
			// Settings
			for (int h = 2; h < screen.menu - 4; h++)
			{
				for (int w = 2; w < 88; w++)
				{
					if (GetMouseY() == h && GetMouseX() == w)
					{
						if (screen.options) screen.options = false;
						else screen.options = true;
						break;
					}
				}
			}
			// Options
			if (screen.options)
			{
				for (int i = 0; i < optionsText.size(); i++)
				{
					for (int h = 0; h < optionsNum.at(i).at(2); h++)
					{
						for (int w = 0; w < optionsNum.at(i).at(3); w++)
						{
							if (GetMouseY() == optionsNum.at(i).at(1) + h && GetMouseX() == optionsNum.at(i).at(0) + w)
							{
								if (*optionsVarables[i] && optionsVarables[i] != &screen.easyMode && optionsVarables[i] != &screen.normalMode && optionsVarables[i] != &screen.hardMode && optionsVarables[i] != &screen.customMode)
									*optionsVarables[i] = false;

								else if (!*optionsVarables[i])
								{
									if (optionsVarables[i] == &screen.easyMode || optionsVarables[i] == &screen.normalMode || optionsVarables[i] == &screen.hardMode || optionsVarables[i] == &screen.customMode)
									{
										for (int j = 0; j < optionsVarables.size(); j++)
										{
											if (optionsVarables[j] == &screen.easyMode || optionsVarables[j] == &screen.normalMode || optionsVarables[j] == &screen.hardMode || optionsVarables[j] == &screen.customMode)
												*optionsVarables[j] = false;
										}
									}
									*optionsVarables[i] = true;
									
									if (screen.easyMode)
									{
										screen.width = 10;
										screen.height = 10;
										screen.windowW = screen.width * (screen.squareSize + screen.spacing) + screen.spacing;
										screen.windowH = screen.height * (screen.squareSize + screen.spacing) + screen.spacing + screen.topbar;
										SetWindowSize(screen.windowW, screen.windowH);

										screen.faceX = screen.windowW / 2 - (screen.faceSize / 2);
										screen.mineAmt = 15;
										screen.timer = 0;
										screen.gameStarted = false;
										screen.gameOver = false;
										screen.victory = false;

										field.clear();
										board.createBoard(field, board);
									}
									if (screen.normalMode)
									{
										screen.width = 15;
										screen.height = 15;
										screen.windowW = screen.width * (screen.squareSize + screen.spacing) + screen.spacing;
										screen.windowH = screen.height * (screen.squareSize + screen.spacing) + screen.spacing + screen.topbar;
										SetWindowSize(screen.windowW, screen.windowH);

										screen.faceX = screen.windowW / 2 - (screen.faceSize / 2);
										screen.mineAmt = 40;
										screen.timer = 0;
										screen.gameStarted = false;
										screen.gameOver = false;
										screen.victory = false;

										field.clear();
										board.createBoard(field, board);
									}
									if (screen.hardMode)
									{
										screen.width = 27;
										screen.height = 17;
										screen.windowW = screen.width * (screen.squareSize + screen.spacing) + screen.spacing;
										screen.windowH = screen.height * (screen.squareSize + screen.spacing) + screen.spacing + screen.topbar;
										SetWindowSize(screen.windowW, screen.windowH);

										screen.faceX = screen.windowW / 2 - (screen.faceSize / 2);
										screen.mineAmt = 90;
										screen.timer = 0;
										screen.gameStarted = false;
										screen.gameOver = false;
										screen.victory = false;

										field.clear();
										board.createBoard(field, board);
									}
									if (screen.customMode)
									{
										screen.width = screen.customWidth;
										screen.height = screen.customHeight;
										screen.windowW = screen.width * (screen.squareSize + screen.spacing) + screen.spacing;
										screen.windowH = screen.height * (screen.squareSize + screen.spacing) + screen.spacing + screen.topbar;
										SetWindowSize(screen.windowW, screen.windowH);
										
										screen.faceX = screen.windowW / 2 - (screen.faceSize / 2);
										screen.mineAmt = screen.customMineAmt;
										screen.timer = 0;
										screen.gameStarted = false;
										screen.gameOver = false;
										screen.victory = false;

										field.clear();
										board.createBoard(field, board);
									}
								}


								data.at(26) = FormatText("%i", screen.rightClickToPlaceFlag);
								data.at(32) = FormatText("%i", screen.numberClick);

								ofstream File("data.txt");
								File << FormatText("Best time (Easy): %i\n", sti(data.at(3)));
								File << FormatText("Best time (normal): %i\n", sti(data.at(7)));
								File << FormatText("Best time (hard): %i\n", sti(data.at(11)));
								File << FormatText("Custom Width: %i\n", sti(data.at(14)));
								File << FormatText("Custom Height: %i\n", sti(data.at(17)));
								File << FormatText("Custom Mine Amout: %i\n", sti(data.at(21)));
								File << FormatText("Right Click for flag: %i\n", sti(data.at(26)));
								File << FormatText("Click on numbers to reveal: %i\n", sti(data.at(32)));
								File.close();
							}
						}
					}
				}
			}
		}
		// Mouse Pressed (Right)
		if (screen.mrp && IsMouseButtonUp(MOUSE_RIGHT_BUTTON) && !screen.gameOver && !screen.victory)
		{
			screen.mrp = false;

			if (!screen.options)
			{
				if (board.mouseOverlaping(field) != -1)
				{
					if (screen.rightClickToPlaceFlag)
					{
						if (field.at(board.mouseOverlaping(field)).state == 2) field.at(board.mouseOverlaping(field)).state = 0;
						else if (field.at(board.mouseOverlaping(field)).state == 0) field.at(board.mouseOverlaping(field)).state = 2;
					}

				}
			}
		}

		// Draw
        BeginDrawing();

        	ClearBackground(WHITE);

			if (!screen.options)
			{
				for (int i = 0; i < field.size(); i++)
				{
					if (field.at(i).state == 0)
					{
						bool hover = false;

						for (int h = 0; h < screen.squareSize; h++)
						{
							for (int w = 0; w < screen.squareSize; w++)
							{
								if (GetMouseY() == field.at(i).y + h && GetMouseX() == field.at(i).x + w)
								{
									hover = true;
									break;
								}
							}
						}
						// Hover
						if (hover) DrawRectangle(field.at(i).x, field.at(i).y, screen.squareSize, screen.squareSize, screen.hoverColor);
						// Not hover
						else DrawRectangle(field.at(i).x, field.at(i).y, screen.squareSize, screen.squareSize, screen.fieldColor);
					}
					// Flag
					else if (field.at(i).state == 2)
					{
						DrawRectangle(field.at(i).x, field.at(i).y, screen.squareSize, screen.squareSize, screen.fieldColor);

						if (field.at(i).type != 1 && screen.gameOver) DrawTexture(wrongflagtexture, field.at(i).x, field.at(i).y, WHITE);
						else DrawTexture(flagtexture, field.at(i).x, field.at(i).y, WHITE);
					}
					// Revealed
					else
					{
						DrawRectangle(field.at(i).x, field.at(i).y, screen.squareSize, screen.squareSize, screen.RevealedColor);

						if (field.at(i).type == 1) DrawRectangle(field.at(i).x, field.at(i).y, screen.squareSize, screen.squareSize, RED);

						if (field.at(i).type >= 2)
						{
							Color numbcolor;
							if (field.at(i).type == 2) numbcolor = BLUE;
							if (field.at(i).type == 3) numbcolor = GREEN;
							if (field.at(i).type == 4) numbcolor = RED;
							if (field.at(i).type == 5) numbcolor = DARKBLUE;
							if (field.at(i).type == 6) numbcolor = { 128, 0, 0, 255 };
							if (field.at(i).type == 7) numbcolor = { 0, 128, 128, 255 };
							if (field.at(i).type == 8) numbcolor = BLACK;
							if (field.at(i).type == 9) numbcolor = YELLOW;

							DrawText(FormatText("%i", field.at(i).type - 1), field.at(i).x + (screen.squareSize / 4), field.at(i).y, screen.squareSize, numbcolor);
						}
					}
				}

				// Timer
				if (screen.timer / 60 < 10)
				{
					DrawText(FormatText("0%i", screen.timer / 60), 10, 10 + screen.menu, 30, screen.textColor);
				}
				else DrawText(FormatText("%i", screen.timer / 60), 10, 10 + screen.menu, 30, screen.textColor);

				// Mines Left
				if (screen.gameStarted)
				{
					if (board.amoutOf(field, 1, false) < 10)
					{
						DrawText(FormatText("0%i", board.amoutOf(field, 1, false)  - board.amoutOf(field, 2, true)), screen.windowW - 40, 10 + screen.menu, 30, screen.textColor);
					}
					else DrawText(FormatText("%i", board.amoutOf(field, 1, false)  - board.amoutOf(field, 2, true)), screen.windowW - 40, 10 + screen.menu, 30, screen.textColor);
				}
				else
				{
					if (screen.mineAmt < 10)
					{
						DrawText(FormatText("0%i", screen.mineAmt  - board.amoutOf(field, 2, true)), screen.windowW - 40, 10 + screen.menu, 30, screen.textColor);
					}
					else DrawText(FormatText("%i", screen.mineAmt  - board.amoutOf(field, 2, true)), screen.windowW - 40, 10 + screen.menu, 30, screen.textColor);
				}

				// Face
				if (screen.gameOver) DrawTexture(deadfacetexture, screen.faceX, screen.faceY + screen.menu, WHITE);
				else if (screen.victory) DrawTexture(coolfacetexture, screen.faceX, screen.faceY + screen.menu, WHITE);
				else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && board.mouseOverlaping(field) != -1 && !screen.victory) DrawTexture(ofacetexture, screen.faceX, screen.faceY + screen.menu, WHITE);
				else DrawTexture(facetexture, screen.faceX, screen.faceY + screen.menu, WHITE);

				// Flag on / off
				if(!screen.rightClickToPlaceFlag)
				{
					if(screen.flagOnOff) DrawTexture(flagtexture, screen.faceX + 60, screen.faceY + screen.menu, WHITE);
					else DrawRectangle(screen.faceX + 60, screen.faceY + screen.menu, screen.faceSize, screen.faceSize, RED);
				}
			}
			// Menu
			DrawText("Settings", 2, 2, screen.menu - 4, screen.textColor);
			
			if (screen.options)
			{
				// titles
				DrawText("Game modes", 50, 50, 20, screen.textColor);
				DrawText("Settings", 50, 170, 20, screen.textColor);

				// Highscores
				DrawText(FormatText("%i", sti(data.at(3)) / 60), 350, 70, 20, screen.textColor); // Easy
				DrawText(FormatText("%i", sti(data.at(7)) / 60), 350, 92, 20, screen.textColor); // Normal
				DrawText(FormatText("%i", sti(data.at(11)) / 60), 350, 114, 20, screen.textColor); // Hard

				for (int i = 0; i < optionsText.size(); i++)
				{
					//DrawRectangle(optionsNum.at(i).at(0), optionsNum.at(i).at(1), optionsNum.at(i).at(3), optionsNum.at(i).at(2), RED);
					if (*optionsVarables.at(i)) DrawText("[*]", optionsNum.at(i).at(0), optionsNum.at(i).at(1), optionsNum.at(i).at(2), screen.textColor);
					else DrawText("[ ]", optionsNum.at(i).at(0), optionsNum.at(i).at(1), optionsNum.at(i).at(2), screen.textColor);

					DrawText(optionsText[i], optionsNum.at(i).at(0) + 33, optionsNum.at(i).at(1), optionsNum.at(i).at(2), screen.textColor);
				}
			}

        EndDrawing();
	}
	CloseWindow();
	return 0;
}