#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

// Converts a 32-bit unsigned integer to a 32-character binary string.

void to_32bit_binary_string(uint32_t value, char *out) {
    for (int i = 31; i >= 0; i--) {
        out[31 - i] = ((value >> i) & 1) ? '1' : '0';
    }
    out[32] = '\0';
}


//Converts a single octal digit ('0'-'7') to its 3-bit binary string using if-else.

const char* oct_digit_to_bin_map(char oct_digit) {
    if (oct_digit == '0')      return "000";
    else if (oct_digit == '1') return "001";
    else if (oct_digit == '2') return "010";
    else if (oct_digit == '3') return "011";
    else if (oct_digit == '4') return "100";
    else if (oct_digit == '5') return "101";
    else if (oct_digit == '6') return "110";
    else if (oct_digit == '7') return "111";
    else                       return "";
}

//Converts a single hexadecimal digit to its 4-bit binary string using if-else.

const char* hex_digit_to_bin_map(char hex_digit) {
    char upper_hex = toupper(hex_digit);

    if      (upper_hex == '0') return "0000";
    else if (upper_hex == '1') return "0001";
    else if (upper_hex == '2') return "0010";
    else if (upper_hex == '3') return "0011";
    else if (upper_hex == '4') return "0100";
    else if (upper_hex == '5') return "0101";
    else if (upper_hex == '6') return "0110";
    else if (upper_hex == '7') return "0111";
    else if (upper_hex == '8') return "1000";
    else if (upper_hex == '9') return "1001";
    else if (upper_hex == 'A') return "1010";
    else if (upper_hex == 'B') return "1011";
    else if (upper_hex == 'C') return "1100";
    else if (upper_hex == 'D') return "1101";
    else if (upper_hex == 'E') return "1110";
    else if (upper_hex == 'F') return "1111";
    else                       return "";
}

//Converts a 4-bit binary string to its single hexadecimal character.

char bin_four_bits_to_hex_map(const char *four_bits) {
    long value = strtol(four_bits, NULL, 2);

    if (value < 10) return value + '0';
    else            return value - 10 + 'A';
}

//Function 1: oct_to_bin

void oct_to_bin(const char *oct, char *out) {
    out[0] = '\0';

    for (int i = 0; oct[i] != '\0'; i++) {
        const char *bin_map = oct_digit_to_bin_map(oct[i]);
        if (bin_map[0] == '\0') {
            fprintf(stderr, "Error: Invalid octal digit '%c'.\n", oct[i]);
            out[0] = '\0';
            return;
        }
        strcat(out, bin_map);
    }
}

//Function 2: oct_to_hex

void oct_to_hex(const char *oct, char *out) {
    int oct_len = strlen(oct);
    int max_bin_len = 3 * oct_len;
    char *bin_temp = (char*)malloc(max_bin_len + 1);

    if (bin_temp == NULL) return;

    // 1. Octal -> Binary Conversion

    oct_to_bin(oct, bin_temp);

    if (bin_temp[0] == '\0' && oct_len > 0) {
        free(bin_temp);
        out[0] = '\0';
        return;
    }

    // 2. Padding

    int bin_len = strlen(bin_temp);
    int extra_bits = bin_len % 4;
    int leading_zeros = (extra_bits == 0) ? 0 : (4 - extra_bits);

    int padded_bin_len = bin_len + leading_zeros;
    char *padded_bin = (char*)malloc(padded_bin_len + 1);

    if (padded_bin == NULL) { free(bin_temp); return; }

    for (int i = 0; i < leading_zeros; i++) {
        padded_bin[i] = '0';
    }
    strcpy(padded_bin + leading_zeros, bin_temp);
    padded_bin[padded_bin_len] = '\0';

    // 3. Group by 4 and Convert to Hex

    out[0] = '\0';
    int hex_index = 0;
    char four_bits[5];

    for (int i = 0; i < padded_bin_len; i += 4) {
        strncpy(four_bits, padded_bin + i, 4);
        four_bits[4] = '\0';

        out[hex_index++] = bin_four_bits_to_hex_map(four_bits);
    }
    out[hex_index] = '\0';

    free(bin_temp);
    free(padded_bin);
}

//Function 3: hex_to_bin

void hex_to_bin(const char *hex, char *out) {
    out[0] = '\0';

    for (int i = 0; hex[i] != '\0'; i++) {
        const char *bin_map = hex_digit_to_bin_map(hex[i]);

        if (bin_map[0] == '\0') {
            fprintf(stderr, "Error: Invalid hex digit '%c'.\n", hex[i]);
            out[0] = '\0';
            return;
        }
        strcat(out, bin_map);
    }
}

//Function 4: to_sign_magnitude

void to_sign_magnitude(int32_t n, char *out) {
    if (n >= 0) {
        to_32bit_binary_string((uint32_t)n, out);
    } else {
        uint32_t magnitude = (uint32_t)abs(n);
        to_32bit_binary_string(magnitude, out);
        // Set the sign bit (MSB, index 0) to 1
        out[0] = '1';
    }
}

//Function 5: to_ones_complement

void to_ones_complement(int32_t n, char *out) {
    if (n >= 0) {
        to_32bit_binary_string((uint32_t)n, out);
    } else {
        to_32bit_binary_string((uint32_t)abs(n), out);

        // Flip ALL bits
        for (int i = 0; i < 32; i++) {
            out[i] = (out[i] == '0') ? '1' : '0';
        }
    }
}

//Function 6: to_twos_complement

