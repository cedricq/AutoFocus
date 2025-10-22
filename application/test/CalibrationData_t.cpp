#include <gtest/gtest.h>

#include "CalibrationData.h"

#include <iostream>

using namespace calib;


TEST(CALIB, LoadCSV)
{
    CalibrationData calib;
    ASSERT_TRUE(calib.loadFromCSV("data/calibration.csv"));

    const auto& data = calib.getData();
    ASSERT_EQ(36, data.size());

    ASSERT_FALSE(data.empty());
    if (!data.empty()) {
        const auto& p = data.front();
        std::cout << "  FocusPos=" << p.focusPosition
                  << ", PPN=" << p.ppn
                  << ", DPN=" << p.dpn << "\n";
        ASSERT_EQ(31826, p.focusPosition);
        ASSERT_EQ(343, p.ppn);
        ASSERT_EQ(349, p.dpn);
    }
}


TEST(CALIB, FindFocus)
{
    CalibrationData calib;
    ASSERT_TRUE(calib.loadFromCSV("data/calibration.csv"));

    const auto& data = calib.getData();

    double targetFocus = 30000;
    auto point = calib.findClosestFocus(targetFocus);
    ASSERT_NE(nullptr, point);
    if (point) {
        std::cout << "Closest to " << targetFocus << ": "
                  << point->focusPosition << " (sharp range "
                  << point->ppn << " - " << point->dpn << " mm)\n";
        ASSERT_EQ(30669, point->focusPosition);
        ASSERT_EQ(349, point->ppn);
        ASSERT_EQ(355, point->dpn);
    }
}
