#include "ShooterControl.h"


ShooterControl::ShooterControl(uint32_t outputChannel, uint32_t lowerLimitSwitchChannel, uint32_t upperLimitSwitchChannel, uint32_t positionChannel)
	:m_shooter (new Talon(outputChannel)),
	m_stflag(0),
	m_lowerPosition(new DigitalInput(lowerLimitSwitchChannel)),
	m_upperPosition(new DigitalInput(upperLimitSwitchChannel)),
	m_pot(new AnalogChannel(positionChannel))
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
	m_stflag = 0;
}
void ShooterControl::Shoot()
{
	if(m_stflag == 0)
	{
		m_stflag = 1;
	}
}

void ShooterControl::ManualControl(float speed)
{
	m_stflag = 2;
	m_speed = speed;
}
bool ShooterControl::Update()
{	
	if(m_stflag == 1)
		{
			if()
			{
				m_shooter->Set(0);
				m_stflag=0;
			}
			if()
				m_speed=0;
			if()
							m_speed = .80;
			if()
				m_speed = 1.0;
			//if(m_cnt >10 && m_cnt<=15)
				//m_shooter->Set(.5);
			//if(m_cnt >13 && m_cnt<=15)
			//	m_shooter->Set(.5-((m_cnt-13)/50) * .14);
			//if(m_cnt >15 && m_cnt<=25)
				//m_shooter->Set(-.80);
			if()
				m_speed = -.30;
			
		}
	else if (m_stflag == 0)
	{
		m_speed = 0;
	}
	else if (m_stflag == 2)
	{
		m_stflag=0;
	}
	
	if(!m_lowerPosition->Get() && m_speed < 0)
	{
		m_speed = 0;
		m_stflag = 0;
	}
	if(!m_upperPosition->Get() && m_speed > 0)
	{
		m_speed = 0;
		
	}
	
	m_shooter->Set(m_speed);
	
	return m_stflag == 0;
}

