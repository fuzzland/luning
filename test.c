#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a complex struct
typedef struct {
    int id;
    char name[50];
    double values[5];
} ComplexData;

// Function to initialize a ComplexData struct
ComplexData* initialize_complex_data(int id, const char* name, double values[], int size) {
    ComplexData* data = (ComplexData*)malloc(sizeof(ComplexData));
    if (data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    data->id = id;
    strncpy(data->name, name, sizeof(data->name) - 1);
    data->name[sizeof(data->name) - 1] = '\0'; // Ensure null-termination

    for (int i = 0; i < size && i < 5; i++) {
        data->values[i] = values[i];
    }

    return data;
}

// Function to modify a ComplexData struct
ComplexData* modify_complex_data(ComplexData* data, int new_id, const char* new_name) {
    if (data == NULL) {
        return NULL;
    }

    data->id = new_id;
    strncpy(data->name, new_name, sizeof(data->name) - 1);
    data->name[sizeof(data->name) - 1] = '\0'; // Ensure null-termination

    return data;
}

int main() {
    double values[] = {1.1, 2.2, 3.3, 4.4, 5.5};

    // Initialize the complex data
    ComplexData* data = initialize_complex_data(1, "InitialData", values, 5);
    if (data == NULL) {
        return 1;
    }

    // Print the initial data
    printf("Initial data: ID=%d, Name=%s, Values=[", data->id, data->name);
    for (int i = 0; i < 5; i++) {
        printf("%f", data->values[i]);
        if (i < 4) {
            printf(", ");
        }
    }
    printf("]\n");

    // Modify the complex data
    ComplexData* modified_data = modify_complex_data(data, 2, "ModifiedData");
    if (modified_data == NULL) {
        free(data);
        return 1;
    }

    // Print the modified data
    printf("Modified data: ID=%d, Name=%s, Values=[", modified_data->id, modified_data->name);
    for (int i = 0; i < 5; i++) {
        printf("%f", modified_data->values[i]);
        if (i < 4) {
            printf(", ");
        }
    }
    printf("]\n");

    // Free the allocated memory
    free(modified_data);

    return 0;
}
