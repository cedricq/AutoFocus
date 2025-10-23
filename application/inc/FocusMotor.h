#ifndef FOCUSMOTOR_H_
#define FOCUSMOTOR_H_

#include <stdexcept>

namespace motor
{

class FocusMotor {
public:
    FocusMotor(int minPos, int maxPos);

    // Simulate setting the motor position
    void SetPosition(int position);

    // Simulate reading the current position
    int GetPosition() const {
        return currentPosition_;
    }

    // Optional: expose limits
    int GetMinPosition() const { return minPosition_; }
    int GetMaxPosition() const { return maxPosition_; }

private:
    int minPosition_;
    int maxPosition_;
    int currentPosition_;
};

} // namespace

#endif
