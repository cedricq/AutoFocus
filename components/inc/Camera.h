#ifndef CAMERA_H_
#define CAMERA_H_

#include <opencv2/opencv.hpp>

#include "CalibrationData.h"

namespace cam
{

/**
 * @brief Class representing a camera with focus control capabilities.
 * 
 * This class manages camera focus operations and calibration data. It provides functionality
 * to control focus position within defined limits and compute focus sequences based on
 * calibration data.
 * 
 * @note This class requires calibration data to be provided during construction.
 */
class Camera {

public:


    /**
     * @brief Constructor for the Camera class
     * 
     * @param minFocusPos The minimum focus position value for the camera
     * @param maxFocusPos The maximum focus position value for the camera
     * @param calibData Reference to calibration data used by the camera
     * 
     * @details Initializes a new Camera instance with the specified focus position bounds
     *          and calibration data. The current focus position is set to the minimum
     *          focus position upon construction.
     */
    Camera(int minFocusPos, int maxFocusPos, CalibrationData& calibData): 
        minFocusPosition_(minFocusPos), 
        maxFocusPosition_(maxFocusPos), 
        currentFocusPosition_(minFocusPos),
        calibData_(calibData)
    {};
    ~Camera() = default;

    /**
     * @brief Creates a depth map mask based on specified parameters.
     * 
     * @param depthMap Input depth map matrix
     * @param ppn Pixels Per Normalized unit
     * @param dpn Depth Per Normalized unit
     * @return cv::Mat Resulting mask
     */
    static cv::Mat takePictureMask(const cv::Mat& depthMap, int ppn, int dpn);

    /**
     * @brief Sets the absolute focus position of the camera
     * @param position The desired focus position value (motor steps)
     */
    void setFocusPosition(int position);
    /**
     * @brief Gets the absolute focus position of the camera
     * @return The current focus position value (motor steps)
     */
    int getFocusPosition() const { return currentFocusPosition_; }

    /**
     * @brief Computes a sequence of calibration points for focusing
     * 
     * @param ppn_target The target physical pixel number
     * @param dpn_target The target digital pixel number
     * @return std::vector<CalibrationPoint> A vector of calibration points representing the focus sequence
     */
    std::vector<CalibrationPoint> computeFocusSequence(int ppn_target, int dpn_target) const {
        return calibData_.computeFocusSequence(ppn_target, dpn_target);
    };

    // Access all data points
    const std::vector<CalibrationPoint>& getData() const { return calibData_.getData(); }

    // Expose limits
    int getMinFocusPosition() const { return minFocusPosition_; }
    int getMaxFocusPosition() const { return maxFocusPosition_; }

    // Get calibration limits
    int getPPNMin() { return calibData_.getPPNMin(); }
    int getDPNMax() { return calibData_.getDPNMax(); }

private:    
    CalibrationData&  calibData_;

    int minFocusPosition_ ;
    int maxFocusPosition_;
    int currentFocusPosition_;
};

} // namespace
#endif
