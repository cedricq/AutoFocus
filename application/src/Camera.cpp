#include "Camera.h"

#include <string>
#include <stdexcept>


cv::Mat Camera::computeFocusMask(const cv::Mat& depthMap, int ppn, int dpn)
{
    if (depthMap.empty()) {
        throw std::invalid_argument("Camera::computeFocusMask: input depth map is empty.");
    }

    if (depthMap.type() != CV_16UC1) {
        throw std::invalid_argument("Camera::computeFocusMask: expected CV_16UC1 depth map.");
    }

    cv::Mat mask = cv::Mat::zeros(depthMap.size(), CV_8UC1);

    // Vectorized operation: create mask for pixels within range (0 if in range, 255 otherwise)
    cv::inRange(depthMap, cv::Scalar(ppn), cv::Scalar(dpn), mask);

    return mask;
}
