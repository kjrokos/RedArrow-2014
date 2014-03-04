#include "RedArrowMain.h"


#define LEFT_DRIVE_PWM 2
#define RIGHT_DRIVE_PWM 1
#define ROLLER_PWM 3
#define SHOOTER_PWM 4
#define WINCH_PWM 5

#define FLAG_SERVO 10
#define GRIP_LEFT 9
#define GRIP_RIGHT 8

#define LOWER_DI 1
#define UPPER_DI 2

#define LEFT_DRIVE_ENC_A 7
#define LEFT_DRIVE_ENC_B 6
#define RIGHT_DRIVE_ENC_A 9
#define RIGHT_DRIVE_ENC_B 8

#define GYRO 1
#define SHOOTER_POT 2
#define ULTRASONIC_SENSOR 3

#define LIGHT_RELAY 1

NextState AutonomousProgramA(BuiltinDefaultCode *robot, int32_t state)
{
	switch(state)
	{
	case 0:
		robot->m_robotDrive->DriveDistance(3.812,5);
		robot->m_roller->SpeedAdjust(1);
		robot->m_roller->SpinStartClockwise();
		return NextState(1,2,6);
		break;
	case 1:
		robot->m_shooter->Shoot();
		return NextState(2,1,2);
	case 2:
		robot->m_roller->SpinStop();
		return NextState::EndState();
	default:
		break;
	}
	return NextState::EndState();
};

NextState AutonomousProgramB(BuiltinDefaultCode *robot, int32_t state)
{
	switch(state)
		{
		case 0:
			robot->m_robotDrive->DriveDistance(3.812,4);
			robot->m_roller->SpeedAdjust(1);
			robot->m_roller->SpinStartClockwise();
			return NextState(1,2,4);
			break;
		case 1:
			robot->m_shooter->Shoot();
			return NextState(2,1,2);
		case 2:
			robot->m_robotDrive->DriveDistance(-4, 4);
			return NextState(3,1,4);
		case 3:
			robot->m_robotDrive->DriveDistance(4, 4);
			robot->m_roller->SpeedAdjust(1);
			robot->m_roller->SpinStartClockwise();
			return NextState(4,2,4);
			break;
		case 4:
			robot->m_shooter->Shoot();
			return NextState::EndState();
		default:
			break;
		}
		return NextState::EndState();
}

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
	m_flag = new TwoStateServoControl(FLAG_SERVO, 0.66, 0.05);
	m_roller = new MotorControl(ROLLER_PWM, 1);
	m_gripLeft = new TwoStateServoControl(GRIP_LEFT, .9, .8);
	m_gripRight = new TwoStateServoControl(GRIP_RIGHT, .9, 1);
	m_distanceSensor = new DistanceSensor(LIGHT_RELAY, ULTRASONIC_SENSOR);
	m_winch = new MotorControl(WINCH_PWM , 1);

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
	delete m_winch;

	delete m_distanceSensor;
	//delete m_pot;
	delete m_autonomousModeChooser;
}


/********************************** Init Routines *************************************/

void BuiltinDefaultCode::RobotInit(void) 
{
	// Actions which would be performed once (and only once) upon initialization of the
	// robot would be put here.

	m_autonomousModeChooser->AddDefault("A: One Ball", new std::string("one"));
	m_autonomousModeChooser->AddObject("B: Two Balls", new std::string("two"));
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
	this->m_watchdog.SetExpiration(0.2);
	m_robotDrive->SetExpiration(0.2);
		
	if(mode == "one")
	{
		m_autonomousManager->SetStartState(AutonomousProgramA, 0);
	}
	else 
	{
		m_autonomousManager->SetStartState(AutonomousProgramB, 0);
	}
	
	//m_autonomousManager->SetStartState(AutonomousProgramA, 0);
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

	m_robotDrive->ManualControl(RSy ,-RSx, ((RSz+1)/2));			// drive with arcade style (use right stick)

	if(LS_B8 || RS_B8)
	{
		m_flag->Lower();
	}
	if(LS_B9 || RS_B9)
	{
		m_flag->Raise();
	}
	if(RS_B6)
	{
		m_gripLeft->Raise();
		m_gripRight->Raise();
	}
	if(RS_B7)
	{
		m_gripLeft->Lower();
		m_gripRight->Lower();
	}
			
	// do logic for decisions
	if(LS_B1)			//Shoot Ball
	{
		m_shooter->Shoot();
	}
	if(LS_B5)
	{
		m_shooter->SoftShoot();
	}
	if(LS_B7)
	{
		m_shooter->SetStart();
	}
	if(LS_B4)
	{
		m_shooter->ManualControl(LSy);
	}
	if(RS_B3)
	{
		m_roller->SpinCounterClockwise();
		m_roller->SpeedAdjust(1.0);
	}
	if(RS_B2 || RS_B1)
	{
		m_roller->SpinClockwise();
		m_roller->SpeedAdjust(1.0);
	}
	if(LS_B2)
	{
		m_winch->SpinClockwise();
		m_winch->SpeedAdjust(1.0);
	}
	if(LS_B3)
	{
		m_winch->SpinCounterClockwise();
		m_winch->SpeedAdjust(1.0);
	}
	
	//m_shooter->CheckEReset();

	
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

	SmartDashboard::PutNumber("CodeVersion", 7);

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

	m_distanceSensor->Reset();
	m_flag->Reset();
	m_robotDrive->Reset();
	m_shooter->Reset();
	m_roller->Reset();
	m_winch->Reset();
}

bool BuiltinDefaultCode::UpdateSubsystems()
{
	//m_robotDrive->GetLeftEncoder();
	//m_robotDrive->GetRightEncoder();

	bool finished = true;
	finished = m_distanceSensor->Update()	&& finished;
	finished = m_shooter->Update()    && finished;
	finished = m_flag->Update()   && finished;
	finished = m_gripLeft->Update()   && finished;
	finished = m_gripRight->Update()   && finished;
			
	finished = m_robotDrive->Update() && finished;
	finished = m_roller->Update() && finished;
	finished = m_winch->Update() && finished;
	return finished;

}

START_ROBOT_CLASS(BuiltinDefaultCode);
