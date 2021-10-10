#include"Passwordlock.h"

SoftwareSerial BT(10,11); //10-TX 11-RX

bool GetKey_Value();   //获取键盘值
void Failure_function();  //失效函数
void Strand_door(); //串口
char Serial_port();//串口接收到信息 并返回
void PasswordComparison_late(); //密码比较后期

const byte ROWS = 4; //行
const byte COLS = 3; //列

//按键接口定义
byte rowePins[ROWS] = {2, 3, 4, 5}; //行
byte colPins[COLS] = {6, 7, 8}; //列
//辅助脚位定义
byte reach = 9;       //达成启动-脚位
byte led_errors = 13; //警告-错误-脚位

short reac_time = 3000; //达成启动-时间

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


//设置LCD1602设备地址,具体看模块手册
LiquidCrystal_I2C lcd(0x27, 16, 2);
//设置矩形键盘
Keypad keypad = Keypad(makeKeymap(Keys), rowePins, colPins, ROWS, COLS);


void setup()  //在main ()中调用
{
digitalWrite(reach, HIGH);
 pinMode(led_errors, OUTPUT);
  pinMode(led_errors, OUTPUT);

  pinMode(reach, OUTPUT); //达成
  Serial.begin(9600);
  BT.begin(38400);
  lcd.init();                  // 初始化LCD
  lcd.backlight();             //设置LCD背景等亮
  if(IF_MAX())
    Serial.println("[state]:Password length:sure\n");
    else
    while(1)
    {
     Serial.println("[warning]:Password length:No way\n Exit modification->Password length\n");
      delay(8000);
    }
 
while(1)
{
 //初始密码
  if((Initial_Password(User_Password)))
  {
     Serial.println("[state]:Password_init:yes\n");
    break;
  }
  
  Serial.println("[warning]:Password_init:No");
  Serial.println("Repeat execution......\n\n");
  delay(8000);
}

}

int ergodic = 10;  //遍历
char auxiliary; //辅助键使用(决定程序走向)

