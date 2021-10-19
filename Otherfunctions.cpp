#include"Passwordlock.h"

//对比
int IF_MAX()
{
  return (Password_Max <= Passwordlock);
}


//初始密码
bool Initial_Password(char *password)
{
  byte ergodic;
  char inits='0';
  for (ergodic = 0; ergodic < Password_Max;ergodic++)
    password[ergodic] =inits++;
 
  if (ergodic >= Password_Max)
    return true;
  else
    return false;
}



//密码比较 临时密码,用户密码   strcmp();
bool Password_Comparison(const char  *Temporary_pd, const char  *User_Pd)
{
 byte temporary, db;
  for (temporary = 0,db = 1; temporary < Password_Max; )
  {
    db *= (User_Pd[temporary] == Temporary_pd[temporary]);
    ++temporary;
  }
  if (db)
    return true;
  else
    return false;
}

//密码更改   临时密码,用户密码
bool Password_Change (char *Temporary_pd , char *use_pdn)
{

  if (Password_Comparison(Temporary_pd, use_pdn))
    return true;

   byte temporary ;
  for (temporary = 0; temporary < Password_Max; )
  {
    use_pdn[temporary] = Temporary_pd[temporary];
     ++temporary;
  }

  if (Password_Comparison(Temporary_pd, use_pdn))
    return true;
  else
  {
    for (temporary = 0; temporary < Password_Max; )
    {
      use_pdn[temporary] = temporary;
       ++temporary;
    }
    return false;
  }
}
//串口数据处理
bool BtSrialPort_data_processing(char* Temporary)
{
  byte ergodic,number,option;
  for (option=ergodic = 0; ergodic < Password_Max; ergodic++)
    for(number=48;number<=57;number++)   
      option+=(number==(int)Temporary[ergodic]);  
      
  if(option>=Password_Max)
   return true;
  else
    return false;
}  


bool EEPROM_WriteIn(char* InPassword)  //写入EEPROM
{
  byte ergodic;
  for(ergodic=0;ergodic<Password_Max;ergodic++)
  {
    EEPROM.write(ergodic,(int)InPassword[ergodic]); 
    delay(10);
  }
  return BtSrialPort_data_processing(InPassword);
}

bool EEPROM_ReadIn(char* InPassword)  //读入EEPROM
{
  byte ergodic;
  for(ergodic=0;ergodic<Password_Max;ergodic++)
  {
    InPassword[ergodic]=(char)EEPROM.read(ergodic);
     delay(10);
  }
  for(ergodic=0;ergodic<Password_Max;ergodic++)
   Serial.print(InPassword[ergodic]);
  Serial.println();
  return BtSrialPort_data_processing(InPassword);
}
