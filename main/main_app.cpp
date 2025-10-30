#include "CalibrationData.h"
#include "DepthSensor.h"
#include "Camera.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <filesystem>

std::string winName = "Mask output";

void initializeDisplayWindow() {
    cv::namedWindow(winName, cv::WINDOW_NORMAL);
    cv::moveWindow(winName, 1, 1);
}

void displayImage(cv::Mat frame, int time) {
    cv::resizeWindow(winName, frame.cols / 2, frame.rows / 2);
    cv::imshow(winName, frame);
    cv::waitKey(time);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0]
                  << " <calibration.csv> <depth_map.png> (--display)" <<std::endl;
        return 1;
    }

    const std::string calibFile = argv[1];
    const std::string depthFile = argv[2];
    std::string display = "";
    if (argc == 4) display = argv[3];
    const bool doDisplay = (display == "--display");

    std::filesystem::path pathObj(depthFile);
    std::string filename_no_ext = pathObj.stem().string();

    try {

        //
        // 1. Load depth map data
        //
        depth::DepthSensor depth_map(depthFile);
        const auto& depthMat = depth_map.getDepthData();
        std::cout << "[OK] Depth map loaded: "
                  << depth_map.getWidth() << "x" << depth_map.getHeight() << " pixels" <<std::endl;
        uint16_t ppn_target = depth_map.getDepthMin();
        uint16_t dpn_target = depth_map.getDepthMax();
        std::cout << "Scene depth range: ["
                  << ppn_target << ", " << dpn_target << "] mm" <<std::endl;

        //
        // 2. Load calibration data and create Camera
        //
        cam::CalibrationData calib(calibFile);
        std::cout << "[OK] Calibration data loaded: "
                  << calib.getData().size() << " points" <<std::endl;
        cam::Camera camera(0, 100000, calib); // Just a random camera

        //
        // 3. Compute focus sequence based on depth map range
        //
        auto focusList = camera.computeFocusSequence(ppn_target, dpn_target);
        if (focusList.empty()) {
            std::cerr << "No suitable focus sequence found for this scene." <<std::endl;
            return 1;
        }
        std::cout << "[OK] Focus sequence: " << focusList.size() << " positions" <<std::endl;

        //
        // 4. Move focus camera motor and take snapshot
        //
        if (doDisplay) initializeDisplayWindow();
        cv::Mat overall_mask = cv::Mat::zeros(depthMat.size(), CV_8UC1);
        int i = 0;
        for (const auto& f : focusList) {
            std::cout << "Moving to focus position: "
                      << f.focusPosition <<std::endl;

            camera.setFocusPosition(f.focusPosition);

            // Simulate waiting time until focus is in position
            // TODO: separate thread for focus adjustment
            while(camera.getFocusPosition() != f.focusPosition);

            // Take snapshot picture
            const auto& mask = cam::Camera::takePictureMask(depthMat, f.ppn, f.dpn);
            
            // Generate output image per snapshot
            if (!mask.empty()) {
                if (doDisplay) displayImage(mask, 500);
                // Output png file per snapshot
                std::string filename = filename_no_ext + "_" + std::to_string(i) 
                    + "_"  + std::to_string(f.ppn) + "_" + std::to_string(f.dpn) + ".png";
                cv::imwrite(filename, mask);
                i++;
                std::cout << "SNAPSHOT ! @ position " << camera.getFocusPosition() <<" => " <<filename << std::endl;

                // Merge the depth masks
                cv::bitwise_or(overall_mask, mask, overall_mask);
            }
        }

        //
        // 5. Generate output overall images
        //
        if (doDisplay) displayImage(overall_mask, 1000);
        std::string filename = filename_no_ext + "_overall_"  + std::to_string(ppn_target) + "_" + std::to_string(dpn_target) + ".png";
        cv::imwrite(filename, overall_mask);
        std::cout << "[DONE] Focus sweep complete and merged => " <<filename <<std::endl;

        // Input depth file normalized into 8bits grey format just for visualization
        filename = filename_no_ext + "_input_greyed.png";
        cv::Mat greyed = cv::max(depthMat - depth_map.getDepthMin(), 0);
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