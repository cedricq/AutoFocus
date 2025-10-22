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
        std::cerr << "Error: could not open file " << filepath << "\n";
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
    }

    std::cout << "✅ Calibration file loaded: " << data_.size() << " rows.\n";
    return true;
}

const CalibrationPoint* CalibrationData::findClosestFocus(int focusPosition) const {
    if (data_.empty()) return nullptr;

    const CalibrationPoint* closest = &data_[0];
    double minDiff = std::abs(focusPosition - data_[0].focusPosition);

    for (const auto& p : data_) {
        double diff = std::abs(focusPosition - p.focusPosition);
        if (diff < minDiff) {
            minDiff = diff;
            closest = &p;
        }
    }
    return closest;
}

std::vector<int> CalibrationData::computeFocusSequence(int ppn_target, int dpn_target) const {
    std::vector<int> focusList;

    if (data_.empty()) {
        std::cerr << "Error: no calibration data loaded.\n";
        return focusList;
    }

    // Sort calibration points by their near sharp limit (PPN)
    std::vector<CalibrationPoint> sorted = data_;
    std::sort(sorted.begin(), sorted.end(),
              [](const CalibrationPoint& a, const CalibrationPoint& b) {
                  return a.ppn < b.ppn;
              });

    double current = ppn_target;

    while (current < dpn_target) {
        // Find first calibration point whose range covers the current depth
        auto it = std::find_if(sorted.begin(), sorted.end(),
            [current](const CalibrationPoint& p) {
                return (p.ppn <= current && p.dpn >= current);
            });

        if (it == sorted.end()) {
            std::cerr << "Warning: no calibration covers depth " << current << " mm.\n";
            break;
        }

        focusList.push_back(it->focusPosition);
        current = it->dpn; // jump to the next uncovered depth
    }

    return focusList;
}

}

