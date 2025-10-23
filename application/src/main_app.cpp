#include "CalibrationData.h"
#include "DepthSensor.h"
#include "FocusMotor.h"

#include <iostream>
#include <string>
#include <stdexcept>


int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0]
                  << " <calibration.csv> <depth_map.png>" <<std::endl;
        return 1;
    }

    const std::string calibFile = argv[1];
    const std::string depthFile = argv[2];

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

        // Move motor and take snapshot
        //
        motor::FocusMotor motor(0, 100000); // Just a random motor
        for (const auto& f : focusList) {
            std::cout << "Moving to focus position: "
                      << f.focusPosition << "  (PPN=" << f.ppn 
                      << ", DPN=" << f.dpn << ")" <<std::endl;

            motor.SetPosition(f.focusPosition);

            // TODO: Wait until motor is in position & Timeout & Likely running on a different thread
            while(motor.GetPosition() != f.focusPosition);

            // Take snapshot picture
            std::cout << "SNAPSHOT ! @ position " << motor.GetPosition() << std::endl;
        }

        std::cout << "[DONE] Focus sweep complete." <<std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}