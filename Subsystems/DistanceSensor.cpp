#include "DistanceSensor.h"
#include "math.h"

const int DistanceSensor::kOn = 0;
const int DistanceSensor::kOff = 1;
const int DistanceSensor::kFlash = 2;

DistanceSensor::DistanceSensor(uint32_t outputID, uint32_t UltraSonicID)
:m_lightSwitch(new Relay(outputID, Relay::kForwardOnly)),
 m_ultraSonic(new AnalogChannel(UltraSonicID)),
 m_state(kOff), 
 m_optimum(140),
 m_optimumRange(10),
 m_flashRange(60),
 m_secondsPerFlash(1),
 m_lastLightToggleTime(-1)
{

}

DistanceSensor::~DistanceSensor()
{
	delete m_ultraSonic;
	delete m_lightSwitch;
}

void DistanceSensor::Reset()
{
	m_state = kOff;
}

bool DistanceSensor::Update()
{
	double currentTime = Timer::GetPPCTimestamp();
	int relativePosition = m_ultraSonic->GetAverageValue() - m_optimum;
	
	if(abs(relativePosition) < m_optimumRange)
	{
		m_state = kOn;
		m_lightSwitch->Set(Relay::kOn);
	}
	else if(abs(relativePosition) > m_flashRange)
	{
		m_state = kOff;
		m_lightSwitch->Set(Relay::kOff);
	}
	else
	{
		double relativeRange = (abs(relativePosition)-(double)m_optimumRange)/((double)m_flashRange);
		m_secondsPerFlash = 0.25 + relativeRange;
		if(currentTime > m_lastLightToggleTime + m_secondsPerFlash)
		{
			if(m_lightSwitch->Get()== Relay::kOn)
			{
				m_lightSwitch->Set(Relay::kOff);
			}
			else
			{
				m_lightSwitch->Set(Relay::kOn);
			}

			m_lastLightToggleTime = currentTime;
		}
	}
	SmartDashboard::PutNumber("secondsPerFlash", m_secondsPerFlash );

	SmartDashboard::PutNumber("Ultrasonic", m_ultraSonic->GetValue());
	SmartDashboard::PutBoolean("LIGHTS On", m_lightSwitch->Get()==Relay::kOn);
	return true;
}