void loop()   //在main ()中for( ; ; ;)调用
{
  //analogRead(Bluetooth);
init_errors:   //goto
  /****************************/
  //只让执行一遍,否则重复打印
  if (ergodic == 10)
  {
  
    lcd.clear();  // 清屏
    lcd.setCursor(0, 0);               //设置显示指针
    lcd.print("QWQ-Welcome-QAQ");
    lcd.setCursor(0, 1);
    lcd.print("'#'Key InPassword");


    //初始化-临时密码
    if ((change_init_password(Temporary_password)))
    {
      Serial.println("[state]:init_password:ok");
    }
    else
    { //初始化失败
      Serial.println("[error]:init_password:on");
      lcd.clear();  // 清屏
      lcd.setCursor(0, 0);
      lcd.print("Press the '*' key");
      lcd.setCursor(0, 1);
      lcd.print("[error]:init...");
      Failure_function();
      ergodic = 10;
      goto init_errors;
    }

  } //只让执行一遍,否则重复打印
  /****************************/
  ergodic = 0;


  //串口开门
  if(BT.available())
  {
     BT.println("\n'");
    BT.println("Enter the password in '3S'");
    BT.println("_________________________________\n");
    BT.println("|Please enter a selection:      |");
    BT.println("|>'1'-Open the door             |");
    BT.println("|>'2'-Change password           |");
     BT.println("________________________________\n");
    BT.println("\n'");
  while (BT.available())
  {
    Strand_door();
    ergodic = 10;
    delay(1000);
  }

  }


  auxiliary = keypad.getKey();
  if ((auxiliary != NO_KEY) && auxiliary == '#')
  { //开始

    Serial.println("Start InPassword");
    delay(20);

Cancel_pd:

    lcd.clear();  // 清屏
    lcd.setCursor(0, 0);
    lcd.print("#Key in password");   //输入密码
    lcd.setCursor(0, 1);
    lcd.print("*Key change password");  //改密码
    Serial.println("[state]:Input password");

    while (1)
    {
      auxiliary = keypad.getKey();

      if ((auxiliary != NO_KEY) && auxiliary == '*' || auxiliary == '#')
      {
        lcd.clear();  // 清屏
        lcd.setCursor(0, 0);
        /****************'*'*************************/
        if (auxiliary == '*')//改密码
        {

          lcd.print("In Original pd:");   //输入密码

          if (GetKey_Value())
            if ((Password_Comparison(Temporary_password, User_Password)))
            { //相等
              Serial.println("[state]:change password..");
              lcd.clear();  // 清屏
              lcd.setCursor(0, 0);
              lcd.print("New password：");  //新密码
              if (GetKey_Value())
              { //获取密钥值
                lcd.clear();  // 清屏
                lcd.setCursor(0, 0);
                lcd.print("Original password：YES");  //改密码
                delay(1900);
                lcd.clear();  // 清屏
                lcd.setCursor(0, 0);
                lcd.print("* Key Determine");  //确定改密码
                lcd.setCursor(0, 1);
                lcd.print("# Key Cancel");  //改密码

                Serial.println("Original password：YES");

                while (1)
                {
                  auxiliary = keypad.getKey();

                  if ((auxiliary != NO_KEY) && auxiliary == '*' || auxiliary == '#')
                  {

                    if (auxiliary == '*')
                    {
                      //密码更改   临时密码,用户密码
                      if (Password_Change(Temporary_password, User_Password))
                      { //密码更改 成功
                        lcd.clear();  // 清屏
                        lcd.setCursor(0, 0);
                        lcd.print("Pd Ce success!");
                        delay(1000);
                        Serial.println("|change password:success!|");
                        goto Cancel_pdok;
                      }//密码更改 失败
                      else
                      {
                        Serial.println("[error]:Acquisition failed");
                        lcd.clear();  // 清屏
                        lcd.setCursor(0, 0);
                        lcd.print("Press the '*' key");
                        lcd.setCursor(0, 1);
                        lcd.print("Acquisition failed");
                         Serial.print("[error]:Acquisition failed");  //更改失败
                        Failure_function();

                      }//else
                    }//if*
                    else
                      goto Cancel_pd;

                  }//if if(* #)
                }//while...while


              }//IF获取密钥值
              else
              { //获取密钥值失败
                Serial.println("[error]:Acquisition failed");
                lcd.clear();  // 清屏
                lcd.setCursor(0, 0);
                lcd.print("Press the '*' key");
                lcd.setCursor(0, 1);
                lcd.print("[error]:Acquisition failed");
                continue;
              }//获取密钥值失败

            }//if相等
            else
            {   
              //不相等
              lcd.clear();  // 清屏
              lcd.setCursor(0, 0);
              lcd.print("OrPassword  error");   //输入密码
              lcd.setCursor(0, 1);
               lcd.print("Press # to continue");   //输入密码
              Serial.println("OriginalPassword error");
              digitalWrite(led_errors, HIGH);
              delay(3000);
              digitalWrite(led_errors, LOW);
                 auxiliary = keypad.getKey();
              if(auxiliary == '#')
              lcd.clear();  // 清屏
              lcd.setCursor(0, 0);
              continue;
            }

        }//if *
        else if (auxiliary == '#')
        {
          /****************'##########'*********/
          lcd.print("In password:");
          Serial.println("[state]:In_password");
          if (GetKey_Value())
            break;
          else
          {
            lcd.clear();  // 清屏
            lcd.setCursor(0, 0);
            lcd.print("#Key in password");   //输入密码
            lcd.setCursor(0, 1);
            lcd.print("*Key change password");  //改密码
            Serial.println("[state]:Input password");
            continue;
          }
        }
      }

      delay(50);

 if(BT.available())
  {
     BT.println("\n'");
    BT.println("Enter the password in '3S'");
    BT.println("_________________________________\n");
    BT.println("|Please enter a selection:      |");
    BT.println("|>'1'-Open the door             |");
    BT.println("|>'2'-Change password           |");
     BT.println("________________________________\n");
    BT.println("\n'");
  while (BT.available())
  {
    Strand_door();
    ergodic = 10;
    delay(1000);
  }
  }//if()

    }//while

Cancel_pdok:
PasswordComparison_late();
 }//if 进入条件
   
}//loop();
/********************************/

void PasswordComparison_late() //密码比较后期
{
   //密码比较 临时,用户
    if ((Password_Comparison(Temporary_password, User_Password)))
    { //相等
      lcd.clear();  // 清屏
      lcd.setCursor(0, 0);
      lcd.print("Unlocked");
      Serial.println("[state]:Password equality");

      digitalWrite(reach, LOW);
      delay(reac_time);
      digitalWrite(reach, HIGH);
    }
    else
    {
      Serial.println("Passwords are not equal");
      lcd.clear();  // 清屏
      lcd.setCursor(0, 0);
      lcd.print("[warning]:");
      lcd.setCursor(0, 1);
      lcd.print("NOPassword");

      digitalWrite(led_errors, HIGH);
      delay(3000);
      digitalWrite(led_errors, LOW);
      //密码比较 临时,用户
    }//else

    ergodic = 10; //
 

}

