#pragma once
#include<vector>
#include<set>
#include "Image.h"
using namespace std;

class Cluster {
public:
	int x, y;
	Pixel p = {0, 0, 0};
	set<int> points;
	Cluster(int _x, int _y, Pixel _p) {
		
		update(_x, _y, _p);
	}
	void update(int _x, int _y, Pixel _p) {
		x = _x, y = _y, p[0] = _p[0], p[1] = _p[1], p[2] = _p[2];
	}
	void update(int _x, int _y, float l, float a, float b) {
		x = _x, y = _y, p[0] = l, p[1] = a, p[2] = b;
	}
};

class Sliv
{
public:
	IMAGE image;
	int K, M, N, S, width, height;
	vector<Cluster> clusters;
	vector<Cluster*> label;
	vector<vector<float>> dis;

	Sliv(wstring fileName, int k, int m);
	void initClusters();
	float getGradient(int x, int y);
	void adjustCluster();
	void search();
	void update();
	void train(int);
	void save(wstring fileName);
};

