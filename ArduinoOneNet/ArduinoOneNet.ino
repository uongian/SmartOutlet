//引入ESP8266.h头文件，建议使用教程中修改后的文件
#include "ESP8266.h"
#include "SoftwareSerial.h"

//配置ESP8266WIFI设置
#define SSID "k"    //填写2.4GHz的WIFI名称，不要使用校园网
#define PASSWORD "xhjxhjxhj"//填写自己的WIFI密码
#define HOST_NAME "api.heclouds.com"  //API主机名称，连接到OneNET平台，无需修改
#define DEVICE_ID "859642542"       //填写自己的OneNet设备ID
#define HOST_PORT (80)                //API端口，连接到OneNET平台，无需修改
String APIKey = "z51k7ZIQCdjxhtnZKUgZHGIQI1g="; //与设备绑定的APIKey


#define SWITCH "GET http://api.heclouds.com/devices/859642542/datapoints?Station HTTP/1.1\napi-key:z51k7ZIQCdjxhtnZKUgZHGIQI1g=\nHost: api.heclouds.com\n\nConnection: close\r\n\r\n"
#define INTERVAL_SENSOR 500 //定义传感器采样及发送时间间隔

//定义ESP8266所连接的软串口
/*********************
 * 该实验需要使用软串口
 * Arduino上的软串口RX定义为D3,
 * 接ESP8266上的TX口,
 * Arduino上的软串口TX定义为D2,
 * 接ESP8266上的RX口.
 * D3和D2可以自定义,
 * 但接ESP8266时必须恰好相反
 *********************/
SoftwareSerial mySerial(3, 2);
ESP8266 wifi(mySerial);

const int digitalPin = 7;
const int ledPin = 11;
//读取按键开关状态的变量
int state = HIGH;

void setup() {
  mySerial.begin(115200); //初始化软串口
  Serial.begin(9600);     //初始化串口
  Serial.print("setup begin\r\n");

  //以下为ESP8266初始化的代码
  //Serial.print("FW Version: ");
  //Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStation()) {
    Serial.print("to station ok\r\n");
  } else {
    Serial.print("to station err\r\n");
  }
  if (wifi.joinAP(SSID, PASSWORD)) {
    //Serial.print("Join AP success\r\n");
    //Serial.print("IP: ");
    //Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.print("Join AP failure\r\n");
  }
  
  mySerial.println("AT+UART_CUR=9600,8,1,0,0");
  mySerial.begin(9600);
  //Serial.println("setup end\r\n");
  
  Serial.println("Initialisation complete.");//调试信息,在串口中显示！
}

unsigned long net_time1 = millis(); //数据上传服务器时间
bool flag=true;
int i=0;
void loop() {
  Get();
  
    state = digitalRead(digitalPin);//读取//按键开关的状态
  //Serial.println(state);//将检测状态输出//到串口
    if (state == LOW)
    {
      // 熄灭LED
      if(flag==false)
      {
        digitalWrite(ledPin, HIGH);
      }else{
        digitalWrite(ledPin, LOW);
      }
    }
    else
    {
      if(flag==true){
        digitalWrite(ledPin, HIGH);
        }else{
        digitalWrite(ledPin, LOW);
        }
    }
  
      int chk = digitalRead(ledPin);
      String sensor="NULL";
      //Serial.print("Read sensor: ");
      switch (chk) {
        case HIGH:
          //Serial.println("HIGH");
          sensor="HIGH";
          break;
        case LOW:
          //Serial.println("LOW");
          sensor="LOW";
          break;
        default:
          Serial.println("Unknown error");
          break;
      }
  
  
      Serial.print("station: ");
      Serial.print(sensor);
    

      
    
    

    
      //sensor="TRUE";
      Post(sensor);
  
      Serial.println("");
}
void Get(){
    uint8_t buffer[1024] = {0};
    String GET;
    if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
        Serial.print("create tcp ok\r\n");
        char *hello = SWITCH;
        
        wifi.send((const uint8_t*)hello, strlen(hello));
        GET = wifi.recv(buffer, sizeof(buffer), 10000);



        Serial.println(GET);
        int n=GET.indexOf("TRUE");
        Serial.println(n);
        if(n!=-1){
          flag=!flag;
          Serial.println("llllllll");
        }else{
          Serial.println("2222222");
        }
     
    } else {
        Serial.println("create tcp err\r\n");
    }
       
    if (wifi.releaseTCP()) {
            Serial.println("release tcp ok\r\n");
        } else {
            Serial.println("release tcp err\r\n");
        }
  }


void Post(String sensor){
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据
        Serial.print("create tcp ok\r\n");
        //拼接发送data字段字符串
        String jsonToSend = "{\"Station\":";
        jsonToSend += "\"" + sensor + "\"";
        jsonToSend += ",\"Change\":";
        jsonToSend += "\"FALSE\"";
        jsonToSend += "}";
  
        //拼接POST请求字符串
        String postString = "POST /devices/";
        postString += DEVICE_ID;
        postString += "/datapoints?type=3 HTTP/1.1";
        postString += "\r\n";
        postString += "api-key:";
        postString += APIKey;
        postString += "\r\n";
        postString += "Host:api.heclouds.com\r\n";
        postString += "Connection:close\r\n";
        postString += "Content-Length:";
        postString += jsonToSend.length();
        postString += "\r\n";
        postString += "\r\n";
        postString += jsonToSend;
        postString += "\r\n";
        postString += "\r\n";
        postString += "\r\n";
        //Serial.print("1\n");
        //Serial.print(postString);
        //Serial.print("1\n");
        const char *postArray = postString.c_str(); //将str转化为char数组
  
        Serial.println(postArray);
        wifi.send((const uint8_t *)postArray, strlen(postArray)); //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
        Serial.println("send success");

        
        if (wifi.releaseTCP()) { //释放TCP连接
          Serial.print("release tcp ok\r\n");
        } else {
          Serial.print("release tcp err\r\n");
        }
        postArray = NULL; //清空数组，等待下次传输数据
      } else {
        Serial.print("create tcp err\r\n");
      }
  }
