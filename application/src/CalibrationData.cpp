#include "CalibrationData.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

namespace calib
{

bool CalibrationData::loadFromCSV(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << filepath << std::endl;
        return false;
    }

    std::string line;
    std::getline(file, line); // Skip the header line

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        CalibrationPoint point;
        std::string token;

        // Read columns separated by commas
        std::getline(ss, token, ','); point.focusDistance = std::stod(token);
        std::getline(ss, token, ','); point.aperture      = std::stod(token);
        std::getline(ss, token, ','); point.focusPosition = std::stod(token);
        std::getline(ss, token, ','); point.ppn           = std::stod(token);
        std::getline(ss, token, ','); point.dpn           = std::stod(token);
        std::getline(ss, token, ','); point.iso           = std::stoi(token);
        std::getline(ss, token, ','); point.speed         = std::stod(token);

        data_.push_back(point);

        // Record min ppn and max dpn possible values
        if (ppn_min_ > point.ppn || ppn_min_ == 0)
        {
            ppn_min_ = point.ppn;
        }
        if (dpn_max_ < point.dpn || dpn_max_ == 0)
        {
            dpn_max_ = point.dpn;
        }
    }
    return true;
}

std::vector<CalibrationPoint> CalibrationData::computeFocusSequence(int ppn, int dpn) const {
    
    std::vector<CalibrationPoint> focusList;

    int ppn_target = std::max(ppn, ppn_min_);
    int dpn_target = std::min(dpn, dpn_max_);

    if (data_.empty()) {
        std::cerr << "Error: no calibration data loaded." << std::endl;
        return focusList;
    }

    // Sort calibration points by their near sharp limit (PPN)
    std::vector<CalibrationPoint> sorted = data_;
    std::sort(sorted.begin(), sorted.end(),
        [](const CalibrationPoint& a, const CalibrationPoint& b) {
            return a.ppn < b.ppn;
        });

    // Find first calibration point whose range covers the current depth
    auto it = std::find_if(sorted.begin(), sorted.end(),
        [ppn_target](const CalibrationPoint& p) {
            return ( p.ppn <= ppn_target && p.dpn >= ppn_target );
        });

    // No focus available in the calibration table => returns empty list
    if (it == sorted.end()) {
        std::cerr << "Warning: no calibration covers depth " << ppn_target << " mm." << std::endl;
        return focusList;
    }

    // Finds all the focuses within the depth range
    while (it != sorted.end() && it->dpn >= ppn_target && it->ppn <= dpn_target) {
        focusList.push_back(*it);
        ++it;
    }

    return focusList;
}

}

