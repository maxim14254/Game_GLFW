#include <iostream>
#include "GLFW/glfw3.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <map>


#define mapW 3
#define mapH 4

class TCell
{
public:
	int Id;
	int WeigthLeft;
	int WeigthRight;
	int WeigthUp;
	int WeigthDown;
	TCell* Left;
	TCell* Right;
	TCell* Up;
	TCell* Down;
	bool vizit = true;
	float Colors[3];

	TCell(int id) : Id(id)
	{
		Left = NULL;
		Right = NULL;
		Up = NULL;
		Down = NULL;

		WeigthLeft = INT32_MAX;
		WeigthRight = INT32_MAX;
		WeigthUp = INT32_MAX;
		WeigthDown = INT32_MAX;
	}

	void Add(TCell* Right, TCell* Up, int WeigthRight, int WeigthUp)
	{
		Right != NULL ? Right->Left = this : NULL;
		Right != NULL ? Right->WeigthLeft = WeigthRight : INT32_MAX;
		Up != NULL ? Up->Down = this : NULL;
		Up != NULL ? Up->WeigthDown = WeigthUp : INT32_MAX;

		Right != NULL ? this->Right = Right : NULL;
		Right != NULL ? this->WeigthRight = WeigthRight : INT32_MAX;
		Up != NULL ? this->Up = Up : NULL;
		Up != NULL ? this->WeigthUp = WeigthUp : INT32_MAX;
	}
};

struct Way
{
	int SumWeigth;
	std::vector<TCell*> Cells;
	Way()
	{
		SumWeigth = INT32_MAX;
	}
};

int start;
int finish;
int closeCell;
int x;
int y;
bool lose;
std::map <int, TCell*> map;
std::map <int, Way*> map2;
float color = rand() % 2;

void CheсkColor(float(&colors)[3])
{
	if (colors[0] == 0 && colors[1] == 0 && colors[2] == 0)
	{
		colors[0] = 0.6f;
		colors[1] = 0.6f;
		colors[2] = 0.6f;
	}
}

void Go(TCell* cell, TCell* cellGo, int way)
{
	if (cellGo == NULL)
		return;

	auto max = map2[cellGo->Id] == NULL ? map2[cellGo->Id] = new Way() : map2[cellGo->Id];
	map2[cell->Id] == NULL ? map2[cell->Id] = new Way() : map2[cell->Id];
	if (cellGo->vizit && way + map2[cell->Id]->SumWeigth < max->SumWeigth)
	{
		map2[cellGo->Id]->SumWeigth = way + map2[cell->Id]->SumWeigth;
		if (map2[cell->Id]->Cells.size() > 0)
		{
			map2[cellGo->Id]->Cells.clear();
			map2[cellGo->Id]->Cells.insert(map2[cellGo->Id]->Cells.end(), map2[cell->Id]->Cells.begin(), map2[cell->Id]->Cells.end());
		}
		map2[cellGo->Id]->Cells.push_back(cellGo);

		float color;
		for (int i = 0; i < 3; ++i)
		{
			color = rand() % 2;
			cellGo->Colors[i] = color;
		}
		CheсkColor(cellGo->Colors);
	}
}

