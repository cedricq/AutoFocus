#ifndef CAMERA_H_
#define CAMERA_H_

#include <opencv2/opencv.hpp>

#include "CalibrationData.h"

namespace cam
{

class Camera {

public:

    Camera(int minFocusPos, int maxFocusPos, CalibrationData& calibData): 
        minFocusPosition_(minFocusPos), 
        maxFocusPosition_(maxFocusPos), 
        currentFocusPosition_(minFocusPos),
        calibData_(calibData)
    {};
    ~Camera() = default;

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

    // Access all data points
    const std::vector<CalibrationPoint>& getData() const { return calibData_.getData(); }
    // Compute a focus sequence to achieve a target sharp range
    std::vector<CalibrationPoint> computeFocusSequence(int ppn_target, int dpn_target) const {
        return calibData_.computeFocusSequence(ppn_target, dpn_target);
    };
    // Get calibration limits
    int getPPNMin() { return calibData_.getPPNMin(); }
    int getDPNMax() { return calibData_.getDPNMax(); }

private:    
    CalibrationData&  calibData_;

    int minFocusPosition_       {0};
    int maxFocusPosition_       {0};
    int currentFocusPosition_   {0};
};

} // namespace
#endif
