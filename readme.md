<p align="center"><h1>ESP8266 HomeKit 室內溫濕度 + 空氣品質計</h1></p>

<p align="center"><h3>ESP8266 實際接線圖</h3></p>
<p align="left"><img width="400" src="./1.png"></p>

<p align="center"><h3>在家庭 App 中顯示</h3></p>
<p align="left"><img width="300" src="./2.png"></p>

<p align="center"><h3>透過 Siri 查詢</h3></p>
<p align="left"><img width="300" src="./3.png"></p>
<br/>

## 編輯 wifi 設定<br/>
打開 TemperatureSensor/wifi_info.h，修改以下兩行:<br/>
```
const char *ssid = "your-wifi-ssid";
const char *password = "your-wifi-password";
```
<br/>

## 主要檔案結構<br/>
```
TemperatureSensor/
    TemperatureSensor.ino   ---> ESP8266 主檔，感測器接收數據都在這裡跑
    my_accessory.c          ---> 定義 HomeKit 裝置
    wifi_info.h             ---> wifi 設定
```
<br/>

## 使用程式庫<br/>
https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266
<br/>
