#include "WeatherAPI.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to hold retrieved data
struct MemoryStruct {
    char *memory;
    size_t size;
};

// Callback function to write retrieved data to a memory buffer
size_t write_callback(void *contents, size_t size, size_t nmemb, struct MemoryStruct *mem) {
    size_t realsize = size * nmemb;
    
    // Reallocate memory to accommodate the new data
    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    
    // Check for memory allocation failure
    if (mem->memory == NULL) {
        fprintf(stderr, "Not enough memory (realloc returned NULL)\n");
        return 0;
    }
    
    // Copy the retrieved data to the memory buffer
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0; // Null-terminate the string
    
    return realsize;
}

int main() {
    // URL for the Visual Crossing API
    const char *url = "https://weather.visualcrossing.com/VisualCrossingWebServices/rest/services/timeline/24.8607,67.0011/next14days?unitGroup=metric&key=ZW9AR97RBK5CAKJJ8RDE9DR4P";

    // Initialize libcurl
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Error initializing cURL.\n");
        return 1;
    }

    // Set the URL for the cURL request
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // MemoryStruct to hold the received data
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    // Set the callback function for writing data
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

    // Perform the request to the Visual Crossing API
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "cURL failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        free(chunk.memory);
        return 1;
    }

    // Cleanup cURL
    curl_easy_cleanup(curl);

    // Parsing Data using cJSON
    cJSON *json = cJSON_Parse(chunk.memory);
    if (!json) {
        fprintf(stderr, "Error parsing JSON data.\n");
        free(chunk.memory);
        return 1;
    }

    // Write the raw JSON data to a file
    FILE *jsonFile = fopen("raw_data.json", "w");

    if (!jsonFile) {
        fprintf(stderr, "Error opening file for writing.\n");
        cJSON_Delete(json);
        free(chunk.memory);
        return 1;
    }

    // Convert cJSON object to a formatted JSON string
    char *jsonString = cJSON_Print(json);

    // Write the JSON string to the file
    fprintf(jsonFile, "%s", jsonString);

    // Cleanup cJSON-related resources
    cJSON_Delete(json);
    free(jsonString);

    // Cleanup memory used for cURL and JSON data
    free(chunk.memory);
    fclose(jsonFile);

    return 0;
}