void Game_New()
{

	for (int a = 0; a < map.size(); ++a)
	{
		delete map[a];
	}

	for (int a = 0; a < map2.size(); ++a)
	{
		delete map2[a];
	}

	srand(time(NULL));

	int i = 1;
	int j = 1;

	TCell* cells = new TCell(i);
	TCell* firstCell = cells;

	map[i] = cells;

	while (i < mapH * mapW)
	{
		++i;
		if (j == mapH)
		{
			TCell* cellRight = map[i] == NULL ? new TCell(i) : map[i];
			cells->Add(cellRight, NULL, rand() % 10 + 1, INT32_MAX);
			cells = cellRight;
		}
		else if (i <= mapH * j)
		{
			TCell* cellRight = map[i] == NULL ? new TCell(i) : map[i];
			TCell* cellUp = map[i + mapW - 1] == NULL ? new TCell(i + mapW - 1) : map[i + mapW - 1];

			i == mapW * j + 1 ? cells->Add(NULL, cellUp, rand() % 10 + 1, rand() % 10 + 1) : cells->Add(cellRight, cellUp, rand() % 10 + 1, rand() % 10 + 1);
			cells = cellRight;

			map[i] = cellRight;
			map[i + mapW - 1] = cellUp;
			i == mapW * j + 1 ? ++j : j;

		}
		else
		{
			TCell* cellUp = new TCell(i + mapW - 1);
			cells->Add(NULL, cellUp, INT32_MAX, rand() % 10 + 1);
			cells = firstCell->Up;
			firstCell = firstCell->Up;
			map[i + mapH - 1] = cellUp;
			++j;
		}
	}

	start = rand() % mapW + 1;
	finish = rand() % (mapH - 1) + (mapH * mapW - mapH + 2);
	//finish = 9;
	TCell* cell = map[start];
	cell->vizit = false;
	map2[start] = new Way();
	map2[start]->SumWeigth = 0;

	for (int i = start; i > 0; --i)
	{
		Go(map[i], map[i]->Left, map[i]->WeigthLeft);
		Go(map[i], map[i]->Up, map[i]->WeigthUp);
	}

	for (int i = start; i <= mapW; ++i)
	{
		Go(map[i], map[i]->Right, map[i]->WeigthRight);
		Go(map[i], map[i]->Up, map[i]->WeigthUp);
	}

	for (i = mapW + 1; i <= map.size(); ++i)
	{
		Go(map[i], map[i]->Down, map[i]->WeigthDown);
		Go(map[i], map[i]->Left, map[i]->WeigthLeft);
		Go(map[i], map[i]->Right, map[i]->WeigthRight);
		Go(map[i], map[i]->Up, map[i]->WeigthUp);
	}

	map2[finish]->Cells.push_back(map[start]);
	float red = rand() % 2;
	float green = rand() % 2;
	float blue = rand() % 2;

	for (i = 0; i < map2[finish]->Cells.size(); ++i)
	{
		map2[finish]->Cells[i]->Colors[0] = red;
		map2[finish]->Cells[i]->Colors[1] = green;
		map2[finish]->Cells[i]->Colors[2] = blue;
		CheсkColor(map2[finish]->Cells[i]->Colors);
	}
}


void ShowField(float red, float green, float blue)
{
	glBegin(GL_TRIANGLE_STRIP);

	glColor3f(red - 0.2f, green - 0.2f, blue - 0.2f); glVertex2f(0, 1);
	glColor3f(red - 0.3f, green - 0.3f, blue - 0.3f); glVertex2f(1, 1); glVertex2f(0, 0);
	glColor3f(red - 0.4f, green - 0.4f, blue - 0.4f); glVertex2f(1, 0);

	glEnd();
}


void Game_Show()
{
	glLoadIdentity();
	glScalef(1.85 / mapW, 1.7 / mapH, 1);
	glTranslated(-mapW * 0.5, -mapH * 0.45, 0);


	for (int y = 0; y < mapH; ++y)
	{
		for (int x = 0; x < mapW; ++x)
		{
			glPushMatrix();
			glTranslated(x, y, 0);

			int index = y * mapW + x + 1;

			ShowField(map[index]->Colors[0], map[index]->Colors[1], map[index]->Colors[2]);

			glPopMatrix();
		}
	}
}


void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	//double y1;
	//double x1;
	//int heigth;
	//int width;
	//glfwGetWindowSize(window, &width, &heigth);
	//glfwGetCursorPos(window, &x1, &y1);

	//y = (heigth - y1) / (heigth / mapH);
	//x = x1 / (width / mapW);

	//if (button == GLFW_MOUSE_BUTTON_LEFT && action == 1 && !map[x][y].flag)
	//{
	//	if (lose || mines == closeCell)
	//		Game_New();
	//	else
	//		OpenNullField(x, y);

	//}
	//else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == 1 && !map[x][y].open && !map[x][y].flag)
	//{
	//	map[x][y].flag = true;
	//}
	//else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == 1 && !map[x][y].open)
	//{
	//	map[x][y].flag = false;
	//}
}

int main()
{
	GLFWwindow* window;
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(800, 800, "Saper", NULL, NULL);
	glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetMouseButtonCallback(window, mouse_callback);

	Game_New();

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		Game_Show();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

