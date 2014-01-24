#ifndef __First_2013__ShooterControl__
#define __First_2013__ShooterControl__

#include "Talon.h"
//#include "DigitalInput.h"

class ShooterControl
{
public:
	ShooterControl(uint32_t outputChannel);
	~ShooterControl();
	
	void Reset();
	
	void Shoot();
	
	bool Update();
	
private:
	Talon *m_shooter;
	
	int m_stflag;
	int m_cnt;
		
};

#endif
