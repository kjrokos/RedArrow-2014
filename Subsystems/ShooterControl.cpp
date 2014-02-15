#include "ShooterControl.h"
#include "SmartDashboard/SmartDashboard.h"

const int ShooterControl::kStop = 0;
const int ShooterControl::kShoot = 1;
const int ShooterControl::kReset = 2;
const int ShooterControl::kManualControl = 3;
const int ShooterControl::kSetStartPosition = 4;



ShooterControl::ShooterControl(uint32_t outputChannel, uint32_t lowerLimitSwitchChannel, uint32_t upperLimitSwitchChannel, uint32_t positionChannel)
	:m_shooter (new Talon(outputChannel)),
	m_stflag(kStop),
	m_lowerPosition(new DigitalInput(lowerLimitSwitchChannel)),
	m_upperPosition(new DigitalInput(upperLimitSwitchChannel)),
	m_pot(new AnalogChannel(positionChannel)), 
	m_potStart(m_pot->GetValue()),
	m_potDistance(382-200), 
	m_setStart(kSetStartPosition)
{
	
}

ShooterControl::~ShooterControl()
{
	delete m_shooter;
	delete m_lowerPosition;
	delete m_upperPosition;
	delete m_pot;
}
void ShooterControl::Reset()
{
	m_stflag = kStop;
}

void ShooterControl::SetStart()
{
	m_setStart = kSetStartPosition;
}

void ShooterControl::Shoot()
{
	if(m_stflag == kStop)
	{
		m_stflag = kShoot;
	}
}

void ShooterControl::ManualControl(float speed)
{
	m_stflag = kManualControl;
	m_speed = speed;
}
bool ShooterControl::Update()
{	
	float relativePosition = m_pot->GetValue() - m_potStart;
	SmartDashboard::PutNumber("Shooter Pot", m_pot->GetValue());
	SmartDashboard::PutNumber("Start: ", m_potStart);
	SmartDashboard::PutNumber("Distance: ", m_potDistance);
	SmartDashboard::PutNumber("relaPot: ", relativePosition);
	if(m_setStart == kSetStartPosition)
	{
		m_potStart = m_pot->GetValue();
		m_setStart = kStop;
	}
	if(m_setStart == kStop)
	{
		
	}
	
	if(m_stflag == kShoot)
	{
		if(relativePosition < m_potDistance)
		{
			m_speed = 1;
		}
		/*if(relativePosition <= -m_potDistance +100 && relativePosition > -m_potDistance)
		{
			m_speed = .6;
		}
		*/
		if(relativePosition > m_potDistance)
		{
			m_stflag = kReset;
		}
	}
	
	if(m_stflag == kReset)
	{
		if(relativePosition > 35)
			m_speed = -.8;
		if(relativePosition > 0 && relativePosition <= 35)
			m_speed = -.2;
		if(relativePosition <= 0)
			m_stflag = kStop;
	}
	if(m_stflag == kStop)
		m_speed = 0;
	if(m_stflag == kManualControl)
		m_stflag = kStop;
		/*
	if(!m_lowerPosition->Get() && m_speed < 0)
	{
		m_speed = 0;
		m_stflag = kStop;
	}
	if(!m_upperPosition->Get() && m_speed > 0)
	{
		m_speed = 0;
		
	}
	*/
	m_shooter->Set(m_speed);
	
	return m_stflag == kStop;
}

