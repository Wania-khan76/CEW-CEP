Environmental Monitoring System in C:

	Overview:

The Environmental Monitoring System is a comprehensive C-based project designed to collect, process, analyze, and report real-time weather data. The system is modular, featuring distinct components that address specific aspects of environmental monitoring. It encompasses weather data retrieval from an external API, data processing and analysis, anomaly detection, report generation, and email alerts.

	Table of Contents:
1.	Getting Started
2.	Prerequisites
3.	Installation
4.	Usage
5.	Code Structure
6.	Weather API Module
7.	Report Generator Module
8.	Data Processing Module
9.	Anomaly Checker Module
10.	Email Sender Module
11.	Technologies Used
12.	Contributing
13.	License


	Getting Started:
        To quickly get started with the Environmental Monitoring System, follow the steps outlined                below.

	Prerequisites:

Before installing and running the system, ensure that you have the following prerequisites installed on your system:
     libcurl: A library for making HTTP requests.
     cJSON: A lightweight JSON parsing library in C.



	Installation
1.	Clone the repository:
                 git clone https://github.com/Wania-khan76/CEW-CEP/tree/main/Finalizing%20CEP%20CEW
2.	Navigate to the project directory:
                      cd environmental-monitoring-system
3.	Compile the code using your preferred C compiler.
4.	For example:
               gcc main.c -lcurl -lcjson -o monitor
  	
 Usage

    Run the compiled executable:
               ./monitor
Follow the on-screen instructions to retrieve, process, and analyze weather data.


	Code Structure:

The project is organized into several header files, each responsible for a specific functionality.

Weather API Module libcurl

•	weather_api.h: 
This module handles the retrieval of weather data from an external API using the library. It defines a structure to hold retrieved data and a callback function to write the data to a memory buffer.

Report Generator Module

•	reportgenerator.h: The report generator module manages the creation of detailed reports based on the processed weather data. It defines a structure (WeatherData) to store essential weather parameters, such as datetime, temperature, windspeed, humidity, dew, and description.

Data Processing Module

•	processdata.h: The data processing module processes and analyzes weather data using the cJSON library for JSON parsing. It defines a structure (DayData) to hold data for a day, including datetime, temperature, windspeed, humidity, dew, and description.

Anomaly Checker Module

•	anomaly_checker.h: The anomaly checker module identifies and logs anomalies in humidity and windspeed. It defines a structure (WeatherData) to represent weather parameters for anomaly checking.

Email Sender Module

•	email_sender.h: The email sender module provides functionality to read file content and send an email with a report and anomaly file attachment.	
Technologies Used:

The Environmental Monitoring System utilizes the following key technologies:

•	libcurl: A versatile and easy-to-use library for making HTTP requests.
•	cJSON: A lightweight JSON parsing library in C.

Contributing
Contributions to the project are welcome! Please follow the Contribution Guidelines for details on how to contribute.

License
This project is licensed under the MIT License - see the LICENSE file for details.

