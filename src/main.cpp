#include "main.h"


void device_getData()
{
  struct sensorData sensorDataTemp_st;
  if(Serial2.available() > 0)
  {
    String _data_str = Serial2.readString();
    log_e("%s",_data_str);
    StaticJsonDocument<500> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, _data_str);

    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    // Fetch values.
    //
    // Most of the time, you can rely on the implicit casts.
    // In other case, you can do doc["time"].as<long>();
    sensorDataTemp_st.salinityVoltage_f = doc["SalinityVoltage"];
    sensorDataTemp_st.salinityVoltage_f = doc["TemperatureVoltage"];
    sensorDataTemp_st.salinityVoltage_f = doc["PHVoltage"];
    sensorDataTemp_st.salinityVoltage_f = doc["DOVoltage"];
    
    sensorDataTemp_st.salinity_f = doc["Salinity"];
    sensorDataTemp_st.ph_f = doc["PH"];
    sensorDataTemp_st.do_f = doc["DO"];
    sensorDataTemp_st.temperature_c_f = doc["Temperature"];
  
  
  }
 
}

void device_dataManagement()
{  
  struct sensorData sensorDataTemp_st;
  sensorDataTemp_st = sensorData_st;
  DS3231_getStringDateTime(realTime, DateTime::TIMESTAMP_FULL , dateTime_string);	
  createSensorDataString(sensorDataString, (const char *)NAME_DEVICE, dateTime_string ,sensorDataTemp_st);
	DS3231_getStringDateTime(realTime, DateTime::TIMESTAMP_DATE, nameFileSaveData);
	SDcard_saveStringDataToFile(&connectionStatus_st, sensorDataString);
  createMessageMQTTString(messageData, (const char *)espID, timeClient, sensorDataTemp_st);
  MQTT_postData(messageData.c_str(), &connectionStatus_st, mqttClient);
}

void setup() {
    Serial.begin(SERIAL_DEBUG_BAUDRATE);
    Serial2.begin(SERIAL_DATA_STREAM_BAUDRATE);
    log_e("Booting...");

    WIFI_init();
	  MQTT_initClient(topic, espID, mqttClient, &connectionStatus_st);
	  timeClient.begin();

    Wire.begin(PIN_SDA_GPIO, PIN_SCL_GPIO, I2C_CLOCK_SPEED);
	  DS3231_init(realTime, timeClient, Wire, connectionStatus_st);
	  SDcard_init(PIN_NUM_CLK, PIN_NUM_MISO, PIN_NUM_MOSI, PIN_CS_SD_CARD, &connectionStatus_st);

    log_e("Init Done");
}

unsigned long device_previousDataControl = 0;
unsigned long device_previousWifiReconnect = 0;

void loop() {
  // get data from all sensor
  device_getData();
  if(millis() - device_previousDataControl > DEVICE_DATA_SAVE_INTERVAL_TIME) 
  {
    device_dataManagement();
    device_previousDataControl = millis();
  }
  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  if ((WiFi.status() != WL_CONNECTED) && (millis() - device_previousWifiReconnect >= WIFI_CONNECT_INTERVAL)) {
    log_e("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    device_previousWifiReconnect = millis();
  }

}