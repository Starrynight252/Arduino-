#ifndef Passwordlock//防止重复包含
#define Passwordlock 15  //限制密码的长度

#include<Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> //引用I2C库
#include <SoftwareSerial.h> 
#include <EEPROM.h>

#define Password_Max 5 //密码长度

//对比 #define IF_MAX(...)(__VA_ARGS__<=Passwordlock);
int IF_MAX();

//初始密码
bool Initial_Password(char *);
//密码比较 临时密码,用户密码   strcmp();
bool Password_Comparison(const char *,const char *);
//密码更改   临时密码,用户密码  
bool Password_Change(char *,char *);

void Failure_function();  //失效函数
void Strand_door(); //串口
char Serial_port();//串口接收到信息 并返回

#endif
