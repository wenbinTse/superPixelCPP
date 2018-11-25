#include "pch.h"

#include "Slic.h"
#include "ctime"
#include "iostream"

Slic::Slic(wstring fileName, int k, int m)
{
	image = Image::read(fileName, width, height);

	N = width * height, K = k, M = m;
	S = (int)sqrt(N / K);

	label = vector<int>(N, -1);
	dis = vector<vector<float>>(width, vector<float>(height, 1e10));
}

void Slic::initClusters()
{
	int x = S / 2, y = S / 2;
	while (x < width) {
		while (y < height) {
			clusters.push_back(Cluster(x, y, image[x][y]));
			y += S;
		}
		y = S / 2;
		x += S;
	}
	K = clusters.size();

	num = vector<int>(K, 0);
	sumL = vector<float>(K, 0);
	sumA = vector<float>(K, 0);
	sumB = vector<float>(K, 0);
	sumX = vector<int>(K, 0);
	sumY = vector<int>(K, 0);
}

float Slic::getGradient(int& x, int& y)
{
	if (x >= width - 1)
		x = width - 2;
	if (y > height - 1)
		y = height - 2;

	float gradient = image[x + 1][y + 1][0] - image[x][y][0] +
		image[x + 1][y + 1][1] - image[x][y][1] +
		image[x + 1][y + 1][2] - image[x][y][2];
	return gradient;
}

void Slic::adjustCluster()
{
	int i, j, x, y;
	float currGra, newGra;
	for (Cluster& cluster : clusters) {
		currGra = getGradient(cluster.x, cluster.y);
		for (i = -1; i <= 1; i++)
			for (j = -1; j <= 1; j++) {
				x = cluster.x + i;
				y = cluster.y + j;
				newGra = getGradient(x, y);
				if (newGra < currGra) {
					cluster.update(x, y, image[x][y]);
					currGra = newGra;
				}
			}
	}
}

void Slic::search()
{
	int x, y, xMin, xMax, yMin, yMax;
	float L, A, B, l, a, b, Dc, Ds, D;
	for (int i = 0; i < K; i++) {
		Cluster cluster = clusters[i];
		xMin  = max(cluster.x - 2 * S, 0), xMax = min(cluster.x + 2 * S, width - 1);
		yMin = max(cluster.y - 2 * S, 0), yMax = min(cluster.y + 2 * S, height - 1);
		l = cluster.p[0], a = cluster.p[1], b = cluster.p[2];
		for (x = xMin; x <= xMax; x++) {
			for (y = yMin; y <= yMax; y++) {
				L = image[x][y][0], A = image[x][y][1], B = image[x][y][2];
				Dc = sqrt(pow(L - l, 2) + pow(A - a, 2) + pow(B - b, 2));
				//Dc = (L - l) * (L - l) + (A - a) * (A - a) + (B - b) * (B - b);
				Ds = sqrt(pow(x - cluster.x, 2) + pow(y - cluster.y, 2));
				//Ds = (x - cluster.x) * (x - cluster.x) + (y - cluster.y) * (y - cluster.y);
				D = sqrt(pow(Dc / M, 2) + pow(Ds / S, 2));
				D = Dc / M + Ds / S;

				if (D < dis[x][y]) {
					int position = y * width + x;
					label[position] = i;
					dis[x][y] = D;
				}
			}
		}
	}
}

void Slic::update()
{
	num.assign(K, 0);
	sumL.assign(K, 0);
	sumA.assign(K, 0);
	sumB.assign(K, 0);
	sumX.assign(K, 0);
	sumY.assign(K, 0);

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			int pos = y * width + x, idx = label[pos];
			num[idx]++;
			sumL[idx] += image[x][y][0];
			sumA[idx] += image[x][y][1];
			sumB[idx] += image[x][y][2];
			sumX[idx] += x;
			sumY[idx] += y;
		}
	}
	for (int i = 0; i < K; i++) {
		int n = num[i];
		clusters[i].update(sumX[i] / n, sumY[i] / n,
			sumL[i] / n, sumA[i] / n, sumB[i] / n);
	}
}

void Slic::train(int times)
{
	initClusters();
	adjustCluster();
	for (int i = 0; i < times; i++) {
		cout << "第" << (i + 1) << "次迭代" << endl;
		search();
		update();
		wstring name = L"result_" + to_wstring(i + 1) + L"_iter.jpg";
		save(name);
		wcout << "保存为: " << name << endl;
	}
}

void Slic::save(wstring fileName)
{
	IMAGE newImage = Image::copy(image, width, height);
	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++) {
			int pos = y * width + x, idx = label[pos];
			newImage[x][y][0] = clusters[idx].p[0];
			newImage[x][y][1] = clusters[idx].p[1];
			newImage[x][y][2] = clusters[idx].p[2];
		}
	Image::save(fileName, newImage, width, height);
}
