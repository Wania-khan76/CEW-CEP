#include "AnomalyGenerator.h"

// Function to check and log anomalies
void checkAndLogAnomalies(WeatherData *data, int numDays) {
    // Open AnomalyLog.txt for writing
    FILE *logFile = fopen("AnomalyLog.txt", "w");

    // Check if file opening was successful
    if (!logFile) {
        perror("Error opening AnomalyLog.txt");
        return;
    }

    // Write header to the log file
    fprintf(logFile, "\nAnomaly Log:\n\n");

    // Loop through each day's weather data
    for (int i = 0; i < numDays; ++i) {
        // Check for humidity and windspeed anomalies
        if (data[i].humidity >= 65 || data[i].humidity <= 30 || data[i].windspeed >= 25) {
            fprintf(logFile, "Date: %s\n", data[i].datetime);

            // Check and log humidity anomalies
            if (data[i].humidity >= 70) {
                fprintf(logFile, "   Humidity: %.2f - Very humid, lots of moisture in the air\n", data[i].humidity);
            } else if (data[i].humidity <= 30) {
                fprintf(logFile, "   Humidity: %.2f - Very dry weather\n", data[i].humidity);
            }

            // Check and log windspeed anomalies
            if (data[i].windspeed >= 25) {
                fprintf(logFile, "   Windspeed: %.2f - Very Windy weather\n", data[i].windspeed);
            }

            // Add a newline after logging anomalies for a day
            fprintf(logFile, "\n");
        }
    }

    // Close the log file
    fclose(logFile);
}

int main() {
    // Open the JSON file for reading
    FILE *jsonFile = fopen("processed_data.json", "r");

    // Check if file opening was successful
    if (!jsonFile) {
        perror("Error opening processed_data.json");
        return 1;
    }

    // Get the file size
    fseek(jsonFile, 0, SEEK_END);
    long fileSize = ftell(jsonFile);
    fseek(jsonFile, 0, SEEK_SET);

    // Read the entire file into a buffer
    char *jsonBuffer = malloc(fileSize + 1);

    // Check if memory allocation was successful
    if (!jsonBuffer) {
        perror("Error allocating memory for JSON data");
        fclose(jsonFile);
        return 1;
    }

    // Read the JSON data into the buffer
    fread(jsonBuffer, 1, fileSize, jsonFile);
    fclose(jsonFile);

    // Null-terminate the buffer
    jsonBuffer[fileSize] = '\0';

    // Parse JSON data using cJSON
    cJSON *json = cJSON_Parse(jsonBuffer);

    // Check if JSON parsing was successful
    if (!json) {
        printf("Error parsing JSON data.\n");
        free(jsonBuffer);
        return 1;
    }

    // Get the "result" array from the JSON object
    cJSON *resultArray = cJSON_GetObjectItemCaseSensitive(json, "result");

    // Check if "result" is an array
    if (!cJSON_IsArray(resultArray)) {
        printf("Error: 'result' is not an array.\n");
        free(jsonBuffer);
        cJSON_Delete(json);
        return 1;
    }

    // Get the number of days from the array size
    int numDays = cJSON_GetArraySize(resultArray);

    // Allocate memory for WeatherData structure array
    WeatherData *weatherData = malloc(numDays * sizeof(WeatherData));

    // Check if memory allocation was successful
    if (!weatherData) {
        perror("Error allocating memory for weather data");
        free(jsonBuffer);
        cJSON_Delete(json);
        return 1;
    }

    // Parse each day's data from the JSON array
    for (int i = 0; i < numDays; ++i) {
        cJSON *dayData = cJSON_GetArrayItem(resultArray, i);

        // Parse individual fields and store in WeatherData structure
        strcpy(weatherData[i].datetime, cJSON_GetObjectItemCaseSensitive(dayData, "datetime")->valuestring);
        weatherData[i].humidity = cJSON_GetObjectItemCaseSensitive(dayData, "humidity")->valuedouble;
        weatherData[i].windspeed = cJSON_GetObjectItemCaseSensitive(dayData, "windspeed")->valuedouble;
    }

    // Check and log anomalies based on weather data
    checkAndLogAnomalies(weatherData, numDays);

    // Free allocated memory
    free(weatherData);
    free(jsonBuffer);

    // Cleanup cJSON resources
    cJSON_Delete(json);

    return 0;
}
