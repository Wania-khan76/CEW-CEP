#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

#define MAX_ENTRIES 30

struct WeatherData {
    char datetime[11];
    float temp;
    float windspeed;
    float humidity;
    float dew;
    char description[50];
};

void calculateAverages(struct WeatherData data[], int size, float *avgTemp, float *avgWindSpeed, float *avgHumidity, float *avgDew) {
    float sumTemp = 0, sumWindSpeed = 0, sumHumidity = 0, sumDew = 0;

    for (int i = 0; i < size; i++) {
        sumTemp += data[i].temp;
        sumWindSpeed += data[i].windspeed;
        sumHumidity += data[i].humidity;
        sumDew += data[i].dew;
    }

    *avgTemp = sumTemp / size;
    *avgWindSpeed = sumWindSpeed / size;
    *avgHumidity = sumHumidity / size;
    *avgDew = sumDew / size;
}

int readJsonFromFile(const char *filename, struct WeatherData data[], int *size) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *json_buffer = (char *)malloc(file_size + 1);
    fread(json_buffer, 1, file_size, file);
    fclose(file);

    json_buffer[file_size] = '\0';

    cJSON *root = cJSON_Parse(json_buffer);
    if (!root) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        free(json_buffer);
        return -1;
    }

    cJSON *result = cJSON_GetObjectItem(root, "result");
    *size = cJSON_GetArraySize(result);

    for (int i = 0; i < *size; i++) {
        cJSON *item = cJSON_GetArrayItem(result, i);
        sscanf(cJSON_GetObjectItem(item, "datetime")->valuestring, "%s", data[i].datetime);
        data[i].temp = cJSON_GetObjectItem(item, "temp")->valuedouble;
        data[i].windspeed = cJSON_GetObjectItem(item, "windspeed")->valuedouble;
        data[i].humidity = cJSON_GetObjectItem(item, "humidity")->valuedouble;
        data[i].dew = cJSON_GetObjectItem(item, "dew")->valuedouble;
        sscanf(cJSON_GetObjectItem(item, "description")->valuestring, "%[^\"]", data[i].description);
    }

    cJSON_Delete(root);
    free(json_buffer);

    return 0;
}

void generateTextFile(const char *filename, struct WeatherData data[], int dataSize, float avgTemp, float avgWindSpeed, float avgHumidity, float avgDew) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Unable to create text file\n");
        return;
    }

    fprintf(file, "Weather Report\n");
    fprintf(file, "   Date        Temp       WindSpeed     Humidity      Dew         \tDescription\n");


    for (int i = 0; i < dataSize; i++) {
        fprintf(file, "%-11s    %-8.1f     %-8.1f      %-8.1f    %-10.1f %s\n",
                data[i].datetime, data[i].temp, data[i].windspeed, data[i].humidity, data[i].dew, data[i].description);
    }

    fprintf(file, "\nAverages:\nTemp: %.2f\nWindSpeed: %.2f\nHumidity: %.2f\nDew: %.2f\n", avgTemp, avgWindSpeed, avgHumidity, avgDew);
    fclose(file);
}

int main() {
    const char *processedFilename = "processed_data.json";
    const char *textFilename = "Report.txt";

    struct WeatherData data[MAX_ENTRIES];

    int dataSize;
    if (readJsonFromFile(processedFilename, data, &dataSize) != 0) {
        return 1;
    }


    float avgTemp, avgWindSpeed, avgHumidity, avgDew;
    calculateAverages(data, dataSize, &avgTemp, &avgWindSpeed, &avgHumidity, &avgDew);
    
    generateTextFile(textFilename, data, dataSize, avgTemp, avgWindSpeed, avgHumidity, avgDew);
    printf("Text file generated successfully.\n");

    return 0;
}

