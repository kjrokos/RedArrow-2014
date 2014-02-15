#include "DistanceSensor.h"
#include "Preferences.h"
#include "math.h"

const int DistanceSensor::kOn = 0;
const int DistanceSensor::kOff = 1;
const int DistanceSensor::kFlash = 2;

DistanceSensor::DistanceSensor(uint32_t outputID, uint32_t UltraSonicID)
:m_lightSwitch(new Relay(outputID, Relay::kForwardOnly)),
 m_ultraSonic(new AnalogChannel(UltraSonicID)),
 m_state(kOff), 
 m_optimum(67),
 m_optimumRange(3),
 m_flashRange(28),
 m_minSecondsPerFlash(.25),
 m_lastLightToggleTime(-1)
{
	this->Reset();
}

DistanceSensor::~DistanceSensor()
{
	delete m_ultraSonic;
	delete m_lightSwitch;
}

void DistanceSensor::Reset()
{
	Preferences *preferences = Preferences::GetInstance();
	m_optimum = preferences->GetInt("Ultrasonic_Optimum", m_optimum);
	m_optimumRange = preferences->GetInt("Ultrasonic_OptimumRange", m_optimumRange);
	m_flashRange = preferences->GetInt("Ultrasonic_FlashRange", m_flashRange);

	m_state = kOff;
}

bool DistanceSensor::Update()
{
	int ultrasonicValue = m_ultraSonic->GetAverageValue();
	double currentTime = Timer::GetPPCTimestamp();
	int optimumVoltage = (int) (1.855*m_optimum-1);
	int optimumVoltageRange = (int) (1.855*m_optimumRange-1);
	int flashVoltageRange = (int) (1.855*m_flashRange-1);
	int relativePosition = ultrasonicValue - optimumVoltage;
	
	if(abs(relativePosition) < optimumVoltageRange)
	{
		m_state = kOn;
		m_lightSwitch->Set(Relay::kOn);
	}
	else if(abs(relativePosition) > flashVoltageRange)
	{
		m_state = kOff;
		m_lightSwitch->Set(Relay::kOff);
	}
	else
	{
		double relativeRange = (abs(relativePosition)-(double)optimumVoltageRange)/((double)flashVoltageRange);
		double secondsPerFlash = m_minSecondsPerFlash + relativeRange;
		SmartDashboard::PutNumber("secondsPerFlash", secondsPerFlash);
		
		if(currentTime > m_lastLightToggleTime + secondsPerFlash)
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
	

	SmartDashboard::PutNumber("Ultrasonic", (ultrasonicValue/1.855)+1);
	SmartDashboard::PutBoolean("LIGHTS On", m_lightSwitch->Get()==Relay::kOn);
	return true;
}
