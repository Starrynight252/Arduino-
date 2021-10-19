#include"Passwordlock.h"

#define ROWS 4 //行
#define COLS 3 //列
//辅助脚位定义
#define reach 9       //达成启动-脚位
#define led_errors 13 //警告-错误-脚位
#define buzzer_errors 12  //蜂鸣器
#define BT_testing 0   //蓝牙检测 A0
#define reac_time 3000 //达成启动-时间

bool Hardware_password(); //硬件密码处理
bool Hardware_pd_Acquisition();//硬件密码获取 
char Serial_port();//串口接收到信息 并返回
void Strand_door();  //串口开门
void Error_lnterrupt(byte );//错误中断

//按键接口定义
byte rowePins[ROWS] = {2, 3, 4, 5}; //行
byte colPins[COLS] = {6, 7, 8}; //列
//定义矩形按键,键位值
char Keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
//用户密码(+串口)-用于保存
char User_Password[Password_Max];
char Temporary_password[Password_Max];  //临时串口,硬件密码 用于比较
byte ergodic=10; //遍历  10-每次只让loop当中if执行一遍,否则重复打印
short Number_Errors=0;   //错误次数
//定义rx,tx蓝牙接口
SoftwareSerial BT(10,11); //10-TX 11-RX
//设置LCD1602设备地址,具体看模块手册
LiquidCrystal_I2C lcd(0x27, 16, 2);
//设置矩形键盘
Keypad keypad = Keypad(makeKeymap(Keys), rowePins, colPins, ROWS, COLS);

void setup() //在main ()中调用
{
  pinMode(buzzer_errors, OUTPUT);
  pinMode(led_errors, OUTPUT);
  pinMode(reach, OUTPUT); //达成   

   digitalWrite(reach, HIGH);
  Serial.begin(9600);
  BT.begin(38400);
  lcd.init();                  // 初始化LCD
  lcd.backlight();             //设置LCD背景等亮

  //密码长度
  if(!IF_MAX())
    while(1)
      Error_lnterrupt(0);
      
     
     if(!EEPROM_ReadIn(User_Password))
     {
       if(!Initial_Password(User_Password))    //初始密码
        while(1)
          Error_lnterrupt(1);
     }
     
}

void loop()//在main ()中for( ; ; ;)调用
{
  char auxiliary; //辅助键使用(决定程序走向)
  //每次只让执行一遍,否则重复打印
  if (Serial.available())
  {
    while (Serial.available())
    { // 当串口还有信息后
       Serial.read();
    }//清空缓冲区+密码错误
    Serial.println();
     for(ergodic=0;ergodic<Password_Max;ergodic++)
       Serial.print(User_Password[ergodic]);
  }


    lcd.clear();  // 清屏
    lcd.setCursor(4, 0);               //设置显示指针
    lcd.print("Welcome");
    lcd.setCursor(1, 1);
    lcd.print("# Key start");
    
      Strand_door();//串口开门
    
   auxiliary = keypad.getKey();
    delay(5);
   if ((auxiliary != NO_KEY) && auxiliary == '#')
    {
      Unequal:
      auxiliary =NO_KEY;
      //开始
      lcd.clear();  // 清屏
      lcd.setCursor(0, 0);
      lcd.print("# key in pd");   //输入密码
      lcd.setCursor(0, 1);
      lcd.print("* key ce pd");  //改密码

      while(1)
      {
        auxiliary = keypad.getKey();
        delay(5);
        if ((auxiliary != NO_KEY)&&(auxiliary == '#'||auxiliary == '*'))
        {
          lcd.clear();  // 清屏
          lcd.setCursor(0, 0);
          lcd.print("pd:");
         // delay(3000);
         if(auxiliary == '#')
         {
           //
           if(Hardware_password())
            {
              lcd.clear();  // 清屏
              lcd.setCursor(0, 0);
              
              Number_Errors=0;
              break;  //密码相等
            }
          else
            goto Unequal;//密码不相等
         }
          else if(auxiliary == '*')
          {
             if(Hardware_password())
             {
               //密码相等
                lcd.clear();  // 清屏
                lcd.setCursor(0, 0);
                lcd.print("IN New pd:");
                Hardware_pd_Acquisition();  //硬件密码获取
                lcd.clear();  // 清屏
                lcd.setCursor(0, 0);

                if(Temporary_password[0]!='Q')
                {
                  //
                  for (ergodic = 0;(ergodic < Password_Max);)
                  {
                    User_Password[ergodic]=Temporary_password[ergodic];
                    ++ergodic;
                  }
                  //保存密码
                  if(!EEPROM_WriteIn(User_Password))
                  {
                    // 密码保存失败进行-初始密码
                    if(!Initial_Password( User_Password))  
                      while(1)
                        Error_lnterrupt(1);
                  }
                  lcd.print("saved");  
                }
                else
                 lcd.print("NO!"); 

              
                delay(2000);
                break;  //密码相等       
             }
            else
              goto Unequal;//密码不相等
          }
        }
      }//
  }
  delay(150);
}



