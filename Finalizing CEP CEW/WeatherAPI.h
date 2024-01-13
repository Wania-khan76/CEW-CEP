#ifndef WEATHER_API_H
#define WEATHER_API_H

#include <curl/curl.h>
#include <cjson/cJSON.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};

size_t write_callback(void *contents, size_t size, size_t nmemb, struct MemoryStruct *mem);

int fetch_weather_data(const char *url, const char *output_filename);

#endif  // WEATHER_API_H
