#define _CRT_SECURE_NO_WARNINGS

#include<windows.h>
#include<math.h>
#include"Ball.h"
#include"Brick.h"
#include"vars.h"
#include<GL/glut.h>
#include <stdio.h>
#include "gl_tex.h"
#include <time.h>

int start = 0;
float r_x = 50, r_y = 290, r_w = 80, r_h = 8; //размеры доски

Ball ball(r_y, 6.0);
Brick brick[10][15];

int score = 0, level = 1;

unsigned int tex1, tex2;

bool showMenu = true;

void newGame();
void MenuCheck(int v);

//
bool isCollision(Ball& b, Brick& br)
{
	float x = b.x, y = b.y, r = b.r, c = br.col, w = br.w, row = br.row, h = br.h;
	if (br.active)
	{
		//--------------------------//
		float p1 = c * w;
		float p2 = row * h;
		float p3 = w / 2;
		float p4 = h / 2;
		if (fabs(x - p1 - p3 - 2) <= (r + p3) && fabs(y - p2 - p4 - 2) <= (r + p4)) //находится ли блок вблизи шарика, то есть пересекаются ли их границы
		{
			if (sqrt((p1 + p3 - x) * (p1 + p3 - x) + (p2 + p4 - y) * (p2 + p4 - y)) //это для угловых столкновений, тут используется теорема пифагора 
				- (p3 - 1) * sqrt(2.0) - r > r * (sqrt(2.0) - 1)) //корень из двух, так как рамки границ являются квадратными
				return 0;
			else
				return 1;
		}
		//--------------------------//

		/*if (fabs(x - c * w - w / 2 - 2) <= (r + w / 2) && fabs(y - row * h - h / 2 - 2) <= (r + h / 2)) //находится ли блок вблизи шарика, то есть пересекаются ли их границы
		{
			if (sqrt((c * w + w / 2 - x) * (c * w + w / 2 - x) + (row * h + h / 2 - y) * (row * h + h / 2 - y)) //это для угловых столкновений, тут используется теорема пифагора
				- (w / 2 - 1) * sqrt(2.0) - r > r* (sqrt(2.0) - 1)) //корень из двух, так как рамки границ являются квадратными
				return 0;
			else
				return 1;
		}*/
	}
	return 0;
}

void drawQuadTex(int x, int y, float w, float h, float tw, float th)
{
	glBegin(GL_TRIANGLES);

	glTexCoord2f(0, 0);
	glVertex2f(x, y);

	glTexCoord2f(0, th);
	glVertex2f(x, y + h);

	glTexCoord2f(tw, th);
	glVertex2f(x + w, y + h);

	glTexCoord2f(0, 0);
	glVertex2f(x, y);

	glTexCoord2f(tw, th);
	glVertex2f(x + w, y + h);

	glTexCoord2f(tw, 0);
	glVertex2f(x + w, y);

	glEnd();
}

void drawCircle(int x, int y, int r)
{
	glBegin(GL_POLYGON);
	for (float i = 0; i < (2 * 3.14); i += (3.14 / 8))
	{
		glVertex2f(x + r * sin(i), y + r * cos(i));
	}
	glEnd();
}

void drawString(float x, float y, char* string)
{
	glRasterPos2f(x, y);
	for (char* c = string; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (unsigned int)*c);
}

void Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Фон текстутрой
	glBindTexture(GL_TEXTURE_2D, tex1);
	glColor3f(1, 1, 1);

	drawQuadTex(0, 0, 300, 300, 1, 1);

	// Шарик
	glBindTexture(GL_TEXTURE_2D, 0);

	glColor3f(0.0, 0.6, 0.6); //плитка
	glRectf(r_x, r_y, r_x + r_w, r_y + r_h); //рисуем плитку
	glColor3f(0.0, 0.9, 0.0); // цвет шарика
	drawCircle(ball.x, ball.y, ball.r); //рисуем шарик

	// Блоки
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, tex2);

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			Brick& b = brick[i][j];
			if (b.active)
			{
				if (b.bonus)
					glColor3f(0.5, 0.5, 1);
				else
					glColor3f(1, 1, 1);

				drawQuadTex(b.col * b.w + 1, b.row * b.h + 1, b.w - 2, b.h - 2, 1, 1);
			}
		}
	} // */

	// Надписи
	char buf[100];

	glColor3f(0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	sprintf(buf, "Score: %d", score);
	drawString(20, 20, buf);

	sprintf(buf, "Lives: %d", ball.life);
	drawString(200, 20, buf);

	sprintf(buf, "Level: %d", level);
	drawString(200, 50, buf);

	if (ball.life == 0)
	{
		glColor3f(1, 0, 0);
		drawString(100, 80, "GAME OVER");
	}

	// Нарисовать меню
	if (showMenu)
	{
		glBindTexture(GL_TEXTURE_2D, 0);

		glColor3f(0, 0, 1);
		drawQuadTex(50, 50, 200, 50, 0, 0);

		glColor3f(0, 1, 0);
		drawString(100, 90, "NEW GAME");

		glColor3f(0, 0, 1);
		drawQuadTex(50, 150, 200, 50, 0, 0);

		glColor3f(0, 1, 0);
		drawString(100, 190, "QUIT");
	}

	// Показать экран
	glutSwapBuffers();
	// printf( "draw\n" );
}

void MousePress(int button, int state, int x, int y)
{
	if ((button == GLUT_LEFT_BUTTON) && (state == 0))
	{
		/*if ((x < 50) || (250 < x))
			return;
		if (50 <= y)
			if (y <= 100)
				MenuCheck(0);
		if (150 <= y)
			if (y <= 200)
				MenuCheck(1);
		return;*/
		if (showMenu)
		{
			if ((x < 50) || (250 < x))
				return;

			if ((50 <= y) && (y <= 100))
				MenuCheck(0);

			if ((150 <= y) && (y <= 200))
				MenuCheck(1);

			return;
		}

		if (!ball.active)
		{
			if (ball.life == 0)
			{
				showMenu = true;
				return;
			}

			ball.active = true;

			ball.dx = 6;
			ball.dy = -6; //-6 так как движемся вверх, движение по оси оу 
		}
	}
}

void Mouse(int ax, int ay)

{
	if (showMenu) return;
	//-----------------------//
	/*int rw2 = r_w / 2;
	r_x = ax - rw2; //движение мыши по оси ох
	if (r_x >= 220)
		r_x = 220;
	if (r_x <= 0)
		r_x = 0;
	if (!ball.active)
	{
		ball.x = r_x + rw2;
		ball.y = r_y - ball.r;
	}*/
	//-----------------------//
	r_x = ax - r_w / 2; //движение мыши по оси ох
	if (r_x >= 220)
		r_x = 220;
	if (r_x <= 0)
		r_x = 0;
	if (!ball.active)
	{
		ball.x = r_x + r_w / 2;
		ball.y = r_y - ball.r;
	}
}

//
int colvo()
{
	//float time_o;
	//time_t time_o;
	//time_o = clock();
	//-------------------------//
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 15; j++)
			for (int k = 0; k < 15; k++)
				if (brick[i][j].active)
					return 1;
	//-------------------------//
	/*for (int i = 0; i < 10; i++)
		for (int j = 0; j < 15; j++)
			if (brick[i][j].active)
				return 1;*/

				//printf("Colvo time - %.16f sec\n",1000*(clock() - time_o) / CLOCKS_PER_SEC);
	return 0;
}