bool Hardware_password() //硬件密码处理
{
  //硬件密码获取
  while(1)
  {
    if(Hardware_pd_Acquisition()!=false)
    {
       //密码比较 临时,用户
      if (Password_Comparison(Temporary_password, User_Password))
      { //相等
        lcd.clear();  // 清屏
        lcd.setCursor(0, 0);
        lcd.print("Yes");

        digitalWrite(reach, LOW);
        delay(reac_time);
        digitalWrite(reach, HIGH);
        return true;
      }
      else
      {
        ++Number_Errors;
        lcd.clear();  // 清屏
        lcd.setCursor(0, 0);
        lcd.print("NO,Password");

       if(Number_Errors)
        {
          digitalWrite(led_errors, HIGH);
          
            delay(2500);
          if(Number_Errors>=3) 
          {
            lcd.setCursor(0, 1);
            lcd.print("Half a minute");
            delay(30000);
          }
          digitalWrite(led_errors, LOW);
        } 
        
        
        return false;
      }//else
    }
    else
      return false;
  }
}

//硬件密码获取 
bool Hardware_pd_Acquisition()
{
  //获得按键值
  for (ergodic = 0; ergodic < Password_Max;)
  {
    Temporary_password[ergodic] = keypad.getKey();
     delay(5);
    if ((Temporary_password[ergodic]!= NO_KEY)&&Temporary_password[ergodic]!='#')
    {
      if(Temporary_password[ergodic] == '*')
      {
        Temporary_password[0]='Q';
        return false;
      }

      lcd.setCursor(ergodic, 1);
      lcd.print("*"); 
       ++ergodic;
     }  
    else if(Temporary_password[ergodic] == '#')//删除
    {
      if(ergodic)
      {
        Temporary_password[ergodic--]==NO_KEY;
        Temporary_password[ergodic]==NO_KEY;
        lcd.setCursor(ergodic, 1);
        lcd.print(' ');
      }
    }
  }  
    return true;
}


//串口接收到信息 并返回
char Serial_port()
{
  char serialData;
  //等待缓冲区拿到值
  delay(10);
  if (BT.available())
  { // 当串口接收到信息后
    serialData = BT.read();
  }
  return serialData;
}
//串口开门


void Strand_door()
{
    char option='.';
  while((analogRead(BT_testing))>300)
  {
    lcd.clear();  // 清屏
    lcd.setCursor(0, 0);
    lcd.print("Serial input");
   delay(5); 
   option=Serial_port(); //获取 1-开门 2-改密码
   if (option== '1'||option=='2') //1-开门
   {
     while (BT.available())
      { // 当串口还有信息后
        BT.read();
      }//主要清空缓冲区
        //请输入密码
      BT.println("In password:");
      //获取
      delay(4500);  //4.5s等待用户输入
      for (ergodic = 0; ergodic < Password_Max; ergodic++)
        Temporary_password[ergodic] = Serial_port();
      delay(5);
      while(BT.available())
      { // 当串口还有信息后
        BT.read();
        Temporary_password[Password_Max]='Q';
      }//清空缓冲区+密码错误

      if(!BtSrialPort_data_processing(Temporary_password))
      break;
      //比较

      if(Password_Comparison(Temporary_password,User_Password))
      {
        //相等
        BT.println("Yes");
        //相等
        lcd.clear();  // 清屏
        lcd.setCursor(0, 0);
        lcd.print("Yes");

        if(option=='1'&&option!='2')
        {
          //开门
          digitalWrite(reach, LOW);
          delay(reac_time);
          digitalWrite(reach, HIGH);
          return ;  //完成返回
        }
        else{
          //改密码
          BT.println("In new pd:");
           //获取
          delay(4500);  //4.5s等待用户输入
          for (ergodic = 0; ergodic < Password_Max; ergodic++)
            Temporary_password[ergodic] = Serial_port();
          delay(5);

          while (BT.available())
          { // 当串口还有信息后
            BT.read();
            Temporary_password[Password_Max]='Q';
          }//清空缓冲区+密码错误
          if((BtSrialPort_data_processing(Temporary_password))==false)
          break;

          for (ergodic = 0;ergodic < Password_Max;)
            {
              User_Password[ergodic]=Temporary_password[ergodic];
              ++ergodic;
            }
            //保存密码
            if(!EEPROM_WriteIn(User_Password))
           {
             // 密码保存失败进行-初始密码
             if(!Initial_Password( User_Password))  
               while(1)
                Error_lnterrupt(1);
           }  
          BT.println("New pd:");
          //打印密码
          for (ergodic = 0; ergodic < Password_Max; ergodic++)
            BT.print(User_Password[ergodic]);
            BT.println();
            delay(5); 
            return ;  //完成返回
        }
     
      }
      else
        break;//密码不相等
   }
   else
    continue;
  }
  if(option!='.'||Temporary_password[Password_Max]=='Q')
  {
    BT.println("Pd error");
    while (BT.available())
    { // 当串口还有信息后
      BT.read();
    }//清空缓冲区+密码错误
    return ;//完成返回
  }
  delay(10);  
}//Strand_door()


void Error_lnterrupt(byte errs)//错误中断
{
  delay(8000);
  lcd.clear();  // 清屏
  lcd.setCursor(0, 0);
  lcd.print("error code:");
  lcd.println(errs);
  switch (errs)
  {
    case 0:       //密码过长
    case 1:         //密码初始化失败  
    default:
      while (1)
      {
        digitalWrite(led_errors, HIGH);
        delay(300);
        digitalWrite(led_errors, LOW);
      }
      break;
  }

}
