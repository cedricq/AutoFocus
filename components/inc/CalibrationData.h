#ifndef CALIBRATIONDATA_H_
#define CALIBRATIONDATA_H_

#include <vector>
#include <string>

namespace cam
{

struct CalibrationPoint {
    int focusDistance;   // Main focus distance (mm)
    int aperture;        // Aperture (F-number)
    int focusPosition;   // Motor position value
    int ppn;             // Near limit of sharp focus (PPN, mm)
    int dpn;             // Far limit of sharp focus (DPN, mm)
    int iso;             // ISO sensitivity
    int speed;           // Shutter speed (1/s)
};

class CalibrationData {
public:
    CalibrationData(const std::string& filepath);
    ~CalibrationData() = default;

    /**
     * @brief Computes a sequence of calibration points for focus adjustment
     * 
     * @param ppn_target, dpn_target Target range for [PPN, DPN]
     * @return std::vector<CalibrationPoint> Vector containing the sequence of calibration points
     */
    std::vector<CalibrationPoint> computeFocusSequence(int ppn_target, int dpn_target) const;
    
    // Access all data points
    const std::vector<CalibrationPoint>& getData() const { return data_; }
    
    // Get calibration limits
    int getPPNMin() { return ppn_min_; }
    int getDPNMax() { return dpn_max_; }

private:
    std::vector<CalibrationPoint> data_ {};
    int ppn_min_ {0};
    int dpn_max_ {0};

};

} // namespace

#endif
