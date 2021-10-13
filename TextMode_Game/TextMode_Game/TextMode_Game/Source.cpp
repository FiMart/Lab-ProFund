#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <thread>

#define screen_x 80
#define screen_y 25

int status[5];
int xb[5], yb[5];

void setcolor(int fg, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}

void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void draw_ship(int x, int y)
{
	gotoxy(x, y);
	setcolor(7, 0);
	printf(" <-0-> ");
}

void erase_ship(int x, int y)
{
	gotoxy(x, y);
	setcolor(0, 0);
	printf("       ");
}

struct bullet
{
	bool status;
	int x, y;
};

struct bullet A[5];

void draw_bullet(int x, int y)
{
	gotoxy(x, y);
	setcolor(7, 0);
	printf("^");
}

void clear_bullet(int x, int y)
{
	gotoxy(x, y);
	setcolor(0, 0);
	printf(" ");
}

void draw_star(int x, int y)
{
	gotoxy(x, y);
	setcolor(7, 0);
	printf("*");
}

void clear_star(int x, int y)
{
	gotoxy(x, y);
	setcolor(0, 0);
	printf(" ");
}

void score(int z, int x, int y)
{
	gotoxy(x, y);
	printf("%d", z);
}

void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}

char cursor(int x, int y)
{
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; 
	COORD c = { x,y };
	DWORD num_read;
	if (!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD) & num_read))
		return '\0';
	else
		return buf[0];
}

HANDLE wHnd;
COORD bufferSize = { screen_x,screen_y };
SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };

int setConsole(int x, int y)
{
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}

int main()
{
	int sc = 0, hit = 0;
	int sx, sy;
	char ch = ' ';
	char direction = ' ';
	int x = 38, y = 20;
	int bx, by, i;
	int bullet = 0;
	setcursor(0);
	//setConsole(screen x, screen y);
	draw_ship(x, y);

	srand(time(NULL));
	for (int i = 1; i <= 20; i++)
	{
		sy = 10 + rand() % 61; /*[10-70] minimun = 10 range = 61*/
		sx = 2 + rand() % 4;   /*[2-5] minimun = 2 range = 4*/
		draw_star(sx, sy);
	}

	do
	{
		if (_kbhit())
		{
			ch = _getch();
			if (ch == 'a')
			{
				direction = 'a';
			}
			if (ch == 'd')
			{
				direction = 'd';
			}
			if (ch == 's')
			{
				direction = 's';
			}
			if (ch == ' ')
			{
				for (int i = 0; i < 5; i++)
				{
					if (A[i].status == false)
					{
						A[i].status = true;
						A[i].x = x + 3;
						A[i].y = y - 1;
						draw_bullet(A[i].x, A[i].y);
						std::thread q(Beep, 360, 1900);
						q.detach();
						break;
					}
				}
			}
			fflush(stdin);
		}
		if (direction == 'a' && x > 0)
		{
			erase_ship(x, y);
			draw_ship(--x, y);
		}
		if (direction == 'd' && x < 73)
		{
			erase_ship(x, y);
			draw_ship(++x, y);
		}
		for (int i = 0; i < 5; i++)
		{
			if (A[i].status == true)
			{
				if (A[i].y == 2)
				{
					A[i].status = false;
					clear_bullet(A[i].x, A[i].y);
				}

				else
				{
					if (cursor(A[i].x, A[i].y - 1) == '*')
					{
						hit = 1;
					}
					clear_bullet(A[i].x, A[i].y);
					draw_bullet(A[i].x, --A[i].y);
				}
				if (hit == 1)
				{
					sc += 1;
					clear_bullet(A[i].x, A[i].y);
					sx = 10 + rand() % 61;
					sy = 2 + rand() % 4;
					draw_star(sx, sy);
					A[i].status = false;
					hit = 0;
					std::thread q(Beep, 400, 100);
					q.detach();
				}
			}
		}
		score(sc, 77, 0);
		Sleep(100);
	} while (ch != 'x');
	return 0;
}