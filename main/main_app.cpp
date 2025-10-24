#include "CalibrationData.h"
#include "DepthSensor.h"
#include "Camera.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <filesystem>


int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0]
                  << " <calibration.csv> <depth_map.png>" <<std::endl;
        return 1;
    }

    const std::string calibFile = argv[1];
    const std::string depthFile = argv[2];

    std::filesystem::path pathObj(depthFile);
    std::string filename_no_ext = pathObj.stem().string();

    try {
        
        // Load calibration data
        //
        calib::CalibrationData calib;
        if (!calib.loadFromCSV(calibFile)) {
            return 1;
        }
        std::cout << "[OK] Calibration file loaded (" 
                  << calib.getData().size() << " points)" <<std::endl;

        // Load depth map
        //
        depth::DepthSensor sensor(depthFile);
        const auto& depthMat = sensor.getDepthData();
        std::cout << "[OK] Depth map loaded: "
                  << sensor.getWidth() << "x" << sensor.getHeight() << " pixels" <<std::endl;

        // Compute global depth range in mm
        uint16_t ppn_target = sensor.getDepthMin();
        uint16_t dpn_target = sensor.getDepthMax();
        std::cout << "Scene depth range: ["
                  << ppn_target << ", " << dpn_target << "] mm" <<std::endl;

        // Compute focus sequence
        //
        auto focusList = calib.computeFocusSequence(ppn_target, dpn_target);
        if (focusList.empty()) {
            std::cerr << "No suitable focus sequence found for this scene." <<std::endl;
            return 1;
        }
        std::cout << "[OK] Focus sequence: " << focusList.size() << " positions" <<std::endl;

        // Move focus camera motor and take snapshot
        //
        cv::Mat overall_mask = cv::Mat::zeros(depthMat.size(), CV_8UC1);
        cam::Camera camera(0, 100000); // Just a random camera
        int i = 0;
        for (const auto& f : focusList) {
            std::cout << "Moving to focus position: "
                      << f.focusPosition <<std::endl;

            camera.setFocusPosition(f.focusPosition);

            // TODO: Wait until camera focus is in position & Timeout & Likely running on a different thread
            while(camera.getFocusPosition() != f.focusPosition);

            // Take snapshot picture
            const auto& mask = cam::Camera::takePictureMask(depthMat, f.ppn, f.dpn);
            
            // Output png file per snapshot
            std::string filename = filename_no_ext + "_" + std::to_string(i) 
                + "_"  + std::to_string(f.ppn) + "_" + std::to_string(f.dpn) + ".png";
            cv::imwrite(filename, mask);
            i++;
            std::cout << "SNAPSHOT ! @ position " << camera.getFocusPosition() <<" => " <<filename << std::endl;

            // Merge the depth masks
            cv::bitwise_or(overall_mask, mask, overall_mask);
        }

        // Overall Output png file combining all the masks
        std::string filename = filename_no_ext + "_overall_"  + std::to_string(ppn_target) + "_" + std::to_string(dpn_target) + ".png";
        cv::imwrite(filename, overall_mask);
        std::cout << "[DONE] Focus sweep complete and merged => " <<filename <<std::endl;

        // Input depth file normalized into 8bits grey format just for visualization
        filename = filename_no_ext + "_input_greyed.png";
        cv::Mat greyed = cv::max(depthMat - sensor.getDepthMin(), 0);
        double minVal, maxVal;
        cv::Point minLoc, maxLoc;
        cv::minMaxLoc(greyed, &minVal, &maxVal, &minLoc, &maxLoc);
        cv::Mat normalized;
        greyed.convertTo(normalized, CV_8U, 255.0 / maxVal);
        cv::imwrite(filename, normalized);
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}