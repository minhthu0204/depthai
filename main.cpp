#include <iostream>
#include <opencv2/opencv.hpp>
#include "DepthAIHandler.h"
#include "ObstacleDetector.h"

int main() {
    DepthAIHandler depthAI;
    ObstacleDetector detector;

    depthAI.startPipeline();

    while (true) {
        cv::Mat depthFrame = depthAI.getDepthFrame();
        auto spatialData = depthAI.getSpatialData();

        detector.processGrid(spatialData, depthFrame);

        std::string action = detector.getAction();
        std::cout << "Action: " << action << std::endl;

        // Display depth frame (optional)
        cv::imshow("Depth Frame", depthFrame);
        if (cv::waitKey(1) == 'q') break;
    }

    return 0;
}
