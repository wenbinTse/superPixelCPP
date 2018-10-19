#pragma once
#include<string>
#include<atlimage.h>

using namespace std;

typedef float Pixel[3];
typedef Pixel** IMAGE;

class Image
{
public:
	static IMAGE read(wstring fileName, int& width, int& height);
	static void save(wstring fileName, IMAGE image, int width, int height);
	static IMAGE create(int width, int height);
	static IMAGE copy(IMAGE image, int width, int height);
	static void rgb2lab(IMAGE, int width, int height);
	static void lab2rgb(IMAGE, int width, int height);
	static void rgb2lab(Pixel& rgb, Pixel& lab);
	static void lab2rgb(Pixel& lab, Pixel& rgb);
};
