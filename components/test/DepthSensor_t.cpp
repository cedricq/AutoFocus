#include <gtest/gtest.h>

#include "DepthSensor.h"

#include <iostream>

using namespace depth;


TEST(DEPTHSENSOR, LoadingDepthImage)
{
    DepthSensor depth_map("data/depth_1.png");

    std::cout << "Loaded depth image: " 
                  << depth_map.getWidth() << "x" << depth_map.getHeight() << std::endl;

    std::cout << "Depth image: [ " 
                  << depth_map.getDepthMin() << " " << depth_map.getDepthMax() << " ]" << std::endl;

    ASSERT_EQ(1920, depth_map.getWidth());
    ASSERT_EQ(1080, depth_map.getHeight());
    ASSERT_EQ(292, depth_map.getDepthMin());
    ASSERT_EQ(397, depth_map.getDepthMax());
    ASSERT_EQ(310, depth_map.at(1000, 1000));
}

