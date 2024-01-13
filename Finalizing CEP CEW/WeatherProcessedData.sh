# List of source files to compile and execute
files=("WeatherAPI.c" "ProcessData.c")

# Loop through each source file
for file in "${files[@]}"; do
    # Extract the filename without the extension
    filename="${file%.*}"

    # Compile the source file using GCC with necessary libraries and include paths
    gcc -o "$filename" "$file" -lcjson -lcurl -I/ahzam/include/cjson -I/ahzam/include

    # Check if the compilation was successful
    if [ $? -eq 0 ]; then
        # Print a success message and execute the compiled program
        echo "Compilation successful for $file"
        "./$filename"
    else
        # Print a failure message and skip execution if compilation failed
        echo "Compilation failed for $file. Skipping execution."
    fi
done
