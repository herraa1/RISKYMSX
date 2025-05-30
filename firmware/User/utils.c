#include "utils.h"
#pragma GCC push_options
#pragma GCC optimize("Ofast")

void initBuffer (CircularBuffer *cb) {
    cb->buffer = (uint32_t *)malloc (BUFFER_SIZE * sizeof (uint32_t));
    cb->head = 0;
    cb->tail = 0;
}

void initMiniBuffer (CircularBuffer *cb) {
    cb->buffer = (uint32_t *)malloc (BUFFER_MINI_SIZE * sizeof (uint32_t));
    cb->head = 0;
    cb->tail = 0;
}

void deinitBuffer (CircularBuffer *cb) {
    if (cb->buffer != NULL) {
        free (cb->buffer);  // Free allocated memory
        cb->buffer = NULL;  // Avoid dangling pointer
    }
    cb->head = 0;
    cb->tail = 0;
}

int isFull (CircularBuffer *cb) {
    uint32_t next = (cb->head + 1) & (BUFFER_SIZE - 1);  // Compute the next position using bitwise AND
    if (next == cb->tail) {
        return 1;
    }
    return 0;
}

int append (CircularBuffer *cb, uint32_t item) {
    uint32_t next = (cb->head + 1) & (BUFFER_SIZE - 1);  // Compute the next position using bitwise AND
    while (next == cb->tail) { };
    cb->buffer[cb->head] = item;
    cb->head = next;
    return 0;  // Success
}

int pop (CircularBuffer *cb, uint32_t *item) {
    if (cb->head == cb->tail) {
        return -1;  // Buffer is empty
    }
    *item = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) & (BUFFER_SIZE - 1);  // Update tail using bitwise AND
    return 0;                                       // Success
}

int popmini (CircularBuffer *cb, uint32_t *item) {
    if (cb->head == cb->tail) {
        return -1;  // Buffer is empty
    }
    *item = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) & (BUFFER_MINI_SIZE - 1);  // Update tail using bitwise AND
    return 0;                                            // Success
}

void appendString (CircularBuffer *cb, const char *inputString) {
    for (int i = 0; i < strlen (inputString); i++) {
        append (cb, inputString[i]);
    }
}

void intToString (int num, char *str) {
    int i = 0;
    int isNegative = 0;

    // Handle negative numbers
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    // Process each digit of the number
    do {
        str[i++] = (num % 10) + '0';
        num /= 10;
    } while (num > 0);

    // Add negative sign if the number is negative
    if (isNegative) {
        str[i++] = '-';
    }

    // Null-terminate the string
    str[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

int strToInt (const char *str) {
    int result = 0;
    int sign = 1;
    int i = 0;

    // Handle optional leading whitespace
    while (str[i] == ' ') {
        i++;
    }

    // Handle optional sign
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    // Convert characters to integer
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    // Apply sign
    result *= sign;

    return result;
}

void handle_path (char *str) {
    char *last_slash = strrchr (str, '/');
    if (last_slash != NULL) {
        if (strcmp (last_slash, "/.") == 0) {
            *last_slash = '\0';  // Remove the /. section by terminating the string at the last '/'
        } else if (strcmp (last_slash, "/..") == 0) {
            *last_slash = '\0';  // Temporarily terminate the string at the last '/'
            char *second_last_slash = strrchr (str, '/');
            if (second_last_slash != NULL) {
                *second_last_slash = '\0';  // Remove the previous section by terminating the string at the second-to-last '/'
            }
        }
    }
}

#pragma GCC pop_options