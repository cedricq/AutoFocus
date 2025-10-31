#ifndef DEPTHSENSOR_H_
#define DEPTHSENSOR_H_

#include <vector>
#include <string>

#include <opencv2/opencv.hpp>

namespace depth
{

/**
 * @class DepthSensor
 * @brief Loads and provides access to a single 16-bit depth image.
 *
 * @details
 * DepthSensor encapsulates a single-channel 16-bit depth image stored in an OpenCV
 * cv::Mat (expected type CV_16U). The image is loaded in the constructor via
 * loadDepthImage(), and the object exposes read-only accessors for the underlying
 * matrix, its dimensions, and per-pixel depth values. Minimum and maximum depth
 * values found in the loaded image are recorded and can be queried.
 *
 */
class DepthSensor {
public:
    
    DepthSensor(const std::string& filePath) {
        loadDepthImage(filePath);
    }

    /**
     * @brief Get the current depth data matrix
     * @return A constant reference to the depth data matrix (cv::Mat)
     */
    const cv::Mat& getDepthData() const { return depth_; }

    uint16_t at(int x, int y) const {
        return depth_.at<uint16_t>(y, x);
    }

    int getWidth() const { return depth_.cols; }
    int getHeight() const { return depth_.rows; }

    uint16_t getMin() const { return static_cast<uint16_t>(depth_min_); }
    uint16_t getMax() const { return static_cast<uint16_t>(depth_max_); }

private:
    cv::Mat depth_      {};
    double depth_min_   {0.0};
    double depth_max_   {0.0};

    void loadDepthImage(const std::string& filePath) ;
};
    

} // namespace

#endif