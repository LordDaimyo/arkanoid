#include "stdafx.h"
#include <Windows.h>
#include "gl_tex.h"

unsigned int loadTex(wchar_t* fname, int target)
{
	// звгрузка текстуры
	/*Выравнивание в *.bmp по байту*/
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int tex_id = -1;

	/*Создание идентификатора для текстуры*/
	glGenTextures(1, &tex_id);
	if (tex_id == -1) return -1;

	/*Начало описания свойств текстуры*/
	glBindTexture(GL_TEXTURE_2D, tex_id);


	HBITMAP hBitmap = (HBITMAP)LoadImage(0, fname, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (hBitmap == NULL)
	{
		MessageBox(0, L"Cant load image", L"", 0);
		exit(1);
	}

	BITMAP bm;

	::GetObject(hBitmap, sizeof(bm), &bm);

	/* Omitting error checks for brevity */
	HDC dcBitmap = CreateCompatibleDC(NULL);
	SelectObject(dcBitmap, hBitmap);

	BITMAPINFO bmpInfo;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = bm.bmWidth;
	bmpInfo.bmiHeader.biHeight = -bm.bmHeight;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 24;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biSizeImage = 0;

	COLORREF* pixel = new COLORREF[bm.bmWidth * bm.bmHeight];
	GetDIBits(dcBitmap, hBitmap, 0, bm.bmHeight, pixel, &bmpInfo, DIB_RGB_COLORS);

	glTexImage2D(target,
		0,
		GL_RGB,
		bm.bmWidth, bm.bmHeight, 0,
		GL_BGR_EXT, // because windows bmp
		GL_UNSIGNED_BYTE,
		pixel);

	/*Создание уровней детализации и инициализация текстуры*/
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bm.bmWidth, bm.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixel);

	/*Повтор изображения по параметрическим осям s и t*/
	(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/*Не использовать интерполяцию при выборе точки на текстуре*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	/*Совмещать текстуру и материал объекта*/
	// glTexEnvi (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

	return tex_id;

}
