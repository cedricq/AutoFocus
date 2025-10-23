#include "DepthSensor.h"

#include <stdexcept>

namespace depth
{
    void DepthSensor::loadDepthImage(const std::string& filePath) {
        // Read the image without changing the bit depth
        depth_ = cv::imread(filePath, cv::IMREAD_UNCHANGED);
        if (depth_.empty()) {
            throw std::runtime_error("Cannot load depth map: " + filePath);
        }

        if (depth_.type() != CV_16UC1) {
            throw std::runtime_error("Expected 16-bit single-channel PNG");
        }
        cv::minMaxLoc(depth_, &depth_min_, &depth_max_);
    }    

} // namespace
