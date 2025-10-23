#ifndef CAMERA_H_
#define CAMERA_H_

#include <opencv2/opencv.hpp>

class Camera {
public:
    Camera() = default;

    static cv::Mat computeFocusMask(const cv::Mat& depthMap, int ppn, int dpn);
};

#endif