bool GetKey_Value()
{
  while (1)
  {
    //获得按键值
    for (ergodic = 0; ergodic < Password_Max;)
    {
      Temporary_password[ergodic] = keypad.getKey();
      //检查获得
      if ((Temporary_password[ergodic] != NO_KEY) && Temporary_password[ergodic] != '#')
      {
        if (Temporary_password[ergodic] == '*')
        {
          ergodic = 10;
          delay(20);
          return true;
        }
        lcd.setCursor(ergodic, 1);
        lcd.print("*");
        ergodic++;
      }
      else if (Temporary_password[ergodic] == '#')
      {
        Temporary_password[ergodic--] = NO_KEY;
        Temporary_password[ergodic] = NO_KEY;
        lcd.setCursor(ergodic, 1);
        lcd.print(' ');
      }

    }//FOR

    if (ergodic == Password_Max)
      return true;
    else
    { //获取失败
      Serial.println("[error]:Acquisition failed");
      lcd.clear();  // 清屏
      lcd.setCursor(0, 0);
      lcd.print("Press the '*' key");
      lcd.setCursor(0, 1);
      lcd.print("[error]:Acquisition failed");
      Failure_function();
      //获取失败
      return false;


    }//else
  }//while
}//GetKey_Value


//失败 函数
void Failure_function()
{
  while (1)
  {
    char nopd;
    digitalWrite(led_errors, HIGH);
    delay(300);
    digitalWrite(led_errors, LOW);
    nopd = keypad.getKey();

    if (nopd != NO_KEY && nopd == Keys[3][0])
    {
      lcd.clear();  // 清屏
      delay(10);
      break;
    }
  }//while
}


//串口开门
void Strand_door()
{
    lcd.clear();  // 清屏
    lcd.setCursor(0, 0);
    lcd.print("Serial input.....");
  Temporary_password[0] = Serial_port(); //获取 1-开门 2-改密码
  if (Temporary_password[0] == '1') //1-开门
  {
    while (BT.available())
    { // 当串口还有信息后
      BT.read();
    }//主要清空缓冲区

    //请输入密码
    BT.println("Please input a password:");
    delay(3500);  //3.5s等待用户输入
    //获取
    for (ergodic = 0; ergodic < Password_Max; ergodic++)
      Temporary_password[ergodic] = Serial_port();


    delay(5);
    while (BT.available())
    { // 当串口还有信息后
       BT.read();
    }//主要清空缓冲区

    //比较
    if((Password_Comparison(Temporary_password,User_Password)))
    {
      BT.println("The password is correct!");

      //相等
    lcd.clear();  // 清屏
    lcd.setCursor(0, 0);
    lcd.print("Unlocked");
    BT.println("[state]:Password equality");

    digitalWrite(reach, LOW);
    delay(reac_time);
    digitalWrite(reach, HIGH);
      
    }
    else
    {
      BT.println("Password error!");

       BT.println("Passwords are not equal");
    lcd.clear();  // 清屏
    lcd.setCursor(0, 0);
    lcd.print("[warning]:");
    lcd.setCursor(0, 1);
    lcd.print("NOPassword");

    digitalWrite(led_errors, HIGH);
    delay(3000);
    digitalWrite(led_errors, LOW);
    }
    /****************************************/
  }

  else if (Temporary_password[0] == '2') /******2-改密码****/
  {
    /****************************************/
    while (BT.available())
    { // 当串口还有后
       BT.read();
    }//主要清空缓冲区

    BT.println("Enter the original password:");
    delay(3500);//3.5s等待用户输入
    //获取
    for (ergodic = 0; ergodic < Password_Max; ergodic++)
      Temporary_password[ergodic] = Serial_port();




    delay(5);
    while (BT.available())
    {
      BT.read();
    }//主要清空缓冲区

    //比较
    if((Password_Comparison(Temporary_password,User_Password)))
    {

      BT.println("The password is correct!");
      BT.println("Enter new password:");

      delay(3500);//3.5s等待用户输入
      //获取
      for (ergodic = 0; ergodic < Password_Max; ergodic++)
        Temporary_password[ergodic] = Serial_port();
   
      //替换
  
      if((Password_Change(Temporary_password,User_Password)))
      {
        BT.println("Saved,New password:");
        //打印提示
        for (ergodic = 0; ergodic < Password_Max; ergodic++)
          BT.print(User_Password[ergodic]);
        BT.print("\n");
      }
      else
      {
        //保存失败
        BT.println("Saved failed");
        BT.println("Password is:");
        delay(5);

        //保存失败-给与1-n(n<=Password_Max)
        for (ergodic = 0; ergodic < Password_Max; ergodic++)
          User_Password[ergodic] = (char)(ergodic+1);
        //打印提示
        for (ergodic = 0; ergodic < Password_Max; ergodic++)
          BT.print(User_Password[ergodic]);
        BT.print("\n");

        
      }
      delay(5);
      while (BT.available())
      { //主要清空缓冲区
       BT.read();
      }
      //可以优化处: 防止远程改了密码
      return false;
    }
    else
    {
      BT.println("Password error!");
      return false;
    }
  }

  while (BT.available())
  { //主要清空缓冲区
    BT.read();
  }
  //对Arduino的缓存区有心理阴影

  lcd.clear();  // 清屏
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
