#include "ObstacleDetector.h"
#include <cmath>
#include <iostream>

ObstacleDetector::ObstacleDetector() {
    resetGrid();
}

void ObstacleDetector::resetGrid() {
    leftDanger = rightDanger = topDanger = bottomDanger = 0;
    leftSafe = rightSafe = topSafe = bottomSafe = 0;
    total = 0;
}

void ObstacleDetector::processGrid(const std::vector<dai::SpatialLocationCalculatorData::SpatialLocation>& spatialData, const cv::Mat& depthFrame) {
    resetGrid();

    // Process spatial data to fill grid and determine danger and safe zones
    for (const auto& depthData : spatialData) {
        auto coords = depthData.spatialCoordinates;
        float distance = std::sqrt(coords.x * coords.x + coords.y * coords.y + coords.z * coords.z);
        int col = static_cast<int>(depthData.config.roi.topLeft().x * GRID_SIZE);
        int row = static_cast<int>(depthData.config.roi.topLeft().y * GRID_SIZE);

        if (distance / 1000.0f < 1.0f) {
            grid[row][col]++;
        }

        // Update danger and safe zones
        if (row == 0) topSafe += grid[row][col];
        if (row == GRID_SIZE - 1) bottomSafe += grid[row][col];
        if (col == 0) leftSafe += grid[row][col];
        if (col == GRID_SIZE - 1) rightSafe += grid[row][col];

        if (row < GRID_SIZE - 1) topDanger += grid[row][col];
        if (row > 0) bottomDanger += grid[row][col];
        if (col < GRID_SIZE - 1) leftDanger += grid[row][col];
        if (col > 0) rightDanger += grid[row][col];

        total += grid[row][col];
    }
}

std::string ObstacleDetector::getAction() {
    std::string action;

    if (total == 0) {
        action = "Không có vật cản, tiếp tục bay";
    } else if (leftDanger > 0 && rightSafe == 0) {
        action = "Né phải";
    } else if (rightDanger > 0 && leftSafe == 0) {
        action = "Né trái";
    } else if (topDanger > 0 && bottomSafe == 0) {
        action = "Bay xuống";
    } else if (bottomDanger > 0 && topSafe == 0) {
        action = "Bay lên";
    } else {
        action = "Lùi lại";
    }

    return action;
}
