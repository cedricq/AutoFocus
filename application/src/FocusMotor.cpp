#include "FocusMotor.h"

namespace motor
{

FocusMotor::FocusMotor(int minPos, int maxPos)
        : minPosition_(minPos), maxPosition_(maxPos), currentPosition_(minPos)
    {}

void FocusMotor::setPosition(int position) {
    if (position < minPosition_ || position > maxPosition_) {
        throw std::out_of_range("FocusMotor: position out of range");
    }

    // For simulation: instant move
    currentPosition_ = position;
}

}

