#include "RedArrowMain.h"


#define LEFT_DRIVE_PWM 1
#define RIGHT_DRIVE_PWM 2
#define ROLLER_PWM 3
#define SHOOTER_PWM 4

#define FLAG_SERVO 9

#define LOWER_DI 1
#define UPPER_DI 2

#define LEFT_DRIVE_ENC_A 6
#define LEFT_DRIVE_ENC_B 7
#define RIGHT_DRIVE_ENC_A 8
#define RIGHT_DRIVE_ENC_B 9

#define GYRO 1
#define SHOOTER_POT 2



NextState AutonomousProgramA(BuiltinDefaultCode *robot, int32_t state)
{
	switch(state)
	{
	case 0:
		//robot->m_elevation->SetPosition(465);
		//robot->m_elevation->SetPosition(435);
		//robot->m_elevation->SetPosition(480);
		//robot->m_elevation->SetPosition(400);//425
		//robot->m_elevation->SetPosition(445);
		//robot->m_shooter->Set(1);
		//robot->m_feeder->ResetNumberOfFeeds();
		//robot->m_unjammer->Lower();
		//robot->m_robotDrive->SetSafetyEnabled(false);
		robot->m_robotDrive->DriveDistance(.1,2);
		//robot->m_robotDrive->Rotate(90);

		return NextState::EndState();
		break;
		/*case 1:
		if(robot->m_feeder->GetNumberOfFeeds() >= 3)
		{
			robot->m_shooter->Set(0);
			robot->m_unjammer->Raise();
			return NextState::EndState();
		}
		robot->m_feeder->Feed();
		return NextState(1,2,3);
		break;
		 */
	default:
		break;
	}
	return NextState::EndState();
};

/*NextState AutonomousProgramB(BuiltinDefaultCode *robot, int32_t state)
{
	switch(state)
	{
	case 0:
		robot->m_elevation->SetPosition(400);//480
		robot->m_shooter->Set(1);
		robot->m_feeder->ResetNumberOfFeeds();
		robot->m_unjammer->Lower();
		return NextState(1,2,5);
		break;
	case 1:
		if(robot->m_feeder->GetNumberOfFeeds() >= 2)
		{
			robot->m_shooter->Set(.9);
			return NextState(2,1,2);
		}
		robot->m_feeder->Feed();
		return NextState(1,3,3);
		break;
	case 2:
		robot->m_feeder->Feed();
		return NextState(3,3,3);
		break;
	case 3:
		robot->m_shooter->Set(0);
		robot->m_unjammer->Raise();
		return NextState::EndState();
		break;
	default:
		break;
		}
		return NextState::EndState();
	};
//FIX ARM CONTROL TO USE POT
NextState AutonomousProgramC(BuiltinDefaultCode *robot, int32_t state)
{
	switch(state)
	{
	case 0:
		robot->m_robotDrive->SetSafetyEnabled(false);
		robot->m_robotDrive->DriveDistance(-2);
		robot->m_feeder->ResetNumberOfFeeds();
		robot->m_elevation->SetPosition(510);
		return NextState(1,1);
		break;
	case 1:
		robot->m_shooter->Set(1);
#ifdef ARM_TYPE_POT
		robot->m_arm->SetPosition(200);
#else
		robot->m_arm->ArmDown();
#endif
		return NextState(2,2);
		break;
	case 2:
		if(robot->m_feeder->GetNumberOfFeeds() >= 3)
			{
			robot->m_shooter->Set(0);
			return NextState(3,2);
			}
			robot->m_feeder->Feed();
		return NextState(2,1);
		break;
	case 3:
		robot->m_elevation->SetPosition(450);
		robot->m_robotDrive->DriveDistance(2);
		return NextState(4,1);
		break;
	case 4:
#ifdef ARM_TYPE_POT
		robot->m_arm->SetPosition(100);
#else
		robot->m_arm->ArmUp();
#endif
		robot->m_feeder->ResetNumberOfFeeds();
		return NextState(5,1);
		break;
	case 5:
		robot->m_shooter->Set(1);
		return NextState(6,1);
		break;
	case 6:
		if(robot->m_feeder->GetNumberOfFeeds() >= 2)
		{
			robot->m_shooter->Set(0);
			return NextState::EndState();
		}
		robot->m_feeder->Feed();
		return NextState(6,2);
		break;
	default:
			break;
	}
	return NextState::EndState();
};
 */

