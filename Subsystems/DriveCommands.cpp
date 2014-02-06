//
//  DriveCommands.cpp
//  First 2013
//
//  Created by Kyle Rokos on 2/7/13.
//  Copyright (c) 2013 Kyle Rokos. All rights reserved.
//

#include "DriveCommands.h"
#include "DriveTrain.h"
#include "math.h"

namespace Drive {

  DriveCommand::DriveCommand(DriveTrain *drive)
    :m_driveTrain(drive),
    m_firstUpdate(true)
  {

  }

  bool DriveCommand::Update()
  {
    if(m_firstUpdate)
    {
      m_firstUpdate=false;
      CommandInit();
      return CommandUpdate();
    }
    return CommandUpdate();
  }

  Distance::Distance(DriveTrain *drive, float meters, float seconds)
    :DriveCommand(drive),
    m_meters(meters),
    m_seconds(seconds),
    m_hasStarted(false), 
    m_pMotionDriveLeft(new Motion(0.06)),
    m_pMotionDriveRight(new Motion(0.06))
  {

  }

  Distance::Distance(DriveTrain *drive, float meters)
    :DriveCommand(drive),
    m_meters(meters),
    m_seconds(fabs(meters / drive->kMaxVelocityMetersPerSecond) + drive->kTimeRequiredToAccelerateToMaxVelocity * 2),
    m_hasStarted(false),
    m_pMotionDriveLeft(new Motion(0.06)),
    m_pMotionDriveRight(new Motion(0.06))
  {

  }

  Distance::~Distance()
  {
    delete m_pMotionDriveLeft;
    delete m_pMotionDriveRight;
  }

  bool Distance::CommandInit()
  {
    m_driveTrain->StartEncoders();
    double currentTime = Timer::GetPPCTimestamp();

    m_pMotionDriveLeft->Reset(m_driveTrain->GetLeftEncoder(), currentTime, m_meters * m_driveTrain->GetEncoderCountsPerMeter(), m_seconds);
    m_pMotionDriveRight->Reset(m_driveTrain->GetRightEncoder(), currentTime, m_meters * m_driveTrain->GetEncoderCountsPerMeter(), m_seconds);
    printf("In CommandInit(): LE: %f RE: %f Time: %f Clicks: %f duration: %f\n", (double)m_driveTrain->GetLeftEncoder(), (double)m_driveTrain->GetRightEncoder(), currentTime, m_meters * m_driveTrain->GetEncoderCountsPerMeter(), m_seconds);
    return true;
  }

  bool Distance::CommandUpdate()
  { 
    bool finished = false;

    double currentTime = Timer::GetPPCTimestamp();
    float leftPower = m_pMotionDriveLeft->AdjustVelocity(m_driveTrain->GetLeftEncoder(), currentTime);
    float rightPower = m_pMotionDriveRight->AdjustVelocity(m_driveTrain->GetRightEncoder(), currentTime);
    if((.01 < leftPower || leftPower < -.01 )&&
        (.01 < rightPower || rightPower < -.01))
    {
      m_hasStarted = true;
    }
    if(-0.01 < leftPower  && leftPower  < 0.01 &&
        -0.01 < rightPower && rightPower < 0.01 && m_hasStarted == true)
      //if(leftPower == 0 && rightPower == 0)
    {
      leftPower = 0;
      rightPower = 0;
      finished = true;
    }
    printf("In CommandUpdate::Distance setDistance = %f powerL=%f, powerR=%f\n", m_meters, leftPower, rightPower);
    printf("	Current time = %f LeftEncoder = %f RightEncoder = %f\n", currentTime, (double) m_driveTrain->GetLeftEncoder(), (double) m_driveTrain->GetRightEncoder());
    m_driveTrain->SetLeftRightMotorOutputs(leftPower, rightPower);

    return finished;
  }

  Rotate::Rotate(DriveTrain *drive, float degrees)
    :Drive::DriveCommand(drive),
    m_degrees(degrees),
    m_pMotion(new Motion(0.06))
  {
    while(m_degrees > 180)
      m_degrees = m_degrees - 360;
    while(m_degrees < -180)
      m_degrees = m_degrees + 360;
  }

  bool Rotate::CommandInit()
  {
    double currentTime = Timer::GetPPCTimestamp();
    m_driveTrain->StartEncoders();

    m_pMotionDriveLeft->Reset(m_driveTrain->GetLeftEncoder(), currentTime, m_degrees * m_driveTrain->GetEncoderCountsPerDegree(), m_seconds);
    m_pMotionDriveRight->Reset(m_driveTrain->GetRightEncoder(), currentTime, -m_degrees * m_driveTrain->GetEncoderCountsPerDegree(), m_seconds);

    m_driveTrain->ResetAngle();
    return true;
  }

  bool Rotate::CommandUpdate()
  {
    bool finished = false;

    double currentTime = Timer::GetPPCTimestamp();
    float leftPower = m_pMotionDriveLeft->AdjustVelocity(m_driveTrain->GetLeftEncoder(), currentTime);
    float rightPower = m_pMotionDriveRight->AdjustVelocity(m_driveTrain->GetRightEncoder(), currentTime);
    if((.01 < leftPower || leftPower < -.01 )&&
        (.01 < rightPower || rightPower < -.01))
    {
      m_hasStarted = true;
    }
    if(-0.01 < leftPower  && leftPower  < 0.01 &&
        -0.01 < rightPower && rightPower < 0.01 && m_hasStarted == true)
      //if(leftPower == 0 && rightPower == 0)
    {
      leftPower = 0;
      rightPower = 0;
      finished = true;
    }
    printf("In CommandUpdate::Rotate setDegrees = %f powerL=%f, powerR=%f\n", m_meters, leftPower, rightPower);
    printf("	Current time = %f LeftEncoder = %f RightEncoder = %f\n", currentTime, (double) m_driveTrain->GetLeftEncoder(), (double) m_driveTrain->GetRightEncoder());
    m_driveTrain->SetLeftRightMotorOutputs(leftPower, rightPower);

    return finished;
  }
}
