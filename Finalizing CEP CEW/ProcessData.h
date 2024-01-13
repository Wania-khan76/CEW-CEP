#ifndef PROCESSDATA_H
#define PROCESSDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

// Structure to hold data for a day
typedef struct {
    cJSON *datetime;
    cJSON *temp;
    cJSON *windspeed;
    cJSON *humidity;
    cJSON *dew;
    cJSON *description;
} DayData;

// Function to retrieve and process data
void retrieveAndWriteData();

#endif // PROCESSDATA_H
