#ifndef config_h
#define config_h

#include <stdint.h>

// define class name and unique id
#define MODEL_IDENTIFIER LinearTransform
#define INSTANTIATION_TOKEN "{D773325B-AB94-4630-BF85-643EB24FCB78}"

#define CO_SIMULATION
#define MODEL_EXCHANGE

#define SET_FLOAT64
#define GET_UINT64
#define SET_UINT64
#define EVENT_UPDATE

#define FIXED_SOLVER_STEP 1
#define DEFAULT_STOP_TIME 10

#define M_MAX 5
#define N_MAX 5

#define U_MAX_ROWS 5
#define U_MAX_COLS 5 

#define Y_MAX_ROWS M_MAX
#define Y_MAX_COLS U_MAX_COLS


typedef enum {
    vr_time,
    vr_m,
    vr_n,
    vr_u,
    vr_A,
    vr_y
} ValueReference;

typedef struct {
    uint64_t m;
    uint64_t n;
    uint64_t u_m;
    uint64_t u_n;
    double u[U_MAX_ROWS][U_MAX_COLS];
    double A[M_MAX][N_MAX];
    double y[Y_MAX_ROWS][Y_MAX_COLS];
} ModelData;

#endif /* config_h */
