#include "pch.h"

#include "Image.h"
#include<vector>
#include<cmath>
#include<iostream>

IMAGE Image::read(wstring fileName, int& width, int& height)
{
	CImage image;
	auto result = image.Load(fileName.c_str());
	if (result == E_FAIL) {
		cout << "文件不存在" << endl;
		exit(-1);
	}
	height = image.GetHeight();
	width = image.GetWidth();
	IMAGE arr = create(width, height);

	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++) {
			COLORREF col = image.GetPixel(x, y);
			arr[x][y][0] = GetRValue(col);
			arr[x][y][1]= GetGValue(col);
			arr[x][y][2] = GetBValue(col);
		}
	rgb2lab(arr, width, height);
	return arr;
}

IMAGE Image::create(int width, int height)
{
	IMAGE image = new Pixel*[width];
	for (int i = 0; i < width; i++) {
		image[i] = new Pixel[height];
	}
	return image;
}

IMAGE Image::copy(IMAGE image, int width, int height)
{
	IMAGE newImage = create(width, height);
	for (int i = 0; i < width; i++) {
		memcpy(newImage[i], image[i], height * sizeof(Pixel));
	}
	return newImage;
}

void Image::save(wstring fileName, IMAGE arr, int width, int height)
{
	CImage image;
	image.Create(width, height, 32);
	lab2rgb(arr, width, height);

	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++) {
			image.SetPixelRGB(x, y, arr[x][y][0], arr[x][y][1], arr[x][y][2]);
		}
	image.Save(fileName.c_str());
}

void Image::rgb2lab(IMAGE image, int width, int height)
{
	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++) {
			rgb2lab(image[x][y], image[x][y]);
		}
}

void Image::lab2rgb(IMAGE image, int width, int height)
{
	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
			lab2rgb(image[x][y], image[x][y]);
}

// the following functions are based off of the pseudocode
// found on www.easyrgb.com
void Image::lab2rgb(Pixel& lab, Pixel& rgb) {
	float y = (lab[0] + 16) / 116,
		x = lab[1] / 500 + y,
		z = y - lab[2] / 200,
		r, g, b;

	x = 0.95047 * ((x * x * x > 0.008856) ? x * x * x : (x - 16.0 / 116) / 7.787);
	y = 1.00000 * ((y * y * y > 0.008856) ? y * y * y : (y - 16.0 / 116) / 7.787);
	z = 1.08883 * ((z * z * z > 0.008856) ? z * z * z : (z - 16.0 / 116) / 7.787);

	r = x * 3.2406 + y * -1.5372 + z * -0.4986;
	g = x * -0.9689 + y * 1.8758 + z * 0.0415;
	b = x * 0.0557 + y * -0.2040 + z * 1.0570;

	r = (r > 0.0031308) ? (1.055 * pow(r, 1 / 2.4) - 0.055) : 12.92 * r;
	g = (g > 0.0031308) ? (1.055 * pow(g, 1 / 2.4) - 0.055) : 12.92 * g;
	b = (b > 0.0031308) ? (1.055 * pow(b, 1 / 2.4) - 0.055) : 12.92 * b;

	rgb[0] = max(0, min(1, r)) * 255;
	rgb[1] = max(0, min(1, g)) * 255;
	rgb[2] = max(0, min(1, b)) * 255;
}

void Image::rgb2lab(Pixel& rgb, Pixel& lab) {
	float r = rgb[0] / 255,
		g = rgb[1] / 255,
		b = rgb[2] / 255,
		x, y, z;

	r = (r > 0.04045) ? pow((r + 0.055) / 1.055, 2.4) : r / 12.92;
	g = (g > 0.04045) ? pow((g + 0.055) / 1.055, 2.4) : g / 12.92;
	b = (b > 0.04045) ? pow((b + 0.055) / 1.055, 2.4) : b / 12.92;

	x = (r * 0.4124 + g * 0.3576 + b * 0.1805) / 0.95047;
	y = (r * 0.2126 + g * 0.7152 + b * 0.0722) / 1.00000;
	z = (r * 0.0193 + g * 0.1192 + b * 0.9505) / 1.08883;

	x = (x > 0.008856) ? pow(x, 1.0 / 3) : (7.787 * x) + 16.0 / 116;
	y = (y > 0.008856) ? pow(y, 1.0 / 3) : (7.787 * y) + 16.0 / 116;
	z = (z > 0.008856) ? pow(z, 1.0 / 3) : (7.787 * z) + 16.0 / 116;

	lab[0] = (116 * y) - 16, lab[1] = 500 * (x - y), lab[2] = 200 * (y - z);
}