void to_twos_complement(int32_t n, char *out) {
    // Cast the signed number to its unsigned equivalent to get the correct bit pattern.
    uint32_t bit_pattern = (uint32_t)n;
    to_32bit_binary_string(bit_pattern, out);
}

// Simplified structure to hold one line of a test case
typedef struct {
    char func_name[50];
    char input1[100];
    char input2[100];
    char expected[100];
} TestCase;

// Array of the provided test cases (using string literals for simplicity)
const char *test_lines[] = {
    "oct_to_bin 725 111010101",
    "oct_to_bin 345 011100101",
    "oct_to_bin 777 111111111",
    "oct_to_bin 0 000",
    "oct_to_bin 1 001",
    "oct_to_bin 7 111",
    "oct_to_bin 10 001000",
    "oct_to_bin 100 001000000",
    "oct_to_bin 377 011111111",
    "oct_to_hex 345 E5",
    "oct_to_hex 777 1FF",
    "oct_to_hex 100 40",
    "oct_to_hex 0 0",
    "oct_to_hex 1 1",
    "oct_to_hex 7 7",
    "oct_to_hex 1234 29C",
    "oct_to_hex 7654 FAC",
    "hex_to_bin A3 10100011",
    "hex_to_bin FF 11111111",
    "hex_to_bin 1A 00011010",
    "hex_to_bin a3 10100011",
    "hex_to_bin Ff 11111111",
    "hex_to_bin aC 10101100",
    "hex_to_bin 0 0000",
    "hex_to_bin 1 0001",
    "hex_to_bin F 1111",
    "hex_to_bin CAFE 1100101011111110",
    "hex_to_bin 1234 0001001000110100",
    "# ====================== SIGNED REPRESENTATION TESTS ======================",
    "to_sign_magnitude 5 00000000000000000000000000000101",
    "to_sign_magnitude 1 00000000000000000000000000000001",
    "to_sign_magnitude 255 00000000000000000000000011111111",
    "to_sign_magnitude -5 10000000000000000000000000000101",
    "to_sign_magnitude -1 10000000000000000000000000000001",
    "to_sign_magnitude -255 10000000000000000000000011111111",
    "to_sign_magnitude 0 00000000000000000000000000000000",
    "to_ones_complement 5 00000000000000000000000000000101",
    "to_ones_complement 1 00000000000000000000000000000001",
    "to_ones_complement 255 00000000000000000000000011111111",
    "to_ones_complement -5 11111111111111111111111111111010",
    "to_ones_complement -1 11111111111111111111111111111110",
    "to_ones_complement -255 11111111111111111111111100000000",
    "to_ones_complement 0 00000000000000000000000000000000",
    "to_twos_complement 5 00000000000000000000000000000101",
    "to_twos_complement 1 00000000000000000000000000000001",
    "to_twos_complement 255 00000000000000000000000011111111",
    "to_twos_complement -5 11111111111111111111111111111011",
    "to_twos_complement -1 11111111111111111111111111111111",
    "to_twos_complement -255 11111111111111111111111100000001",
    "to_twos_complement 0 00000000000000000000000000000000",
    "to_twos_complement 2147483647 01111111111111111111111111111111",
    "to_twos_complement -2147483648 10000000000000000000000000000000"
};

void run_tests() {
    char actual_output[100];
    int passed_count = 0;
    int total_tests = 0;

    for (size_t k = 0; k < sizeof(test_lines) / sizeof(test_lines[0]); ++k) {
        char line[100];
        strcpy(line, test_lines[k]);

        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\0') {
            if (line[0] == '#') printf("\n%s\n", line);
            continue;
        }

        TestCase test;
        // Use scanf to parse the space-separated fields
        if (sscanf(line, "%s %s %s", test.func_name, test.input1, test.expected) < 3) {
            fprintf(stderr, "Error parsing test line: %s\n", line);
            continue;
        }

        total_tests++;
        int is_pass = 0;

        // Determine which function to call
        if (strcmp(test.func_name, "oct_to_bin") == 0) {
            oct_to_bin(test.input1, actual_output);
        } else if (strcmp(test.func_name, "oct_to_hex") == 0) {
            oct_to_hex(test.input1, actual_output);
        } else if (strcmp(test.func_name, "hex_to_bin") == 0) {
            hex_to_bin(test.input1, actual_output);
        } else if (strcmp(test.func_name, "to_sign_magnitude") == 0) {
            int32_t n = (int32_t)strtol(test.input1, NULL, 10);
            to_sign_magnitude(n, actual_output);
        } else if (strcmp(test.func_name, "to_ones_complement") == 0) {
            int32_t n = (int32_t)strtol(test.input1, NULL, 10);
            to_ones_complement(n, actual_output);
        } else if (strcmp(test.func_name, "to_twos_complement") == 0) {
            // Note: Use long long for the very large min/max values
            long long n_ll = strtoll(test.input1, NULL, 10);
            to_twos_complement((int32_t)n_ll, actual_output);
        } else {
            fprintf(stderr, "Unknown function: %s\n", test.func_name);
            continue;
        }

        // Check result
        if (strcmp(actual_output, test.expected) == 0) {
            is_pass = 1;
            passed_count++;
        }

        printf("Test %d: %s(%s) -> Expected: %s, Got: %s [%s]\n",
               total_tests, test.func_name, test.input1, test.expected, actual_output,
               is_pass ? "PASS" : "FAIL");
    }

    printf("Summary: %d/%d tests passed.\n", passed_count, total_tests);
}

int main(void) {
    run_tests();
    return 0;
}