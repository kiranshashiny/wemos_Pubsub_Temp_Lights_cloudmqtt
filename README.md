# wemos_Pubsub_Temp_LivingRoom_Lights_cloudmqtt
Arduino sketch used for sending / receiving commands to turn living room and Temperature/ humidity to Broker

User has to change the Userid/Password of the router to be connected, and also enter the credentials of the broker in the Arduino sketch.

This uses a single microcontroller to control the 220-240v lights by triggering a Solid State Relay SSR over 5v.

The uC is also connected to a DHT11 Temperature and Humidity sensor and sends data to the cloud.

The broker can also be used to publish messages downstream to the controller to turn lights on or off.

The CONTROL_LIGHTS Pin 14 ( Pin D5) onboard the Wemos D1 R1 is initially turned off as I dont want to trigger the SSR to turn on the 220v/240 Living room lights on.

The onboard LED is off until the unit connects to the router ( another indicator that the connection is not done when connected )

If the onboard-blue LED is light - that means the unit/uC established the connection to the router successfully. ( Another way to debug in case things dont work as planned )

To do :
A subscribe option to tell what is the state of the SSR/ living room lights, and turn on a LED on / off.
 



Compile Parameters for Wemos D1 R1

![image](https://user-images.githubusercontent.com/14288989/178680412-45afd241-ef33-40b8-9d33-046d9b927726.png)


Upload speed is 115200 bps
![image](https://user-images.githubusercontent.com/14288989/178680481-ff98344b-752d-4b0f-ac5c-d80d949e5b58.png)

cloudmqtt Broker getting publish messages from the Wemos in house.

![image](https://user-images.githubusercontent.com/14288989/178680605-bc494a93-e8e9-4d92-bb8a-e06403d8019a.png)


Snapshot of the electrical box will be added later.



Snapshot of the JavaScript page that can turn the lights on of off. This code is sitting on my laptop. ( Tranquibar)

![image](https://user-images.githubusercontent.com/14288989/178681289-6c2fa9bb-3e99-44c2-a94a-96cd38023c12.png)

Snapshot of the Java Script using DevExtreme libraries to show the temperature and humidity.  See the date/time is also updated.

![image](https://user-images.githubusercontent.com/14288989/178681539-3e081ee9-b753-489a-b194-7b568f72e3f6.png)

