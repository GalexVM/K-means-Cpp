#include <iostream>
#include <vector>
#include <random>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <math.h>

using namespace std;
struct Point;
using matrixPoint = vector<vector<Point*>>;
const double min_movement = 0.1;



struct Point {
	double x, y;
	short color = -1; //-1: empty, [0-3]: centroid
	Point(double x_, double y_) :x(x_), y(y_) {}
};


vector<Point*> generatePoints(int n, double xMax, double yMax);
vector<Point*> generateCentroids(int xMax, int yMax);
matrixPoint colorPoints(vector<Point*> pts, vector<Point*> centroids);
double getDistance(Point p1, Point p2);
vector<bool> moveCentroids(vector<Point*> centroids, matrixPoint data);
double moveCentroid(Point* c, vector<Point*> d);


int main() {
	srand(time(NULL));
	vector<Point*> puntos = generatePoints(10, 20, 20);
	cout << "Puntos iniciales\n";
	for (auto i : puntos)
		cout << "(" << i->x << "," << i->y << ")" << endl;
	cout << endl;

	cout << "Centros iniciales\n";
	vector<Point*> centroids = generateCentroids(50, 40);
	for (auto i : centroids)
		cout << "(" << i->x << "," << i->y << ")" << endl;

	cout << "Grupos:\n";
	matrixPoint data = colorPoints(puntos, centroids);
	for (auto i : data)
	{
		for (auto j : i)
		{
			cout << "(" << j->x << "," << j->y << ")" << "; ";
		}
		cout << endl;
	}

	cout << "Por color:\n";
	for (auto i : puntos)
		cout << "(" << i->x << "," << i->y << ")" << "; color: " << i->color << endl;
	cout << endl;

	cout << "Nuevos centroids:\n";
	vector<bool> c_key = moveCentroids(centroids, data);
	for (auto i : centroids)
		cout << "(" << i->x << "," << i->y << ")" << endl;


	do {
		matrixPoint data = colorPoints(puntos, centroids);
		cout << "Por color:\n";
		for (auto i : puntos)
			cout << "(" << i->x << "," << i->y << ")" << "; color: " << i->color << endl;
		cout << endl;

		c_key = moveCentroids(centroids, data);
		cout << "Nuevos centroids:\n";
		vector<bool> c_key = moveCentroids(centroids, data);
		for (auto i : centroids)
			cout << "(" << i->x << "," << i->y << ")" << endl;
		cout << "En movimiento:\n";
		for (auto i : c_key)
			cout << i;
		cout << endl;

	} while (c_key[0] || c_key[1] || c_key[2] || c_key[3]);
	cout << endl;
	
	/*for (auto i : centroids)
		cout << "(" << i->x << "," << i->y << ")" << endl;

	for (auto i : puntos)
		cout << "(" << i->x << "," << i->y << ")" << "; color: " << i->color << endl;
	cout << endl;*/

}
double moveCentroid(Point* c, vector<Point*> d)
{
	double maxX, minX, maxY, minY, dist_recorrida;
	if (d.size() > 0)
	{
		maxX = minX = d[0]->x;
		maxY = minY = d[0]->y;
		for (int j = 0; j < d.size(); j++) //Each point
		{
			maxX = (d[j]->x > maxX) ? d[j]->x : maxX;
			maxY = (d[j]->y > maxY) ? d[j]->y : maxY;
			minX = (d[j]->x < minX) ? d[j]->x : minX;
			minY = (d[j]->y < minY) ? d[j]->y : minY;
		}
		dist_recorrida = getDistance({ (maxX + minX) / 2 ,(maxY + minY) / 2 }, { c->x ,c->y });
		c->x = (maxX + minX) / 2;
		c->y = (maxY + minY) / 2;
	}
	else
	{
		dist_recorrida = 0;
	}
	return dist_recorrida;
}

vector<bool> moveCentroids(vector<Point*> centroids, matrixPoint data)
{
	vector<bool> c_key = { true,true,true,true };
	for (int i = 0; i < 4; i++)//Each group
	{

		if (c_key[i] && moveCentroid(centroids[i], data[i]) < min_movement)
		{
			c_key[i] = false;
		}
		
	}
	return c_key;
}

matrixPoint colorPoints(vector<Point*> pts, vector<Point*> centroids)
{
	matrixPoint r = matrixPoint(4, vector<Point*>(0));
	for (int i = 0; i < pts.size(); i++)//Recorrer cada punto
	{
		short minIn = 0;
		for (int j = 0; j < 4; j++)//Recorrer cada centroid
		{
			if (getDistance(*pts[i], *centroids[j]) <
				getDistance(*pts[i], *centroids[minIn])) {
				minIn = j;
			}
		}
		pts[i]->color = minIn;
		Point* temp = pts[i];
		r[minIn].push_back(temp);

	}
	return r;
}

double getDistance(Point p1, Point p2)
{
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

vector<Point*> generatePoints(int n, double xMax, double yMax)
{
	vector<Point*> r;
	r = vector<Point*>(n);

	for (auto i = 0; i < n; i++)
	{
		double randX = (double)rand() / RAND_MAX;
		double randy = (double)rand() / RAND_MAX;
		r[i] = new Point(randX * xMax, randy * yMax); //random points
	}
	return r;
}

vector<Point*> generateCentroids(int xMax, int yMax)
{
	vector<Point*> r;
	r = generatePoints(4, xMax, yMax);
	return r;
}