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

#endif  // WEATHER_API_H