//
void Timer(int)
{
	//time_t time_o;
	//time_o = clock();
	if (colvo() == 0) //если закончились блоки, то вы победили
	{
		if (level < 3)
		{
			level++;
		}
		newGame();
	}
	else if (ball.active)
	{
		ball.move();
		if (level == 1)
		{
			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 15; j++)
				{
					Brick& b = brick[i][j];
					if (isCollision(ball, b))
					{
						//----------------------------------//
						double f1 = fabs(ball.x - b.col * b.w - b.w / 2);
						double f2 = fabs(ball.y - b.row * b.h - b.h / 2);

						if (f1 < f2) //находится ли мяч сверху или снизу,то мы изменяем вертикальную сторону
						{
							ball.dy *= -1;
						}
						else if (f1 > f2) //находится ли мяч справа или слева,то мы изменяем горизонтальную сторону
						{
							ball.dx *= -1;
						}
						//----------------------------------//
						/*if (fabs(ball.x - b.col * b.w - b.w / 2) < fabs(ball.y - b.row * b.h - b.h / 2)) //находится ли мяч сверху или снизу,то мы изменяем вертикальную сторону
						{
							ball.dy *= -1;
						}
						else if (fabs(ball.x - b.col * b.w - b.w / 2) > fabs(ball.y - b.row * b.h - b.h / 2)) //находится ли мяч справа или слева,то мы изменяем горизонтальную сторону
						{
							ball.dx *= -1;
						}*/
						else //иначе шарик находится с диагональной стороны
						{
							if ((ball.dx > 0) && (ball.x < b.col * b.w + 1))
							{
								ball.dx *= -1;
							}
							else if (ball.x > (b.col + 1) * b.w - 1)
								ball.dx *= -1;
							if ((ball.dy > 0) && (ball.y < b.row * b.h + 1))
							{
								ball.dy *= -1;
							}
							else if (ball.y > (b.row + 1) * b.h - 1)
								ball.dy *= -1;

							/*if (ball.dx > 0)
							{
								if (ball.x < b.col * b.w + 1)
									ball.dx *= -1;
							}
							else if (ball.x > (b.col + 1)* b.w - 1)
									ball.dx *= -1;
							if (ball.dy > 0)
							{
								if (ball.y < b.row * b.h + 1)
									ball.dy *= -1;
							}
							else if (ball.y > (b.row + 1)* b.h - 1)
								ball.dy *= -1;*/
						}
						if (--b.hit == 0)
						{
							b.active = false;
							if (b.bonus == 1)
							{
								score += 50;
								ball.life++;
							}
							else
								score += 10;
						}
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 15; j++)
				{
					Brick& b = brick[i][j];
					if (isCollision(ball, b))
					{
						//----------------------------------//
						double f1 = fabs(ball.x - b.col * b.w - b.w / 2);
						double f2 = fabs(ball.y - b.row * b.h - b.h / 2);

						if (f1 < f2) //находится ли мяч сверху или снизу,то мы изменяем вертикальную сторону
						{
							ball.dy *= -1;
						}
						else if (f1 > f2) //находится ли мяч справа или слева,то мы изменяем горизонтальную сторону
						{
							ball.dx *= -1;
						}
						//----------------------------------//
						/*if (fabs(ball.x - b.col * b.w - b.w / 2) < fabs(ball.y - b.row * b.h - b.h / 2)) //находится ли мяч сверху или снизу,то мы изменяем вертикальную сторону
						{
							ball.dy *= -1;
						}
						else if (fabs(ball.x - b.col * b.w - b.w / 2) > fabs(ball.y - b.row * b.h - b.h / 2)) //находится ли мяч справа или слева,то мы изменяем горизонтальную сторону
						{
							ball.dx *= -1;
						}*/
						else //иначе шарик находится с диагональной стороны
						{
							if ((ball.dx > 0) && (ball.x < b.col * b.w + 1))
							{
								ball.dx *= -1;
							}
							else if (ball.x > (b.col + 1) * b.w - 1)
								ball.dx *= -1;
							if ((ball.dy > 0) && (ball.y < b.row * b.h + 1))
							{
								ball.dy *= -1;
							}
							else if (ball.y > (b.row + 1) * b.h - 1)
								ball.dy *= -1;

							/*if (ball.dx > 0)
							{
								if (ball.x < b.col * b.w + 1)
									ball.dx *= -1;
							}
							else if (ball.x > (b.col + 1)* b.w - 1)
									ball.dx *= -1;
							if (ball.dy > 0)
							{
								if (ball.y < b.row * b.h + 1)
									ball.dy *= -1;
							}
							else if (ball.y > (b.row + 1)* b.h - 1)
								ball.dy *= -1;*/
						}
						if (--b.hit == 0)
						{
							b.active = false;
							if (b.bonus == 1)
							{
								score += 50;
								ball.life++;
							}
							else
								score += 10;
						}
					}
				}
			}
		}
	}

	Draw();
	glutTimerFunc(30, Timer, 0);
	//printf("Timer time - %.16f sec\n", (clock() - time_o) / CLOCKS_PER_SEC);
}

