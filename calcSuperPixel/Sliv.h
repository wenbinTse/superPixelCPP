#pragma once
#include<vector>
#include<set>
#include "Image.h"
using namespace std;

class Cluster {
public:
	int x, y;
	Pixel p = {0, 0, 0};
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
	vector<int> label; // 存储对应的Cluster的ID
	vector<vector<float>> dis;

	vector<int> num; // 聚类数目
	vector<float> sumL;
	vector<float> sumA;
	vector<float> sumB;
	vector<int> sumX;
	vector<int> sumY;

	Sliv(wstring fileName, int k, int m);
	void initClusters();
	float getGradient(int x, int y);
	void adjustCluster();
	void search();
	void update();
	void train(int);
	void save(wstring fileName);
};

