#include "Camera.h"

#include <string>
#include <stdexcept>

namespace cam
{

cv::Mat Camera::takePictureMask(const cv::Mat& depthMap, int ppn, int dpn) 
{
    if (depthMap.empty()) {
        throw std::invalid_argument("Camera::takePictureMask: input depth map is empty.");
    }

    if (depthMap.type() != CV_16UC1) {
        throw std::invalid_argument("Camera::takePictureMask: expected CV_16UC1 depth map.");
    }

    cv::Mat mask = cv::Mat::zeros(depthMap.size(), CV_8UC1);

    // Create mask for pixels within range (0 if in range, 255 otherwise)
    cv::inRange(depthMap, cv::Scalar(ppn), cv::Scalar(dpn), mask);

    int count = cv::countNonZero(mask);
    if (count == 0) mask.release(); // return empty if no pixel is in range
    
    return mask;

}

Camera::Camera(int minPos, int maxPos)
        : minFocusPosition_(minPos), maxFocusPosition_(maxPos), currentFocusPosition_(minPos)
    {}

void Camera::setFocusPosition(int position) {
    if (position < minFocusPosition_ || position > maxFocusPosition_) {
        throw std::out_of_range("Focus position out of range");
    }

    // For simulation: instant move
    currentFocusPosition_ = position;
}

} // namespace