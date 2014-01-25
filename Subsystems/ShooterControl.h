#ifndef __First_2013__ShooterControl__
#define __First_2013__ShooterControl__

#include "Talon.h"
#include "DigitalInput.h"

class ShooterControl
{
public:
	ShooterControl(uint32_t outputChannel, uint32_t lowerLimitSwitchChannel, uint32_t upperLimitSwitchChannel);
	~ShooterControl();
	
	void Reset();
	
	void Shoot();
	
	void ManualControl(float speed);
	bool Update();
	
private:
	Talon *m_shooter;
	
	float m_speed;
	int m_stflag;
	int m_cnt;
	
	DigitalInput *m_lowerPosition;
	DigitalInput *m_upperPosition;
};

#endif