//
void newGame()
{
	//time_t time_o;
	//time_o = clock();

	for (int i = 0; i < 10; i += 1)
		for (int j = 0; j < 15; j += 1)
		{
			brick[i][j].col = j;
			brick[i][j].row = i;
			brick[i][j].bonus = 0;
			brick[i][j].hit = 1;
		}
	//------------------------------------//
	if (level == 1) // level 1
	{
		for (int i = 0; i < 5; i += 1)
			for (int j = 0; j < 15; j += 1)
			{
				brick[i][j].active = (i <= 5 * 1);
			}
	}
	else if (level == 2) //level 2
	{
		for (int i = 0; i < 10; i += 1)
			for (int j = 0; j < 15; j += 1)
			{
				brick[i][j].active = (i <= 5 * 2);
			}
	}
	else // level 3
	{
		for (int i = 0; i < 10; i += 1)
			for (int j = 0; j < 15; j += 1)
			{
				brick[i][j].active = rand() % 100 > 40;
				brick[i][j].bonus = (rand() % 100 > 98) ? 1 : 0;
			}
	}
	//------------------------------------//
	/*if (level <= 2)
	{
		for (int i = 0; i < 10; i += 1)
			for (int j = 0; j < 15; j += 1)
			{
				brick[i][j].active = (i <= 5 * level);
			}
	}
	else // level 3
	{
		for (int i = 0; i < 10; i += 1)
			for (int j = 0; j < 15; j += 1)
			{
				brick[i][j].active = rand() % 100 > 40;
				brick[i][j].bonus = (rand() % 100 > 98) ? 1 : 0;
			}
	}*/

	ball.active = false;
	//printf("newGame time - %.16f sec\n", 1000 * (clock() - time_o) / CLOCKS_PER_SEC);
}

void MenuCheck(int v)
{
	switch (v)
	{
	case 0:
		showMenu = false;
		score = 0;
		level = 1;
		ball.life = 3; //жизни
		newGame();
		break;
	case 2:
		/* if (level < 3)
			level++;
		newGame(); // */

		for (int i = 0; i < 10; i += 1)
			for (int j = 0; j < 15; j += 1)
				brick[i][j].active = false;
		break;
	case 1:
		exit(1);
		break;
	}
}

void MenuInit()
{
	int M = glutCreateMenu(MenuCheck);
	glutSetMenu(M);
	glutAddMenuEntry("New game", 0);
	glutAddMenuEntry("Clear", 2);
	glutAddMenuEntry("Quit", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
} //*/

int main(int argc, char** argv)
{
	//HWND hWnd = GetForegroundWindow();
	//ShowWindow(hWnd, SW_HIDE);//Прячем консоль
	//SW_MAXIMIZE
	HWND hWnd = GetForegroundWindow();
	ShowWindow(hWnd, SW_MAXIMIZE);//Прячем консоль

	// Начальная инициализация
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// Окошки
	glutCreateWindow("Arcanoid");
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(600, 200);

	// Текстуры
	glEnable(GL_TEXTURE_2D);

	tex1 = loadTex(L"forest.bmp");
	tex2 = loadTex(L"brick.bmp");

	// Вызываемые функции
	glutDisplayFunc(Draw);
	glutTimerFunc(30, Timer, 0);
	glutPassiveMotionFunc(Mouse);
	glutMouseFunc(MousePress); // */

	// Меню
	MenuInit();

	// Основная проекция
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0.0, 300.0, 300.0, 0.0, 1.0, 0.0);

	// Шрифты

	newGame();

	glutMainLoop();

	return 0;
}
