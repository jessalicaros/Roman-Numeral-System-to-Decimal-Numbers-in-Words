#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// Roman Numeral Conversion Functions
// Convert a single Roman numeral character to its decimal equivalent
int romanCharToDecimal(char romanChar) {
    switch (romanChar) {
        case 'I': 
            return 1; // 'I' represents 1
        case 'V':
             return 5; // 'V' represents 5
        case 'X': 
            return 10; // 'X' represents 10
        case 'L': 
            return 50; // 'L' represents 50
        case 'C': 
            return 100; // 'C' represents 100
        case 'D': 
            return 500; // 'D' represents 500
        case 'M': 
            return 1000; // 'M' represents 1000
        default: 
        return 0; // Return 0 for invalid Roman numeral characters
    }
}

// Convert a Roman numeral string to its decimal equivalent
int romanStringToDecimal(const char *romanStr) {
    int decimalValue = 0; // Initialize the total decimal value to 0
    int length = strlen(romanStr); // Get the length of the Roman numeral string

    // Iterate through each character in the Roman numeral string
    for (int i = 0; i < length; i++) {
        int currentValue = romanCharToDecimal(romanStr[i]); // Current Roman numeral value
        int nextValue = romanCharToDecimal(romanStr[i + 1]); // Next Roman numeral value (if exists)

        // If the next value is greater, subtract the current value (e.g., IX = 10 - 1)
        if (i + 1 < length && currentValue < nextValue) {
            decimalValue -= currentValue;
        } else { // Otherwise, add the current value
            decimalValue += currentValue;
        }
    }

    return decimalValue; // Return the total decimal value
}



// Number to Words Conversion Logic
// Convert a number into its word representation
void numberToWords(int number, char *output) {
    // Arrays for units, teens, tens, and large units
    const char *units[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    const char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    const char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    const char *largeUnits[] = {"", "Thousand", "Million", "Billion"};

    if (number == 0) { // If number is zero, return "Zero"
        strcpy(output, "Zero");
        return;
    }

    output[0] = '\0'; // Initialize the output buffer
    int groups[4] = {0}; // Array to store groups of three digits
    int groupIndex = 0; // Index to track groups

    // Split the number into groups of three digits
    while (number > 0) {
        groups[groupIndex++] = number % 1000; // Store the last three digits
        number /= 1000; // Remove the last three digits from the number
    }

    // Process each group of three digits
    for (int i = groupIndex - 1; i >= 0; i--) {
        if (groups[i] == 0) continue; // Skip empty groups

        int hundreds = groups[i] / 100; // Extract hundreds digit
        int remainder = groups[i] % 100; // Extract the last two digits

        if (hundreds > 0) {
            sprintf(output + strlen(output), "%s Hundred ", units[hundreds]); // Append hundreds word
        }

        if (remainder >= 10 && remainder < 20) { // Handle teen numbers
            strcat(output, teens[remainder - 10]);
        } else { // Handle tens and units separately
            if (remainder >= 20) {
                strcat(output, tens[remainder / 10]); // Append tens word
                strcat(output, " ");
            }
            if (remainder % 10 > 0) {
                strcat(output, units[remainder % 10]); // Append units word
            }
        }

        if (i > 0) { // Append large unit (e.g., Thousand, Million)
            strcat(output, " ");
            strcat(output, largeUnits[i]);
            strcat(output, " ");
        }
    }

    // Remove trailing spaces
    int len = strlen(output);
    if (output[len - 1] == ' ') output[len - 1] = '\0';
}


// File Input/Output Operations
// Read Roman numeral strings from a file
int readRomanNumerals(const char *filename, char romanNumerals[][20]) {
    FILE *file = fopen(filename, "r"); // Open the file in read mode
    if (!file) { // Check if file opening failed
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    int count = 0; // Counter for the number of Roman numerals read
    while (fgets(romanNumerals[count], 20, file)) { // Read each line from the file
        romanNumerals[count][strcspn(romanNumerals[count], "\n")] = '\0'; // Remove newline character
        count++;
    }

    fclose(file); // Close the file
    return count; // Return the number of Roman numerals read
}

// Write decimal results in word format to a file
void writeResultsToFile(const char *filename, int results[], int count) {
    FILE *file = fopen(filename, "w"); // Open the file in write mode
    if (!file) { // Check if file opening failed
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    char wordBuffer[256]; // Buffer to store the word representation of numbers
    for (int i = 0; i < count; i++) {
        numberToWords(results[i], wordBuffer); // Convert number to words
        fprintf(file, "%s\n", wordBuffer); // Write the word representation to the file
    }

    fclose(file); // Close the file
}


// Main Program Logic

int main() {
    char romanNumerals[100][20]; // Array to store Roman numerals from the input file
    int decimalResults[100]; // Array to store decimal equivalents of the Roman numerals
    int totalRomans; // Total number of Roman numerals read

    // Step 1: Read Roman numerals from input file
    totalRomans = readRomanNumerals("input.txt", romanNumerals);

    // Step 2: Convert Roman numerals to decimal numbers
    for (int i = 0; i < totalRomans; i++) {
        decimalResults[i] = romanStringToDecimal(romanNumerals[i]);
    }

    // Step 3: Adjust results (add last two values if total is greater than 3)
    if (totalRomans > 3) {
        decimalResults[2] += decimalResults[3];
    }

    // Step 4: Write results to output file in word format
    writeResultsToFile("output.txt", decimalResults, totalRomans - 1);

    return 0; 
}