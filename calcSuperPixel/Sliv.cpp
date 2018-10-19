#include "pch.h"

#include "Sliv.h"
#include "ctime"
#include "iostream"

Sliv::Sliv(wstring fileName, int k, int m)
{
	image = Image::read(fileName, width, height);

	N = width * height, K = k, M = m;
	S = (int)sqrtf(N / K);

	label = vector<Cluster*>(width * height, NULL);
	dis = vector<vector<float>>(width, vector<float>(height, 1e10));
}

void Sliv::initClusters()
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
}

float Sliv::getGradient(int x, int y)
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

void Sliv::adjustCluster()
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

void Sliv::search()
{
	int x, y;
	float L, A, B, Dc, Ds, D;
	for (Cluster& cluster : clusters) {
		int xMin  = cluster.x - 2 * S, xMax = cluster.x + 2 * S;
		int yMin = cluster.y - 2 * S, yMax = cluster.y + 2 * S;
		for (x = xMin; x <= xMax; x++) {
			if (x < 0 || x >= width) continue;
			for (y = yMin; y <= yMax; y++) {
				if (y < 0 || y >= height) continue;
				L = image[x][y][0], A = image[x][y][1], B = image[x][y][2];
				Dc = sqrt(pow(L - cluster.p[0], 2) + pow(A - cluster.p[1], 2) + pow(B - cluster.p[2], 2));
				Ds = sqrt(pow(x - cluster.x, 2) + pow(y - cluster.y, 2));
				D = sqrt(pow(Dc / M, 2) + pow(Ds / S, 2));
				if (D < dis[x][y]) {
					int position = x * width + y;
					if (label[position]) {
						label[position]->points.erase(position);
					}
					cluster.points.insert(position);
					label[position] = &cluster;
					dis[x][y] = D;
				}
			}
		}
	}
}

void Sliv::update()
{
	int x, y;
	for (Cluster& cluster : clusters) {
		int sumX = 0, sumY = 0;
		for (int position : cluster.points) {
			x = position / width;
			y = position % height;
			sumX += x;
			sumY += y;
		}
		x = sumX / cluster.points.size();
		y = sumY / cluster.points.size();
		cluster.update(x, y, image[x][y]);
	}
}

void Sliv::train(int times)
{
	initClusters();
	adjustCluster();
	for (int i = 0; i < times; i++) {
		time_t start = time(0);
		search();
		update();
		time_t end = time(0);
		std::cout << i << " " << end - start << endl;
		wstring name = L"lena" + to_wstring(i) + L".jpg";
		save(name);
		std::cout << "save time " << time(0) - end << endl;
	}
}

void Sliv::save(wstring fileName)
{
	int x, y, w = width, h = height;
	IMAGE newImage = Image::copy(image, width, height);
	for (Cluster& cluster : clusters)
		for (int  p : cluster.points) {
			x = p / w;
			y = p % w;
			newImage[x][y][0] = cluster.p[0];
			newImage[x][y][1] = cluster.p[1];
			newImage[x][y][2] = cluster.p[2];
		}
	Image::save(fileName, newImage, width, height);
}
