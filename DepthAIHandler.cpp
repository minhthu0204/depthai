#include "DepthAIHandler.h"
#include <iostream>

DepthAIHandler::DepthAIHandler() : device(nullptr) {
    setupPipeline();
}

DepthAIHandler::~DepthAIHandler() {
    if (device) {
        delete device;
    }
}

void DepthAIHandler::setupPipeline() {
    using namespace dai;

    // Define nodes and create pipeline
    auto monoLeft = pipeline.create<node::MonoCamera>();
    auto monoRight = pipeline.create<node::MonoCamera>();
    auto stereo = pipeline.create<node::StereoDepth>();
    auto spatialLocationCalculator = pipeline.create<node::SpatialLocationCalculator>();

    auto xoutDepth = pipeline.create<node::XLinkOut>();
    auto xoutSpatialData = pipeline.create<node::XLinkOut>();
    xinSpatialCalcConfig = pipeline.create<node::XLinkIn>();

    xoutDepth->setStreamName("depth");
    xoutSpatialData->setStreamName("spatialData");

    // Set up cameras
    monoLeft->setResolution(MonoCameraProperties::SensorResolution::THE_400_P);
    monoRight->setResolution(MonoCameraProperties::SensorResolution::THE_400_P);
    monoLeft->setCamera("left");
    monoRight->setCamera("right");

    // Set up stereo depth
    stereo->setDefaultProfilePreset(node::StereoDepth::PresetMode::HIGH_DENSITY);
    stereo->setLeftRightCheck(true);
    stereo->setSubpixel(true);

    // Set up spatial location calculator
    spatialLocationCalculator->inputConfig.setWaitForMessage(false);
    createROIs();  // Call to create the ROIs

    // Linking
    monoLeft->out.link(stereo->left);
    monoRight->out.link(stereo->right);
    stereo->depth.link(spatialLocationCalculator->inputDepth);
    spatialLocationCalculator->passthroughDepth.link(xoutDepth->input);
    spatialLocationCalculator->out.link(xoutSpatialData->input);
    xinSpatialCalcConfig->out.link(spatialLocationCalculator->inputConfig);

    // Start device and pipeline
    device = new Device(pipeline);
    device->setIrLaserDotProjectorBrightness(1000);

    // Output queues
    depthQueue = device->getOutputQueue("depth", 4, false);
    spatialCalcQueue = device->getOutputQueue("spatialData", 4, false);
}

void DepthAIHandler::createROIs() {
    using namespace dai;
    const int GRID_SIZE = 5;

    auto spatialLocationCalculator = pipeline.create<node::SpatialLocationCalculator>();

    // Create 25 ROIs (5x5 grid)
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            SpatialLocationCalculatorConfigData config;
            config.depthThresholds.lowerThreshold = 200;
            config.depthThresholds.upperThreshold = 30000;
            config.calculationAlgorithm = SpatialLocationCalculatorAlgorithm::MEDIAN;
            config.roi = Rect(
                Point2f(i * 0.2f, j * 0.2f),
                Point2f((i + 1) * 0.2f, (j + 1) * 0.2f)
                );
            spatialLocationCalculator->initialConfig.addROI(config);
        }
    }
}

void DepthAIHandler::startPipeline() {
    // Placeholder for any additional startup logic
}

cv::Mat DepthAIHandler::getDepthFrame() {
    auto inDepth = depthQueue->get<dai::ImgFrame>();
    return inDepth->getFrame();
}

std::vector<dai::SpatialLocationCalculatorData::SpatialLocation> DepthAIHandler::getSpatialData() {
    return spatialCalcQueue->get<dai::SpatialLocationCalculatorData>()->getSpatialLocations();
}