/**
 * Constructor for this "BuiltinDefaultCode" Class.
 * 
 * The constructor creates all of the objects used for the different inputs and outputs of
 * the robot.  Essentially, the constructor defines the input/output mapping for the robot,
 * providing named objects for each of the robot interfaces. 
 */
BuiltinDefaultCode::BuiltinDefaultCode(void)	
{

	// Create a robot using standard right/left robot drive on PWMS 1, 2,
	m_robotDrive = //new RobotDrive(LEFT_DRIVE_PWM, RIGHT_DRIVE_PWM);
			new DriveTrain(LEFT_DRIVE_PWM, RIGHT_DRIVE_PWM,LEFT_DRIVE_ENC_A, LEFT_DRIVE_ENC_B, RIGHT_DRIVE_ENC_A, RIGHT_DRIVE_ENC_B, GYRO);

	m_shooter = new ShooterControl(SHOOTER_PWM, LOWER_DI, UPPER_DI, SHOOTER_POT);
	m_flag = new TwoStateServoControl(FLAG_SERVO, 0.66, 0.10);
	m_roller = new MotorControl(ROLLER_PWM, 1);
	

	// Initialize AutonomousManager
	m_autonomousManager = new AutonomousManager<BuiltinDefaultCode>(this, 0, 0);

	// Acquire the Driver Station object
	m_ds = DriverStation::GetInstance();

	m_autonomousModeChooser = new SendableChooser();

	// Define joysticks being used at USB port #1 and USB port #2 on the Drivers Station
	m_rightStick = new Joystick(1);
	m_leftStick = new Joystick(2);
	m_t1=new CxTimer();
	m_t1->Reset();

	ResetSubsystems();

}

BuiltinDefaultCode::~BuiltinDefaultCode(void)
{
	delete m_robotDrive;
	delete m_ds;
	delete m_rightStick;
	delete m_leftStick;
	delete m_flag;
	delete m_t1;
	delete m_shooter;
	delete m_autonomousManager;
	delete m_roller;

	//delete m_pot;
	delete m_autonomousModeChooser;
}


/********************************** Init Routines *************************************/

void BuiltinDefaultCode::RobotInit(void) 
{
	// Actions which would be performed once (and only once) upon initialization of the
	// robot would be put here.

	m_autonomousModeChooser->AddDefault("A: Test", new std::string("side"));
	m_autonomousModeChooser->AddObject("B: Test", new std::string("middle"));
	SmartDashboard::PutData("Autonomous Mode", m_autonomousModeChooser);
	//Camera Initalization




}

void BuiltinDefaultCode::DisabledInit(void) 
{
}

void BuiltinDefaultCode::AutonomousInit(void) 
{
	ResetSubsystems();
	//m_robotDrive->StartEncoders();
	std::string mode = *((std::string*)m_autonomousModeChooser->GetSelected());
	SmartDashboard::PutString("Autonomous Mode", mode);
	/*
	if(mode == "middle")
		m_autonomousManager->SetStartState(AutonomousProgramB, 0);
	else
		m_autonomousManager->SetStartState(AutonomousProgramA, 0);
	 */
	this->m_watchdog.SetExpiration(0.2);
	m_robotDrive->SetExpiration(0.2);
	m_autonomousManager->SetStartState(AutonomousProgramA, 0);
	//m_autonomousManager->SetStartState(AutonomousProgramB, 0);
	//m_autonomousManager->SetStartState(AutonomousProgramC, 0);
	m_robotDrive->StartEncoders();
}

void BuiltinDefaultCode::TeleopInit(void) 
{	
	ResetSubsystems();
	m_robotDrive->StartEncoders();
}

/********************************** Periodic Routines *************************************/

void BuiltinDefaultCode::DisabledPeriodic(void)  
{	
}

void BuiltinDefaultCode::AutonomousPeriodic(void) 
{
	CxTimer::Update();
	this->m_watchdog.Feed();
	m_autonomousManager->Run();
}


