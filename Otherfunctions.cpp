#include"Passwordlock.h"

//对比
int IF_MAX()
{
  return (Password_Max <= Passwordlock);
}

//改变初始化_临时密码
bool change_init_password(char *init_password)
{
  int temporary;
  char increas = 'A';
  for (temporary = 0; temporary < Password_Max; temporary++)
  {
    *(init_password + temporary) = increas++; //非语法糖
  }

  if (temporary >= Password_Max)
    return true;
  else
    return false;
}



//初始密码
bool Initial_Password(char *password)
{
  int initsd;
  char inits='0';
  for (initsd = 0; initsd < Password_Max;initsd++)
    password[initsd] =inits++;
    Serial.print("init_password:"); 
 for (initsd = 0; initsd < Password_Max; initsd++)
    Serial.print(password[initsd]); 
    Serial.println("\n"); 
  if (initsd >= Password_Max)
    return true;
  else
    return false;
}



//密码比较 临时密码,用户密码   strcmp();
bool Password_Comparison(const char  *Temporary_pd, const char  *User_Pd)
{
  int temporary, db = 1;
  for (temporary = 0; temporary < Password_Max; temporary++)
  {
    db *= ((User_Pd[temporary]) == (Temporary_pd[temporary]));
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

  int temporary ;
  for (temporary = 0; temporary < Password_Max; temporary++)
  {
    use_pdn[temporary] = Temporary_pd[temporary];
  }

  if (Password_Comparison(Temporary_pd, use_pdn))
    return true;
  else
  {
    for (temporary = 0; temporary < Password_Max; temporary++)
    {
      use_pdn[temporary] = temporary;
    }
    return false;
  }
}
