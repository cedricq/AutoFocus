#include <gtest/gtest.h>

#include "FocusMotor.h"

#include <iostream>

using namespace motor;


TEST(FOCUSMOTOR, Creation)
{
    auto motor = FocusMotor(0, 500);
    ASSERT_EQ(0, motor.getPosition());
    ASSERT_EQ(0, motor.getMinPosition());
    ASSERT_EQ(500, motor.getMaxPosition());
}

TEST(FOCUSMOTOR, Move)
{
    auto motor = FocusMotor(300, 600);
    ASSERT_EQ(300, motor.getPosition());
    ASSERT_EQ(300, motor.getMinPosition());
    ASSERT_EQ(600, motor.getMaxPosition());

    motor.setPosition(500);
    ASSERT_EQ(500, motor.getPosition());
}