void BuiltinDefaultCode::TeleopPeriodic(void) 
{
	CxTimer::Update();

	GetDS();
	//bool leftJoystickIsUsed = false;



	m_robotDrive->ArcadeDrive(-RSy ,-RSx);			// drive with arcade style (use right stick)


	// do logic for decisions
	if(LS_B1)			//Shoot Ball
	{
		m_shooter->Shoot();
	}
	if(LS_B7)
	{
		m_shooter->SetStart();
	}
	if(LS_B4)
	{
		m_shooter->ManualControl(LSy);
	}
	if(LS_B2)
	{
		m_roller->SpinCounterClockwise();
		m_roller->SpeedAdjust(1.0);
	}
	else if(LS_B3)
	{
		m_roller->SpinClockwise();
		m_roller->SpeedAdjust(1.0);
	}
	
	m_shooter->SetPotDistance((int)(((LSz+1)/2)*225)+100);
	
	UpdateSubsystems();
} // TeleopPeriodic(void)

void BuiltinDefaultCode::GetDS()
{
	RSx=m_rightStick->GetX();
	RSy=-m_rightStick->GetY();
	RSz=m_rightStick->GetZ();
	RS_B1=m_rightStick->GetRawButton(1);
	RS_B2=m_rightStick->GetRawButton(2);
	RS_B3=m_rightStick->GetRawButton(3);
	RS_B4=m_rightStick->GetRawButton(4);
	RS_B5=m_rightStick->GetRawButton(5);
	RS_B6=m_rightStick->GetRawButton(6);
	RS_B7=m_rightStick->GetRawButton(7);
	RS_B8=m_rightStick->GetRawButton(8);
	RS_B9=m_rightStick->GetRawButton(9);
	RS_B10=m_rightStick->GetRawButton(10);
	RS_B11=m_rightStick->GetRawButton(11);

	LSx=m_leftStick->GetX();
	LSy=-m_leftStick->GetY();
	LSz=m_leftStick->GetZ();
	LS_B1=m_leftStick->GetRawButton(1);
	prev_LS_B2 = LS_B2;
	LS_B2=m_leftStick->GetRawButton(2);
	LS_B3=m_leftStick->GetRawButton(3);
	LS_B4=m_leftStick->GetRawButton(4);
	LS_B5=m_leftStick->GetRawButton(5);
	LS_B6=m_leftStick->GetRawButton(6);
	LS_B7=m_leftStick->GetRawButton(7);
	LS_B8=m_leftStick->GetRawButton(8);
	LS_B9=m_leftStick->GetRawButton(9);
	LS_B10=m_leftStick->GetRawButton(10);
	LS_B11=m_leftStick->GetRawButton(11);


	SmartDashboard::PutNumber("RSx",(double)RSx);
	SmartDashboard::PutNumber("RSy",(double)RSy);
	SmartDashboard::PutNumber("RSz", (double)RSz);

	SmartDashboard::PutNumber("LSx",(double)LSx);
	SmartDashboard::PutNumber("LSy",(double)LSy);
	SmartDashboard::PutNumber("LSz", (double)LSz);

	m_robotDrive->GetLeftEncoder();
	m_robotDrive->GetRightEncoder();

	SmartDashboard::PutNumber("CodeVersion", 6);

}

void BuiltinDefaultCode::ResetSubsystems()
{
	//m_setposition = 0;

	RSx=0;
	RSy=0;
	RSx=0;
	RS_B1=false;
	RS_B2=false;
	RS_B3=false;
	RS_B4=false;
	RS_B5=false;
	RS_B6=false;
	RS_B7=false;
	RS_B8=false;
	RS_B9=false;
	RS_B10=false;
	RS_B11=false;
	LSx=0;
	LSy=0;
	LSx=0;
	LS_B1=false;
	LS_B2=false;
	LS_B3=false;
	LS_B4=false;
	LS_B5=false;
	LS_B6=false;
	LS_B7=false;
	LS_B8=false;
	LS_B9=false;
	LS_B10=false;
	LS_B11=false;

	m_flag->Reset();
	m_robotDrive->Reset();
	m_shooter->Reset();
	m_roller->Reset();
}

bool BuiltinDefaultCode::UpdateSubsystems()
{
	//m_robotDrive->GetLeftEncoder();
	//m_robotDrive->GetRightEncoder();

	bool finished = true;
	finished = m_shooter->Update()    && finished;
	finished = m_flag->Update()   && finished;
	finished = m_robotDrive->Update() && finished;
	finished = m_roller->Update() && finished;
	return finished;

}

START_ROBOT_CLASS(BuiltinDefaultCode);
