#include "ProcessData.h"

void createInitialFile() {
    FILE *initialFile = fopen("processed_data.json", "w");

    if (initialFile == NULL) {
        perror("Error opening processed_data.json");
        return;
    }
    fprintf(initialFile, "{ \"result\" : [] }");
    fclose(initialFile);
}

// Function to retrieve data from raw_data.json, process it, and write to processed_data.json
void retrieveAndWriteData() {
    // Open the raw_data.json file for reading
    FILE *raw_file = fopen("raw_data.json", "r");
    if (raw_file == NULL) {
        perror("Error opening input file");
        return;
    }

    // Determine the size of the raw_data.json file
    fseek(raw_file, 0, SEEK_END);
    long fileSize = ftell(raw_file);
    fseek(raw_file, 0, SEEK_SET);

    // Check if the file is empty or has an invalid size
    if (fileSize <= 0) {
        printf("ERROR: Empty file or file size is not valid.\n");
        fclose(raw_file);
        return;
    }

    // Allocate memory for storing the content of raw_data.json
    char *raw_file_data = (char *)malloc(fileSize + 1);
    if (raw_file_data == NULL) {
        printf("ERROR: Memory allocation failed.\n");
        fclose(raw_file);
        return;
    }

    // Read the content of raw_data.json into raw_file_data
    size_t len = fread(raw_file_data, 1, fileSize, raw_file);
    if (len != (size_t)fileSize) {
        printf("ERROR: Unable to read the entire file.\n");
        fclose(raw_file);
        free(raw_file_data);
        return;
    }

    // Null-terminate the raw_file_data
    raw_file_data[len] = '\0';

    // Parse raw_file_data as JSON using cJSON
    cJSON *json = cJSON_Parse(raw_file_data);
    free(raw_file_data);

    // Check if JSON parsing was successful
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            printf("ERROR: %s\n", error_ptr);
        }
        cJSON_Delete(json);
        fclose(raw_file);
        return;
    }

    // Extract the 'days' array from the JSON structure
    cJSON *daysArray = cJSON_GetObjectItemCaseSensitive(json, "days");

    // Check if 'days' is an array
    if (cJSON_IsArray(daysArray)) {
        // Create a new JSON array for processed data
        cJSON *resultArray = cJSON_CreateArray();

        // Get the size of the 'days' array
        int array_size = cJSON_GetArraySize(daysArray);

        // Loop through each item in the 'days' array
        for (int i = 0; i < array_size; i++) {
            cJSON *day_item = cJSON_GetArrayItem(daysArray, i);

            // Check if the item is an object
            if (cJSON_IsObject(day_item)) {
                // Extract individual elements from the day object
                cJSON *datetime = cJSON_GetObjectItemCaseSensitive(day_item, "datetime");
                cJSON *temp = cJSON_GetObjectItemCaseSensitive(day_item, "temp");
                cJSON *windspeed = cJSON_GetObjectItemCaseSensitive(day_item, "windspeed");
                cJSON *humidity = cJSON_GetObjectItemCaseSensitive(day_item, "humidity");
                cJSON *dew = cJSON_GetObjectItemCaseSensitive(day_item, "dew");
                cJSON *description = cJSON_GetObjectItemCaseSensitive(day_item, "description");

                // Creating a new dictionary for each day which will help in the report later on
                cJSON *dayDict = cJSON_CreateObject();
                cJSON_AddItemToObject(dayDict, "datetime", cJSON_Duplicate(datetime, 1));
                cJSON_AddItemToObject(dayDict, "temp", cJSON_Duplicate(temp, 1));
                cJSON_AddItemToObject(dayDict, "windspeed", cJSON_Duplicate(windspeed, 1));
                cJSON_AddItemToObject(dayDict, "humidity", cJSON_Duplicate(humidity, 1));
                cJSON_AddItemToObject(dayDict, "dew", cJSON_Duplicate(dew, 1)); 
                cJSON_AddItemToObject(dayDict, "description", cJSON_Duplicate(description, 1)); 

                // Adding the dictionaries to the Result Array 
                cJSON_AddItemToArray(resultArray, dayDict);
            } else {
                printf("ERROR: Element at index %d is not an object.\n", i);
            }
        }

        // Open the existing processed_data.json file for reading
        FILE *processed_file = fopen("processed_data.json", "r");
        if (processed_file == NULL) {
        // If the file doesn't exist, create it and write the initial format
        createInitialFile();
        processed_file = fopen("processed_data.json", "r");
        }
        
        if (processed_file != NULL) {
            // Determine the size of the existing processed_data.json file
            fseek(processed_file, 0, SEEK_END);
            long existingFileSize = ftell(processed_file);
            fseek(processed_file, 0, SEEK_SET);

            // Allocate memory for storing the content of processed_data.json
            char *existingFileData = (char *)malloc(existingFileSize + 1);
            if (existingFileData != NULL) {
                // Read the content of processed_data.json into existingFileData
                size_t existingFileLen = fread(existingFileData, 1, existingFileSize, processed_file);
                if (existingFileLen == (size_t)existingFileSize) {
                    existingFileData[existingFileLen] = '\0';

                    // Parse existingFileData as JSON using cJSON
                    cJSON *existingJson = cJSON_Parse(existingFileData);
                    free(existingFileData);

                    // Check if JSON parsing was successful
                    if (existingJson != NULL) {
                        // Get the 'result' array from the existing JSON
                        cJSON *existingResultArray = cJSON_GetObjectItemCaseSensitive(existingJson, "result");

                        // Check if 'result' is an array
                        if (cJSON_IsArray(existingResultArray)) {
                            // Loop through each item in the resultArray and add it to existingResultArray
                            for (int i = 0; i < cJSON_GetArraySize(resultArray); i++) {
                                cJSON *dayDict = cJSON_GetArrayItem(resultArray, i);
                                cJSON_AddItemToArray(existingResultArray, cJSON_Duplicate(dayDict, 1));
                            }

                            // Open the processed_data.json file for writing
                            FILE *output_file = fopen("processed_data.json", "w");
                            if (output_file == NULL) {
                                perror("Error opening output file");
                                cJSON_Delete(json);
                                cJSON_Delete(existingJson);
                                fclose(processed_file);
                                return;
                            }

                            // Convert existingJson to a string and write it to output_file
                            char *outputJsonString = cJSON_Print(existingJson);
                            fprintf(output_file, "%s", outputJsonString);

                            // Close the output file
                            fclose(output_file);

                            // Free memory and delete cJSON structures
                            cJSON_free(outputJsonString);
                            cJSON_Delete(existingJson);

                        } else {
                            printf("ERROR: 'result' is not an array in the existing JSON.\n");
                            cJSON_Delete(existingJson);
                        }
                    } else {
                        const char *error_ptr = cJSON_GetErrorPtr();
                        if (error_ptr != NULL) {
                            printf("ERROR: %s\n", error_ptr);
                        }
                        cJSON_Delete(existingJson);
                    }
                } else {
                    printf("ERROR: Unable to read the existing file.\n");
                }

                // Close the processed_data.json file
                fclose(processed_file);
            } else {
                printf("ERROR: Memory allocation failed.\n");
            }
        } else {
            printf("ERROR: Unable to open processed_output.json for reading.\n");
        }
    } else {
        printf("ERROR: 'days' is not an array in the JSON.\n");
    }

    // Cleanup: Free memory and close files
    cJSON_Delete(json);
    fclose(raw_file);
}

// Main function
int main() {
    // Call the retrieveAndWriteData function
    retrieveAndWriteData();
    return 0;
}
