#ifndef DEPTHTAIHANDLER_H
#define DEPTHTAIHANDLER_H

#include <depthai/depthai.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

class DepthAIHandler {
public:
    DepthAIHandler();
    ~DepthAIHandler();

    void setupPipeline();
    void startPipeline();
    cv::Mat getDepthFrame();
    std::vector<dai::SpatialLocationCalculatorData::SpatialLocation> getSpatialData();

private:
    void createROIs();

    dai::Pipeline pipeline;
    dai::Device* device;
    std::shared_ptr<dai::DataOutputQueue> depthQueue;
    std::shared_ptr<dai::DataOutputQueue> spatialCalcQueue;
    std::shared_ptr<dai::DataInputQueue> xinSpatialCalcConfig;
};

#endif // DEPTHTAIHANDLER_H
