#include "MotorControl.h"

MotorControl::MotorControl(uint32_t outputChannel, float speedlimit)
{
	m_motor = new Talon(outputChannel);
	Reset();
	m_maxSpeed = speedlimit;
}

MotorControl::~MotorControl()
{
	delete m_motor;
}

void MotorControl::Reset()
{	
	m_state = STOP;
	m_speed = 0;
	m_motor->Set(0);
}

void MotorControl::SpinCounterClockwise()
{
	m_state = SPINCOUNTERCLOCKWISE;
}

void MotorControl::SpinClockwise()
{
	m_state = SPINCLOCKWISE;
}

void MotorControl::SpinStartCounterClockwise()
{
	m_state = STARTCOUNTERCLOCKWISE;
}

void MotorControl::SpinStartClockwise()
{
	m_state = STARTCLOCKWISE;
}

void MotorControl::SpinStop()
{
	m_state = STOP;
}

void MotorControl::SpeedAdjust(float speed)
{
	m_speed = speed*m_maxSpeed;
}

void MotorControl::ManualControl(float speed)
{
	m_speed = speed*m_maxSpeed;
	m_state = MANUALCONTROL;
}

bool MotorControl::Update()
{
	switch(m_state) {
	case SPINCOUNTERCLOCKWISE:
		m_motor->Set(m_speed);
		m_state = STOP;
		break;
	case SPINCLOCKWISE:
		m_motor->Set(-m_speed);
		m_state = STOP;
		break;
	case STARTCOUNTERCLOCKWISE:
		m_motor->Set(m_speed);
		break;
	case STARTCLOCKWISE:
		m_motor->Set(-m_speed);
		printf("is working\n");
		break;
	case MANUALCONTROL:
		m_motor->Set(m_speed);
		m_state = STOP;
		break;
	case STOP:
		m_motor->Set(0);
		break;
	default:
		Reset();
		break;
	}
	
	return true;
}

