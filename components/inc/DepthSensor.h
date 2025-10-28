#ifndef DEPTHSENSOR_H_
#define DEPTHSENSOR_H_

#include <vector>
#include <string>

#include <opencv2/opencv.hpp>

namespace depth
{

class DepthSensor {
public:
    
    explicit DepthSensor(const std::string& filePath) {
        loadDepthImage(filePath);
    }

    const cv::Mat& getDepthData() const { return depth_; }
    int getWidth() const { return depth_.cols; }
    int getHeight() const { return depth_.rows; }

    uint16_t at(int x, int y) const {
        return depth_.at<uint16_t>(y, x);
    }

    uint16_t getDepthMin() const { return static_cast<uint16_t>(depth_min_); }
    uint16_t getDepthMax() const { return static_cast<uint16_t>(depth_max_); }

private:
    cv::Mat depth_;
    double depth_min_;
    double depth_max_;

    void loadDepthImage(const std::string& filePath) ;
};
    

} // namespace

#endif