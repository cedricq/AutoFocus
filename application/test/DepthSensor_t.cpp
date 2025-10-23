#include <gtest/gtest.h>

#include "DepthSensor.h"

#include <iostream>

using namespace depth;


TEST(DEPTHSENSOR, LoadingDepthImage)
{
    DepthSensor sensor("data/depth_1.png");

    std::cout << "Loaded depth image: " 
                  << sensor.getWidth() << "x" << sensor.getHeight() << std::endl;

    std::cout << "Depth image: [ " 
                  << sensor.getDepthMin() << " " << sensor.getDepthMax() << " ]" << std::endl;

    ASSERT_EQ(1920, sensor.getWidth());
    ASSERT_EQ(1080, sensor.getHeight());
    ASSERT_EQ(0, sensor.getDepthMin());
    ASSERT_EQ(397, sensor.getDepthMax());
    ASSERT_EQ(310, sensor.at(1000, 1000));
}

