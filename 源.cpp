#include <iostream>
#include<Windows.h>
#include <chrono>
#include<time.h>
#include<list>
#include <fstream>
const int delays = 200;
int nScreenWidth = 120;
int nScreenHeight = 30;

void storeScore(int nScore);

struct sSnakeSegment
{
	int x;
	int y;
};

int main()
{
	//����������
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
		screen[i] = L' ';
	HANDLE  hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, NULL, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwbyteswitten = 0;

	//��ѭ��
	while (1)
	{
		std::list< sSnakeSegment> snake = { {60,15},{61,15},{62,15 },{63,15},{64,15},{65,15},{66,15},{67,15},{68,15} };
		int nFoodX = 30;
		int nFoodY = 15;
		int nScore = 0;
		int highScore = 0;
		int SnakeDiraction = 3;
		bool bDead = false;
		bool right = false;
		bool left = false;
		bool rightold = false;
		bool leftold = false;
		while (!bDead)
		{
			//����
			clock_t start_time;
			start_time = clock();
			while ((clock() - start_time) < 0.15 * CLOCKS_PER_SEC)
			{
				right = (0x8000 & GetAsyncKeyState(VK_RIGHT)) != 0;
				left = (0x8000 & GetAsyncKeyState(VK_LEFT)) != 0;

				if (right && !rightold)
				{
					SnakeDiraction++;
					if (SnakeDiraction == 4) SnakeDiraction = 0;
				}
				if (left && !leftold)
				{
					SnakeDiraction--;
					if (SnakeDiraction == -1) SnakeDiraction = 3;
				}
				rightold = right;
				leftold = left;
			}
			
			//�ƶ�
			switch (SnakeDiraction)
			{
			case 0://��
				snake.push_front({ snake.front().x ,snake.front().y - 1 });
				break;
			case 1://��
				snake.push_front({ snake.front().x + 1 ,snake.front().y });
				break;
			case 2://��
				snake.push_front({ snake.front().x ,snake.front().y + 1 });
				break;
			case 3://��
				snake.push_front({ snake.front().x - 1 ,snake.front().y });
				break;
			}

			//��ײ���:
			//�߽�
			if (snake.front().x < 0 || snake.front().x >= nScreenWidth)
				bDead = true;
			if (snake.front().y < 3 || snake.front().y >= nScreenHeight)
				bDead = true;
			//ʳ�
			if (snake.front().x == nFoodX && snake.front().y == nFoodY)
			{
				nScore++;
				while (screen[nFoodY * nScreenWidth + nFoodX] != L' ')//�������food
				{
					nFoodX = rand() % nScreenWidth;
					nFoodY = (rand() % (nScreenHeight - 3)) + 3;
				}
				for (int i = 0; i < 3; i++)
				{
					snake.push_back({ snake.back().x, snake.back().y });
				}
			}
			//���Լ���
			for (std::list<sSnakeSegment>::iterator i = snake.begin(); i != snake.end(); i++)
			{
				if (i != snake.begin() && i->x == snake.front().x && i->y == snake.front().y)
					bDead = true;  //���ͷ�����κ������κ�һ�����غϣ�����(������ͷ����
			}
			snake.pop_back();//����һ��



			for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
				screen[i] = L' ';
			for (int i = 0; i < nScreenWidth; i++)
			{
				screen[i] = L'=';
				screen[2 * nScreenWidth + i] = L'=';
			}
			wsprintf(&screen[nScreenWidth + 5], L" the Snake game of CR7                           score: %d ", nScore);

			for (auto s : snake)
				screen[s.y * nScreenWidth + s.x] = bDead ? L'+' : L'O';//������

			screen[snake.front().y * nScreenWidth + snake.front().x] = bDead ? L'X' : L'Z';//����ͷ

			screen[nFoodY * nScreenWidth + nFoodX] = L'*';//ʳ��

			if (bDead)
			{
				wsprintf(&screen[15 * nScreenWidth + 40], L"    �˹������ո����");
				storeScore(nScore);
			}
			WriteConsoleOutputCharacter(hConsole, screen, nScreenHeight * nScreenWidth, { 0,0 }, &dwbyteswitten);
		}
		while ((0x8000 & GetAsyncKeyState((unsigned char)('\x20'))) == 0);//���¿ո�����ѭ����������ѭ��
	}
	return 0;
}


void storeScore(int nScore)
{
	std::ifstream get;
	int lastscore;
	get.open("score.txt");
	get >> lastscore;
	if (lastscore < nScore)
	{
		std::ofstream out;
		out.open("score.txt");
		out << nScore;
		out.close();
	}
}
