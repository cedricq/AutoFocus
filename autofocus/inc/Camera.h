#ifndef CAMERA_H_
#define CAMERA_H_

#include <opencv2/opencv.hpp>

namespace cam
{

class Camera {

public:

    Camera(int minFocusPos, int maxFocusPos);

    static cv::Mat takePictureMask(const cv::Mat& depthMap, int ppn, int dpn);

    // Simulate setting the motor position
    void setFocusPosition(int position);

    // Simulate reading the current position
    int getFocusPosition() const {
        return currentFocusPosition_;
    }

    // Optional: expose limits
    int getMinFocusPosition() const { return minFocusPosition_; }
    int getMaxFocusPosition() const { return maxFocusPosition_; }

private:    
    int minFocusPosition_;
    int maxFocusPosition_;
    int currentFocusPosition_;
};

} // namespace
#endif
