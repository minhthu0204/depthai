#ifndef OBSTACLEDETECTOR_H
#define OBSTACLEDETECTOR_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "depthai/depthai.hpp"

class ObstacleDetector {
public:
    static const int GRID_SIZE = 5;

    ObstacleDetector();
    void processGrid(const std::vector<dai::SpatialLocationCalculatorData::SpatialLocation>& spatialData, const cv::Mat& depthFrame);
    std::string getAction();

private:
    int grid[GRID_SIZE][GRID_SIZE] = {0};
    int total = 0;
    int leftDanger, rightDanger, topDanger, bottomDanger;
    int leftSafe, rightSafe, topSafe, bottomSafe;

    void resetGrid();
};

#endif // OBSTACLEDETECTOR_H
