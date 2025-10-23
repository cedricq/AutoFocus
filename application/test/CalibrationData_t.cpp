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
                  << ", DPN=" << p.dpn << std::endl;
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
                  << point->ppn << " - " << point->dpn << " mm)" << std::endl;
        ASSERT_EQ(30669, point->focusPosition);
        ASSERT_EQ(349, point->ppn);
        ASSERT_EQ(355, point->dpn);
    }
}

TEST(CALIB, ComputeFocusSequence)
{
    CalibrationData calib;
    ASSERT_TRUE(calib.loadFromCSV("data/calibration.csv"));
    const auto& data = calib.getData();

    int ppn_target = 393;
    int dpn_target = 411;

    auto focusList = calib.computeFocusSequence(ppn_target, dpn_target);
    ASSERT_EQ(3, focusList.size());

    std::cout << "Looking for focuses target matching the [" << ppn_target <<" " <<dpn_target  << "] mm range"<< std::endl;
    for (auto it = focusList.begin(); it != focusList.end(); ++it) {
        std::cout << it->focusPosition << " " << it->ppn << " " << it->dpn << std::endl;
    }

    // Just checking the 3 points output based on the ppn/dpn range
    ASSERT_EQ(23656, focusList[0].focusPosition);
    ASSERT_EQ(390, focusList[0].ppn);
    ASSERT_EQ(400, focusList[0].dpn);

    ASSERT_EQ(22366, focusList[1].focusPosition);
    ASSERT_EQ(401, focusList[1].ppn);
    ASSERT_EQ(411, focusList[1].dpn);

    ASSERT_EQ(21246, focusList[2].focusPosition);
    ASSERT_EQ(411, focusList[2].ppn);
    ASSERT_EQ(421, focusList[2].dpn);
}


TEST(CALIB, ComputeFocusSequence_NoOutput)
{
    CalibrationData calib;
    ASSERT_TRUE(calib.loadFromCSV("data/calibration.csv"));
    const auto& data = calib.getData();

    // ppn and dpn out of calibration possible data
    int ppn_target = 1512;
    int dpn_target = 3557;

    auto focusList = calib.computeFocusSequence(ppn_target, dpn_target);
    ASSERT_EQ(0, focusList.size());
}

TEST(CALIB, ComputeFocusSequence_ReversedInput)
{
    CalibrationData calib;
    ASSERT_TRUE(calib.loadFromCSV("data/calibration.csv"));
    const auto& data = calib.getData();

    // ppn and dpn in the wrong order
    int ppn_target = 411;
    int dpn_target = 393;

    auto focusList = calib.computeFocusSequence(ppn_target, dpn_target);
    ASSERT_EQ(0, focusList.size());
}