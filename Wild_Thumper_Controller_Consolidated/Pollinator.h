//=== GLOBALS ===//
byte InPhase;                         // 1 when the brush PWM keeps them spinning in sync with the wheels, controls how long to control for?
int calculateRateHZ                   // This is the number of times this function will be called per second.

// PID control for the brush speed.
int getBrushPWM(int driveSpeed, /*int brushSpeed,*/ int lastBrushSpeed, int lastBrushPWM)
{
  int desiredSpeed;
  int currentSpeed;
  int lastSpeed;
  int speedError;

  currentSpeed = brushSpeed;
  desiredSpeed = driveSpeed * WheelRad / BrushRad;
  //lastSpeed = lastBrushSpeed;
  speedError = currentSpeed - desiredSpeed;                        // positive if we are overshooting. negative if undershooting.
  /*  speedSlope = (currentSpeed - lastSpeed) * calculateRateHz;
    newPWM = lastBrushPWM - brushKP * speedError - brushKD * speedSlope - brushKI; */

  newPWM = lastBrushPWM - brushKP * speedError;

  return newPWM;
}

//=== reading the encoders using a transitions and using time ===//
void readRotaryEncoders() //using a timer
{
  int i = 0;
  int timingInterval = 2
                       int LCount = 0;
  int RCount = 0;

  int RTimer = 0;
  int LTimer = 0;

  //want to wait until the right rotary encoder is triggered
  if (digitalRead(RREnc) == HIGH)
  {
    RTimer = millis();
    for (i = 0; i < timingInterval) {
      while (digitalRead(RREnc) == HIGH);
      while (digitalRead(RREnc) == LOW);
      i++;
    }
    RTimer = millis() - RTimer
  }
  else
  {
    RTimer = millis();
    for (i = 0; i < timingInterval) {
      while (digitalRead(RREnc) == LOW);
      while (digitalRead(RREnc) == HIGH);
      i++;
    }
    RTimer = millis() - RTimer
  }
  //for the left side
  if (digitalRead(RREnc) == HIGH)
  {
    LTimer = millis();
    for (i = 0; i < timingInterval) {
      while (digitalRead(RREnc) == HIGH);
      while (digitalRead(RREnc) == LOW);
      i++;
    }
    LTimer = millis() - LTimer
  }
  else
  {
    LTimer = millis();
    for (i = 0; i < timingInterval) {
      while (digitalRead(RREnc) == LOW);
      while (digitalRead(RREnc) == HIGH);
      i++;
    }
    LTimer = millis() - LTimer
  }
}
}


/*
 * This Function will read the the left or right rotary encoders over a predefined time interval
 * INPUTS:
 * int LeftOrRight = 0 reads right rotary encoder
 * int LeftOrRight = 1 reads left rotary encoder
 *
 * OUTPUTS:
 * double RPM's
*/

double getRPM(int LeftOrRight)// to read the right encoder set input for function = 0 to read left set input to function = 1
{
  int Atime = 0;
  int Btime = 0;
  int Last_Atime = -1;
  int AFlag = 0;
  int BFlag = 0;
  int ACurrentState = LOW;
  int BCurrentState = LOW;
  int ACount = 0;
  int d = 1;                     // assume we are going forwards

  int AtoA = 0;
  int Total_AtoA = 0;
  double Average_AtoA = 0;
  double RPM

  int EncA;
  int EncB;

  int timer;

  if (LeftOrRight == 0)                                                       //looking at right rotary encoder
  {
    EncA = RREncA;
    EncB = RREncB;
  }
  else                                                                        //default to left
  {
    EncA = LREncA;
    EncB = LREncB;
  }

  timer = millis();
  while (millis() - timer < 500)                                              // for 500ms
  {
    if (digitalRead(EncA) == HIGH && AFlag == 0 && ACurrentState == LOW)        //when the A signal goes from low to high
    {
      if (ACount != 0)        // first time round we don't set Last_Atime
      {
        Last_Atime = Atime;
      }
      Atime = millis();
      AFlag = 1;
      ACurrentState = HIGH;
      ACount ++;
    }
    else if (digitalRead(EncA) == LOW && AFlag == 0 && ACurrentState == HIGH)    //when A signal goes high to low
    {
      ACurrentState = LOW;
    }
    if (digitalRead(EncB) == HIGH && BFlag == 0 && BCurrentState == LOW)        //when the B signal goes from low to high
    {
      Btime = millis();
      BFlag = 1;
      BCurrentState = HIGH;
    }
    else if (digitalRead(EncB) == LOW && BFlag == 0 && BCurrentState == HIGH)    //when the B signal goes from high to low
    {
      BCurrentState = LOW;
    }

    if (AFlag == 1 && BFlag == 1)                                               //if both flags are set we want the difference in there time
    {
      AFlag = 0;             // Reset flags
      BFlag = 0;
      if (Btime < Atime)     // B triggered before A, we must be going backwards.
      {
        d = -1;
      }
      else
      {
        d = 1;
      }

      if (Acount > 1)        // We have more than one Atime to compare.
      {
        AtoA = Atime - Last_Atime;
        Total_AtoA += AtoA;
      }
    }
    // If the thing is moving we will have had at least 1 tick, otherwise we assume we are standing still.
    if (Acount > 1)
    {
      Average_AtoA = (double)Total_AtoA / (ACount - 1);
    }
    else
    {
      Average_AtoA = INF;
    }
  }
  RPM = d * MILLIS_IN_SEC * SECS_IN_MIN / (TICKS_PER_ENC * Average_AtoA);

  return RPM;
}
