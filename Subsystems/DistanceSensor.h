#ifndef __First_2014__DistanceSensor__
#define __First_2014__DistanceSensor__

#include "WPILib.h"
#include "Relay.h"
#include "AnalogChannel.h"


class DistanceSensor
{
public:
	static const int kOn;
	static const int kOff;
	static const int kFlash;
	
	DistanceSensor(uint32_t outputID, uint32_t UltraSonicID);
	~DistanceSensor();
	
	void Reset();
	
	bool Update();
	
	
private:
	Relay *m_lightSwitch;
	AnalogChannel *m_ultraSonic; 
	
	int m_state;
	int m_optimum;
	int m_optimumRange;
	int m_flashRange;
	double m_minSecondsPerFlash;
	double m_lastLightToggleTime;
};

#endif
