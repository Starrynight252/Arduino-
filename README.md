# Arduino-
Arduino密码电子锁 蓝牙HC05 LCD1602-IIC

材料(只是主要的):

材料
名称	个数
Arduino UNO	1
LCD1602A--IIC(要带IIC转接板)	1
矩形按键3*4 (也可以自制)	
1

继电器模块 (也可以自制 Q8550+IN4148)	1
任意V电磁锁	1
5mm或3mm led	2
杜邦线	n

接口定义:

名称	     
蓝牙
单片机接口      硬件接口
10------->TX
11<------RX
VCC---+5V
GND----GND

名称
LCD1602AIIC	
单片机接口      硬件接口
SDA---SDA
SCL---SCL
VCC---+5V
GND---GND

名称
矩形按键3*4	
单片机接口 
2, 3, 4, 5 //行
6, 7, 8    //列

名称
达成启动(继电器)
单片机接口9
硬件接口 IN
名称
错误LED错误(如密码输入错误)
单片机接口 13
硬件接口   +

使用的库文件
Passwordlock.h	自定义
Keypad.h	标
Wire.h	标
LiquidCrystal_I2C.h	标
SoftwareSerial.h	标
使用的是多文件编写,主要使用的是C(不包括库文件{头文件})，现在只是写完,但为完善

例如:

下次添加(可选):
1.开门一直不关检测  2.NFC开门(刷卡)    3.WLAN(无线，互联网，远程)开门

4.可找回密码(需要USB连入电脑/手机)并且要管理员密码.....

想办法进行断电保存...

说明:
接键输入密码开门，可以改密码,并且可以删除输入(#键);
蓝牙输入密码开门，可以改密码。
互不冲突(不同密码)也可以同密码需改下。
蓝牙连接上可提示。
有任何问题会亮灯并在usb波特9800报错。

请复制Otherfunctions.cpp和Passwordlock.h和Passwordlock.ino文件
如果没有使用的标库文件请留言或联系我.
个人邮箱:Starry_night_mc@outlook.com
有BUG也反馈.
