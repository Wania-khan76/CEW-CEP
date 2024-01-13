#ifndef WEATHER_API_H
#define WEATHER_API_H

#include <curl/curl.h>
#include <cjson/cJSON.h>

// Structure to hold retrieved data
struct MemoryStruct {
    char *memory;
    size_t size;
};

// Callback function to write retrieved data to a memory buffer
size_t write_callback(void *contents, size_t size, size_t nmemb, struct MemoryStruct *mem);

// Function to fetch weather data from a specified URL and save it to a file
int fetch_weather_data(const char *url, const char *output_filename);

#endif  // WEATHER_API_H
