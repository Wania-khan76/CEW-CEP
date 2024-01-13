// Include necessary header files
#include "EmailSender.h"
#include <stdio.h>
#include <curl/curl.h>

// Callback function to read data from a file
size_t read_callback(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t retcode = fread(ptr, size, nmemb, stream);
    return retcode;
}

// Function to send an email with attachment
int Email(const char *to, const char *report_path, const char *anomaly_path) {
    CURL *curl;
    CURLcode res = CURLE_OK;

    // Open report and anomaly files in binary mode
    FILE *report_file = fopen(report_path, "rb");
    FILE *anomaly_file = fopen(anomaly_path, "rb");

    // Check if file(s) opened successfully
    if (!report_file || !anomaly_file) {
        fprintf(stderr, "Error opening file(s).\n");
        if (report_file) fclose(report_file);
        if (anomaly_file) fclose(anomaly_file);
        return 1; // Return error code
    }

    // Initialize the libcurl library
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        struct curl_slist *recipients = NULL;

        // Specify email server details (For Gmail)
        curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com");
        curl_easy_setopt(curl, CURLOPT_USERPWD, "ahzambari@gmail.com:golo riby raft jiup");
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

        // Add email recipients
        if (to)
            recipients = curl_slist_append(recipients, to);
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        // Set the read callback function to read the report file content
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(curl, CURLOPT_READDATA, report_file);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        // Perform the email sending for the report file
        res = curl_easy_perform(curl);

        // Check if email sending for the report file was successful
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        // Reset read callback function for the anomaly file
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(curl, CURLOPT_READDATA, anomaly_file);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        // Perform the email sending for the anomaly file
        res = curl_easy_perform(curl);

        // Check if email sending for the anomaly file was successful
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        // Clean up
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }

    // Close the files
    fclose(report_file);
    fclose(anomaly_file);

    // Clean up libcurl
    curl_global_cleanup();

    return (int)res; // Return the result of the email sending operation
}

// Main function
int main() {
    // Define email details and file paths
    const char *to = "rehanahzam@gmail.com";
    const char *report_path = "Report.txt";
    const char *anomaly_path = "AnomalyLog.txt";

    // Send email and capture the result
    int result = Email(to, report_path, anomaly_path);

    // Display success or failure message based on the result
    if (result == 0) {
        printf("Email sent successfully.\n");
    } else {
        printf("Failed to send email.\n");
    }

    return 0;
}
