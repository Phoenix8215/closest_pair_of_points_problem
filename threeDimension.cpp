#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib> 
#include <ctime>  
#include <iostream>
#include <limits>
#include <vector>

// 三维点结构
struct Point {
  double x, y, z; 
};

// 计算两点之间的欧几里得距离
double distance(const Point &p1, const Point &p2) {
  return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
}

// 暴力法求最近点对距离
double bruteForceClosestPair3D(const std::vector<Point> &points) {
  double minDist = std::numeric_limits<double>::max();
  for (size_t i = 0; i < points.size(); ++i) {
    for (size_t j = i + 1; j < points.size(); ++j) {
      double dist = distance(points[i], points[j]);
      minDist = std::min(minDist, dist);
    }
  }
  return minDist;
}

// 分治法求解最近点对距离
double closestUtil(std::vector<Point> &points, int left, int right) {
  if (right - left + 1 <= 3) {
    double minDist = std::numeric_limits<double>::max();
    for (int i = left; i < right; ++i) {
      for (int j = i + 1; j <= right; ++j) {
        minDist = std::min(minDist, distance(points[i], points[j]));
      }
    }
    return minDist;
  }

  int mid = left + (right - left) / 2;
  Point midPoint = points[mid];

  double dl = closestUtil(points, left, mid);
  double dr = closestUtil(points, mid + 1, right);
  double d = std::min(dl, dr);

  std::vector<Point> strip;
  for (int i = left; i <= right; ++i) {
    if (abs(points[i].x - midPoint.x) < d) {
      strip.push_back(points[i]);
    }
  }

  std::sort(strip.begin(), strip.end(),
            [](const Point &p1, const Point &p2) { return p1.y < p2.y; });

  for (size_t i = 0; i < strip.size(); ++i) {
    for (size_t j = i + 1; j < strip.size() && (strip[j].y - strip[i].y) < d;
         ++j) {
      if (abs(strip[j].z - strip[i].z) < d) {
        d = std::min(d, distance(strip[i], strip[j]));
      }
    }
  }

  return d;
}

double closestPair3D(std::vector<Point> &points) {
  std::sort(points.begin(), points.end(),
            [](const Point &p1, const Point &p2) { return p1.x < p2.x; });
  return closestUtil(points, 0, points.size() - 1);
}

std::vector<Point> generateRandomPoints(int numPoints, double rangeMin,
                                        double rangeMax) {
  std::vector<Point> points;
  points.reserve(numPoints);

  for (int i = 0; i < numPoints; ++i) {
    double x = rangeMin +
               static_cast<double>(rand()) / RAND_MAX * (rangeMax - rangeMin);
    double y = rangeMin +
               static_cast<double>(rand()) / RAND_MAX * (rangeMax - rangeMin);
    double z = rangeMin +
               static_cast<double>(rand()) / RAND_MAX * (rangeMax - rangeMin);
    points.push_back({x, y, z});
  }

  return points;
}

int main() {
  srand(static_cast<unsigned>(time(nullptr)));

  // 生成 1000 个随机点，坐标范围 [-1000, 1000]
  std::vector<Point> points = generateRandomPoints(1000, -1000.0, 1000.0);

  auto startBrute = std::chrono::high_resolution_clock::now();
  double bruteResult = bruteForceClosestPair3D(points);
  auto endBrute = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsedBrute = endBrute - startBrute;

  auto startDivide = std::chrono::high_resolution_clock::now();
  double divideResult = closestPair3D(points);
  auto endDivide = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsedDivide = endDivide - startDivide;

  std::cout << "Brute Force Result: " << bruteResult
            << ", Time: " << elapsedBrute.count() << "s\n";
  std::cout << "Divide and Conquer Result: " << divideResult
            << ", Time: " << elapsedDivide.count() << "s\n";

  return 0;
}
