#ifndef config_h
#define config_h

#include <stddef.h>  // for size_t
#include <stdbool.h> // for bool
#include <inttypes.h>

// define class name and unique id
#define MODEL_IDENTIFIER Feedthrough
#define INSTANTIATION_TOKEN "{37B954F1-CC86-4D8F-B97F-C7C36F6670D2}"

#define CO_SIMULATION
#define MODEL_EXCHANGE

#define GET_FLOAT64
#define SET_FLOAT64

#define EVENT_UPDATE

#define FIXED_SOLVER_STEP 1
#define DEFAULT_STOP_TIME 10

#define A_MAX_ROWS 5
#define A_MAX_COLS 5

#define FLOAT_ARRAY_MAX_LEN 128

typedef enum {
    vr_time,
    vr_A,  
    vr_B, 
} ValueReference;

typedef struct {
    double A[A_MAX_ROWS][A_MAX_COLS];
    double B[A_MAX_ROWS][A_MAX_COLS];
    size_t A_rows;
    size_t A_cols;
    size_t B_rows;
    size_t B_cols;
    
} ModelData;

#endif /* config_h */
