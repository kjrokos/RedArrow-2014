#ifndef __First_2013__ShooterControl__
#define __First_2013__ShooterControl__

#include "Talon.h"
#include "DigitalInput.h"
#include "AnalogChannel.h"

class ShooterControl
{
public:
	static const int kStop;
	static const int kShoot;
	static const int kReset;
	static const int kManualControl;
	static const int kSetStartPosition;
public:
	ShooterControl(uint32_t outputChannel, uint32_t lowerLimitSwitchChannel, uint32_t upperLimitSwitchChannel, uint32_t positionChannel);
	~ShooterControl();
	
	void SetPotDistance(int distance) {m_potDistance=distance;};
	void Reset();
	
	void SetStart();
	
	void Shoot();
	
	void ManualControl(float speed);
	bool Update();
	
private:
	Talon *m_shooter;
	
	float m_speed;
	int m_stflag;
	
	DigitalInput *m_lowerPosition;
	DigitalInput *m_upperPosition;
	AnalogChannel *m_pot;
	int m_potStart;
	int m_potDistance;
	int m_setStart;
	
};

#endif
