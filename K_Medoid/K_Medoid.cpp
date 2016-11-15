// K_Medoid.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

typedef struct
{
	int x;
	int y;
} point;

typedef struct
{
	vector<point> clusterPoints;
	int cost = 0;
} cluster;

int manhattanDist(point pointA, point pointB)
{
	return abs(abs(pointA.x - pointB.x) + abs(pointA.y - pointB.y));
}

bool pointInVector(point pointA, vector<point> pointList)
{
	for (auto i = 0; i < pointList.size(); i++)
	{
		if (pointA.x == pointList[i].x && pointA.y == pointList[i].y)
			return true;
	}

	return false;
}

int totalClusterListCost(const vector<cluster> clusters)
{
	int totalCost = 0;

	for (auto i = 0; i < clusters.size(); i++)
	{
		totalCost += clusters[i].cost;
	}

	return totalCost;
}

void printCluster(const vector<point> clusterPoints)
{
	for (auto j = 0; j < clusterPoints.size(); j++)
	{
		cout << clusterPoints[j].x << " " << clusterPoints[j].y << endl;
	}
	cout << endl;
}

void printClusterList(const vector<cluster> clusterList)
{
	for (auto i = 0; i < clusterList.size(); i++)
	{
		cout << "Cluster" << i+1 << endl;
		cout << "Cost: " << clusterList[i].cost << endl;
		printCluster(clusterList[i].clusterPoints);
	}
	cout << "Total Cost: " << totalClusterListCost(clusterList) << endl;
}

vector<cluster> clustersTotalCost(const vector<point> centerPoints, const vector<point> points)
{
	int totalCost = 0, index;
	int minDist;
	point clusterTmp;
	vector<cluster> clusterList(centerPoints.size());

	for (auto i = 0; i < centerPoints.size(); i++)
	{
		clusterList[i].clusterPoints.push_back(centerPoints[i]);
	}

	for (auto i = 0; i < points.size(); i++)
	{
		if (pointInVector(points[i], centerPoints))
			continue;
		index = -1;
		minDist = manhattanDist(centerPoints[0], points[i]);

		for (auto j = 0; j < centerPoints.size(); j++)
		{
			int dist = manhattanDist(centerPoints[j], points[i]);
			if (dist <= minDist)
			{
				index = j;
				minDist = dist;
				clusterTmp = points[i];
			}
		}

		if (index != -1)
		{
			clusterList[index].clusterPoints.push_back(clusterTmp);
			clusterList[index].cost += minDist;
		}
	}

	return clusterList;
}

void computeUsingKMedoid(const vector<point> points, int k)
{
	vector<point> center;
	vector<cluster> minCostClusters;
	int minTotalCost = 0;

	if (k < 1)
	{
		cout << "ERROR: Value of K below 1." << endl;
		return;
	}

	for (auto i = 0; i < k; i++)
	{
		center.push_back(points[rand() % points.size()]);
	}
	
	minCostClusters = clustersTotalCost(center, points);

	minTotalCost = totalClusterListCost(minCostClusters);


	for (auto i = k - 1; i >= 0; i--)
	{	
		for (auto j = 0; j < points.size(); j++)
		{
			if (pointInVector(points[j], center))
				continue;
			center[i] = points[j];

			vector<cluster> tmpClusterList = clustersTotalCost(center, points);
			int tmpTotalClusterCost = totalClusterListCost(tmpClusterList);

			if (tmpTotalClusterCost < minTotalCost)
			{
				minTotalCost = tmpTotalClusterCost;
				minCostClusters = tmpClusterList;
			}
		}
	}

	printClusterList(minCostClusters);	
}


int main()
{
	int k = 2;
	string filename = "";
	vector<point> points;

	cout << "\t\tK-Medoid" << endl;

	while (true)
	{
		//cout << "Enter Filename: ";
		//getline(cin, filename);
		filename = "test.txt";
		ifstream openFile(filename);
		if (openFile.is_open())
		{
			string line;

			while (getline(openFile, line))
			{
				size_t pos = line.find(" ");
				point tmp;

				tmp.x = stoi(line.substr(0, pos));
				tmp.y = stoi(line.substr(pos+1));

				points.push_back(tmp);
			}

			computeUsingKMedoid(points, k);

			openFile.close();
			break;
		}

		cout << "Invalid File Name!!" << endl;
	}

	getchar();
    return 0;
}

