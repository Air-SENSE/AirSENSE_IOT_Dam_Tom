#include "DataStructure.h"

ERROR_CODE creatCalibDataString(char *_calibDataString, struct calibData _calibData)
{
	sprintf(_calibDataString, "%u|%u|%u|%u|%u|%u|%u\n",
							  _calibData.temperature_calibInt_u32,
							  _calibData.humidity_calibInt_u32,
							  _calibData.pm1_calibInt_u32,
							  _calibData.pm10_calibInt_u32,
							  _calibData.pm25_calibInt_u32,
							  _calibData.temperature_calibFloat_u32,
							  _calibData.humidity_calibFloat_u32 );
	if (_calibDataString[0] == '\0')
	{
		log_e("Create string failed!");
		return ERROR_DATA_CREATE_STRINGDATA_FAILED;
	} else {    
        log_e("%s",_calibDataString);

		log_i("Create string complete!");
		return ERROR_NONE;
	}
}


ERROR_CODE createSensorDataString(char *_sensorDataString,
                                  const char *_nameDevice,
                                  const char *_dateTimeString,
                                  struct sensorData _sensorData_st)
{
    strcpy(_sensorDataString, "");
    sprintf(_sensorDataString, "%s,%s,%.1f,%.1f,%f,%u,",
                                _nameDevice,
                                _dateTimeString,
                                _sensorData_st.salinityVoltage_f,
                                _sensorData_st.temperatureVoltage_f,
                                _sensorData_st.phVoltage_f,
                                _sensorData_st.doVoltage_f);
    
	if (_sensorDataString[0] == '\0')
	{
		log_e("Create string failed!");
		return ERROR_DATA_CREATE_STRINGDATA_FAILED;
	} else {
        
        log_e("%s",_sensorDataString);
		log_i("Create string complete!");
		return ERROR_NONE;
	}
}

ERROR_CODE createMessageMQTTString( String &messageMQTTString,
                                    const char *_espID,
                                    NTPClient& _timeClient,
                                    struct sensorData _sensorData_st)
{
	_timeClient.update();

    StaticJsonDocument<200> doc;

    doc["station_id"] = _espID;
    doc["Time"] = _timeClient.getEpochTime();
    
    doc["SalinityVoltage"] = _sensorData_st.salinityVoltage_f;
    doc["PHVoltage"] = _sensorData_st.phVoltage_f;
    doc["DOVoltage"] = _sensorData_st.doVoltage_f;
    doc["TemperatureVoltage"]  = _sensorData_st.temperatureVoltage_f;

    String output;
    serializeJson(doc, output);
    messageMQTTString = output;
    return ERROR_NONE;
}