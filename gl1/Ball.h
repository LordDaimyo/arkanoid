#pragma once
//by - ���������� "�" � ������
//br - ������ ������ (����� ����������)

class Ball
{
public:
	int life;
	float x, y, r, dx, dy;
	bool active;
	void move();

	Ball(float y_, float br)
	{
		x = 0;
		y = y_;
		r = br;
		dx = 0;
		dy = 0;
		active = false;
		life = 10;
	}
};