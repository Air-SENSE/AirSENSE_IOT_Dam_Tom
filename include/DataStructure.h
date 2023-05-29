/*
 * @file DataStructure.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-11
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <Arduino.h>
#include <string>
#include "esp_log.h"
#include "configs.h"
#include <ArduinoJson.h>
#include <NTPClient.h>

#define ERROR_DATA_CREATE_STRINGDATA_FAILED 0x91




struct calibData {
    uint32_t temperature_calibInt_u32;
    uint32_t humidity_calibInt_u32;
    uint32_t pm1_calibInt_u32;
    uint32_t pm10_calibInt_u32;
    uint32_t pm25_calibInt_u32;
    uint32_t temperature_calibFloat_u32;
    uint32_t humidity_calibFloat_u32;

    calibData() {     // initialation function for struct (C++ style)
        this->temperature_calibInt_u32      = 0;
        this->humidity_calibInt_u32         = 0;
        this->pm1_calibInt_u32              = 0;
        this->pm10_calibInt_u32             = 0;
        this->pm25_calibInt_u32             = 0;
        this->temperature_calibFloat_u32    = 0;
        this->humidity_calibFloat_u32       = 0;
    }
};



/**
 * @brief Create a string from sensor data for storing in the SD card
 * 
 * @param[out] _calibDataString: pointer char*, use to store string
 * @param[in]  _calibData: struct calibdata
 * @return ERROR_CODE 
 */
ERROR_CODE creatCalibDataString(char *_calibDataString, struct calibData _calibData);

struct sensorData {
    float phVoltage_f;  
    float doVoltage_f;
    float temperatureVoltage_f;
    float salinityVoltage_f;

    sensorData() 
    {
        this->phVoltage_f	         = 0;
        this->doVoltage_f		     = 0;
        this->temperatureVoltage_f   = 0;
        this->salinityVoltage_f      = 0;
    };


    sensorData operator=(const sensorData _sensorData_temp)
    {
        
        this->phVoltage_f	         = _sensorData_temp.phVoltage_f;
        this->doVoltage_f		     = _sensorData_temp.doVoltage_f;
        this->temperatureVoltage_f   = _sensorData_temp.temperatureVoltage_f;
        this->salinityVoltage_f      = _sensorData_temp.salinityVoltage_f;

        return _sensorData_temp;
    }
};


/**
 * @brief Create a string
 * 
 * @param[out] _calibDataString: pointer char*, use to store string
 * @param[in]  _calibData: struct calibdata
 * @return ERROR_CODE 
 */
ERROR_CODE createSensorDataString(char *_sensorDataString,
                                  const char *_nameDevice,
                                  const char *_dateTimeString,
                                  struct sensorData _sensorData_st);

/**
 * @brief Create a MQTT string
 * 
 * @param[out] _calibDataString: pointer char*, use to store string
 * @param[in]  _calibData: struct calibdata
 * @return ERROR_CODE 
 */
ERROR_CODE createMessageMQTTString( String &messageMQTTString,
                                    const char *_espID,
                                    NTPClient& _timeClient,
                                    struct sensorData _sensorData_st);


#endif