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
 m_optimum(57),
 m_optimumRange(9)
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
	
	m_state = kOff;
}

bool DistanceSensor::Update()
{
	SmartDashboard::PutNumber("Ultrasonic_Optimum", m_optimum);
	SmartDashboard::PutNumber("Ultrasonic_OptimumRange", m_optimumRange);
	int ultrasonicValue = m_ultraSonic->GetAverageValue();
	int optimumVoltage = (int) (1.855*m_optimum-1);
	int optimumVoltageRange = (int) (1.855*m_optimumRange-1);
	int relativePosition = ultrasonicValue - optimumVoltage;
	
	if(abs(relativePosition) < optimumVoltageRange)
	{
		m_state = kOn;
		m_lightSwitch->Set(Relay::kOn);
	}
	else 
	{
		m_state = kOff;
		m_lightSwitch->Set(Relay::kOff);
	}

	

	SmartDashboard::PutNumber("Ultrasonic", ((ultrasonicValue/1.855)+1));
	SmartDashboard::PutBoolean("LIGHTS On", m_lightSwitch->Get()==Relay::kOn);
	return true;
}
