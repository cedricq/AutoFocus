#include <gtest/gtest.h>

#include "Camera.h"

#include <opencv2/opencv.hpp>
#include <iostream>

TEST(CAMERA, test)
{
    cv::Mat depth = (cv::Mat_<uint16_t>(3, 3) <<
        380, 395, 410,
        420, 440, 460,
        470, 480, 490
    );

    cv::Mat output = Camera::computeFocusMask(depth, 419, 461);

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

