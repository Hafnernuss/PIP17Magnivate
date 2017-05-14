#include <BlinkTask.h>
#include <Debouncer.h>
#include <DelayRun.h>
#include <Dimmer.h>
#include <FrequencyTask.h>
#include <Heartbeat.h>
#include <Rotary.h>
#include <Task.h>
#include <TonePlayer.h>
#include <PciManager.h>
#include <SoftPwmTask.h>
#include <SoftTimer.h>

#include <LiquidCrystal.h>

#define LCD_RS 12
#define LCD_EN 11
#define LCD_D4 10
#define LCD_D5 9
#define LCD_D6 8
#define LCD_D7 7

#define LCD_NUM_ROWS 4
#define LCD_NUM_COLUMNS 20

#define MOTOR_BASE_PIN 3

#define MOTOR_SPEED_MIN 14
#define MOTOR_SPEED_MAX 255
#define MOTORSPEEDSTR "Motor Speed: %d%%"
#define PERCENT_TO_SPEED(x) (((x * (MOTOR_SPEED_MAX - MOTOR_SPEED_MIN)) / 100)) + MOTOR_SPEED_MIN
#define ADC_TO_PERCENT(x) (long)((((long)x) * (long)100) / (long)1020)

#define POT_PIN 0

void ReadPotentiometer(Task * This);

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
Task ReadPotentiometerTask(100, ReadPotentiometer);

bool bFirstRun = true;

void setup() 
{
  lcd.begin(LCD_NUM_COLUMNS, LCD_NUM_ROWS);

  pinMode(MOTOR_BASE_PIN, OUTPUT);

  SetMotorSpeed(0);

  SoftTimer.add(&ReadPotentiometerTask);

}

void SetMotorSpeed(int nNewPercentage)
{
  int nPercentage = constrain(nNewPercentage, 0, 100);
  char cScreenBuf[255];
  
  lcd.clear();
  sprintf(cScreenBuf, MOTORSPEEDSTR, nPercentage);
  lcd.print(cScreenBuf);

  int nMotorSpeed = PERCENT_TO_SPEED(nPercentage);
  if(nPercentage == 0)
    nMotorSpeed = 0;
    
  analogWrite(MOTOR_BASE_PIN, nMotorSpeed);
}

void ReadPotentiometer(Task * This)
{
  int val = analogRead(POT_PIN); 
  int nPercentage = ADC_TO_PERCENT(val);
  if(bFirstRun && nPercentage != 0)
  {
    lcd.setCursor(0,1);
    lcd.print("FUSED.");
  }
    
  else
  {
    bFirstRun = false; 
    SetMotorSpeed(nPercentage);
  }
    
  
}

