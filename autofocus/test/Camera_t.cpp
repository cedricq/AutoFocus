#include <gtest/gtest.h>

#include "Camera.h"

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cam;


TEST(CAMERA, Creation)
{
    auto calib = CalibrationData ("data/calibration.csv");
    auto camera = Camera(0, 500, calib);
    ASSERT_EQ(0, camera.getFocusPosition());
    ASSERT_EQ(0, camera.getMinFocusPosition());
    ASSERT_EQ(500, camera.getMaxFocusPosition());
}

TEST(CAMERA, Move)
{
    auto calib = CalibrationData ("data/calibration.csv");
    auto camera = Camera(300, 600, calib);
    ASSERT_EQ(300, camera.getFocusPosition());
    ASSERT_EQ(300, camera.getMinFocusPosition());
    ASSERT_EQ(600, camera.getMaxFocusPosition());

    camera.setFocusPosition(500);
    ASSERT_EQ(500, camera.getFocusPosition());
}

TEST(CAMERA, DepthMask)
{
    cv::Mat depth = (cv::Mat_<uint16_t>(3, 3) <<
        380, 395, 410,
        420, 440, 460,
        470, 480, 490
    );

    cv::Mat output = Camera::takePictureMask(depth, 419, 461);

    cv::Mat expected = (cv::Mat_<uint8_t>(3, 3) <<
        0, 0, 0,
        255, 255, 255,
        0, 0, 0
    );

    std::cout << "Expected matrix:" <<std::endl << expected << std::endl;
    std::cout << "Output matrix:" <<std::endl << output << std::endl;

    ASSERT_EQ(expected.size(), output.size()) << "Matrix sizes differ";
    ASSERT_EQ(expected.type(), output.type()) << "Matrix types differ";

    cv::Mat diff;
    cv::compare(expected, output, diff, cv::CMP_NE);  // diff=1 where unequal
    int nDiff = cv::countNonZero(diff);

    ASSERT_EQ(0, nDiff);
}

TEST(CAMERA, DepthMaskEmpty)
{
    cv::Mat depth = (cv::Mat_<uint16_t>(3, 3) <<
        380, 395, 410,
        420, 440, 460,
        470, 480, 490
    );

    cv::Mat output = Camera::takePictureMask(depth, 505, 508);
    ASSERT_TRUE(output.empty()) << "Output is not empty";
}

