#include <gtest/gtest.h>

#include "FocusMotor.h"

#include <iostream>

using namespace motor;


TEST(FOCUSMOTOR, Creation)
{
    auto motor = FocusMotor(0, 500);
    ASSERT_EQ(0, motor.GetPosition());
    ASSERT_EQ(0, motor.GetMinPosition());
    ASSERT_EQ(500, motor.GetMaxPosition());
}

TEST(FOCUSMOTOR, Move)
{
    auto motor = FocusMotor(300, 600);
    ASSERT_EQ(300, motor.GetPosition());
    ASSERT_EQ(300, motor.GetMinPosition());
    ASSERT_EQ(600, motor.GetMaxPosition());

    motor.SetPosition(500);
    ASSERT_EQ(500, motor.GetPosition());
}