#include "pch.h"

#include "Sliv.h"
#include "ctime"
#include "iostream"

Sliv::Sliv(wstring fileName, int k, int m)
{
	image = Image::read(fileName, width, height);

	N = width * height, K = k, M = m;
	S = (int)sqrtf(N / K);

	label = vector<Cluster*>(N, NULL);
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
	int x, y, xMin, xMax, yMin, yMax;
	float L, A, B, l, a, b, Dc, Ds, D;
	for (Cluster& cluster : clusters) {
		xMin  = max(cluster.x - 2 * S, 0), xMax = min(cluster.x + 2 * S, width - 1);
		yMin = max(cluster.y - 2 * S, 0), yMax = min(cluster.y + 2 * S, height - 1);
		l = cluster.p[0], a = cluster.p[1], b = cluster.p[2];
		for (x = xMin; x <= xMax; x++) {
			for (y = yMin; y <= yMax; y++) {
				L = image[x][y][0], A = image[x][y][1], B = image[x][y][2];
				Dc = sqrt(pow(L - l, 2) + pow(A - a, 2) + pow(B - b, 2));
				Ds = sqrt(pow(x - cluster.x, 2) + pow(y - cluster.y, 2));
				D = sqrt(pow(Dc / M, 2) + pow(Ds / S, 2));

				if (D < dis[x][y]) {
					int position = x * height + y;
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
		int sumX = 0, sumY = 0, num = cluster.points.size();
		float sumL = 0, sumA = 0, sumB = 0;
		for (int position : cluster.points) {
			x = position / width;
			y = position % height;
			sumX += x;
			sumY += y;
			sumL += image[x][y][0];
			sumA += image[x][y][1];
			sumB += image[x][y][2];
		}
		x = sumX / num;
		y = sumY / num;
		cluster.update(x, y, sumL / num, sumA / num, sumB / num);
	}
}

void Sliv::train(int times)
{
	initClusters();
	adjustCluster();
	for (int i = 0; i < times; i++) {
		time_t start = time(0);
		search();
		time_t end = time(0);
		std::cout << i << " search " << end - start << endl;
		update();
		end = time(0);
		std::cout << i << " update " << time(0) - end << endl;
		wstring name = L"lena" + to_wstring(i) + L".jpg";
		end = time(0);
		save(name);
		std::cout << "save time " << time(0) - end << endl;
	}
}

void Sliv::save(wstring fileName)
{
	int x, y;
	IMAGE newImage = Image::copy(image, width, height);
	for (Cluster& cluster : clusters)
		for (int  p : cluster.points) {
			x = p / height;
			y = p % height;
			newImage[x][y][0] = cluster.p[0];
			newImage[x][y][1] = cluster.p[1];
			newImage[x][y][2] = cluster.p[2];
		}
	Image::save(fileName, newImage, width, height);
}
