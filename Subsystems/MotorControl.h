#ifndef __First_2014__MotorControl__
#define __First_2014__MotorControl__

#include "Talon.h"
//#include "DigitalInput.h"

#define STOP 0
#define SPINCOUNTERCLOCKWISE 1
#define SPINCLOCKWISE 2
#define STARTCOUNTERCLOCKWISE 3
#define STARTCLOCKWISE 4
#define MANUALCONTROL 5

class MotorControl
{
public:
	MotorControl(uint32_t outputChannel, float speedlimit = 1);
	~MotorControl();
	
	void Reset();
	
	void SpinCounterClockwise();
	void SpinClockwise();
	
	void SpinStartCounterClockwise();
	void SpinStartClockwise();
	void SpinStop();
	
	void SpeedAdjust(float speed);
	
	void ManualControl(float speed);
	
	bool Update();
	
private:
	Talon *m_motor;
	
	float m_speed;
	float m_maxSpeed;
	int m_state;
	
		
};

#endif
