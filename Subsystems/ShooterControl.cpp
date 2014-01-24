#include "ShooterControl.h"


ShooterControl::ShooterControl(uint32_t outputChannel)
	:m_shooter (new Talon(outputChannel)),
	m_stflag(0),
	m_cnt(0)
{
	
}

ShooterControl::~ShooterControl()
{
	delete m_shooter;
}
void ShooterControl::Reset()
{
	m_stflag = 0;
	m_cnt = 0;
}
void ShooterControl::Shoot()
{
	if(m_stflag == 0)
	{
		m_stflag = 1;
		m_cnt = 0;
	}
}
bool ShooterControl::Update()
{
	if(m_stflag == 1)
		{
			if(m_cnt > 30)
			{
				m_shooter->Set(0);
				m_stflag=0;
			}
			if(m_cnt ==0)
				m_shooter->Set(0);
			if(m_cnt >0 && m_cnt <=10)
				m_shooter->Set(0.80);
			//if(m_cnt >10 && m_cnt<=15)
				//m_shooter->Set(.5);
			//if(m_cnt >13 && m_cnt<=15)
			//	m_shooter->Set(.5-((m_cnt-13)/50) * .14);
			//if(m_cnt >15 && m_cnt<=25)
				//m_shooter->Set(-.80);
			if(m_cnt >10 && m_cnt<=30)
				m_shooter->Set(-.40);
			m_cnt++;
		}
	else
	{
		m_shooter->Set(0);
	}
	return !m_stflag;
}
