#include "config.h"
#include "model.h"



void setStartValues(ModelInstance *comp) {
    M(A_rows) = 3; 
    M(A_cols) = 3; 
    M(B_rows) = 3; 
    M(B_cols) = 3; 

    // identity matrix if square
    for (int i = 0; i < A_MAX_ROWS; i++) {
        for (int j = 0; j < A_MAX_COLS; j++) {
            M(A)[i][j] = i == j ? 1 : 0;
    }}

}


Status calculateValues(ModelInstance *comp) {
    for (size_t i = 0; i < M(A_rows); i++) {
        for (size_t j = 0; j < M(A_cols); j++) {
            M(B)[i][j] = M(A)[i][j];
        }
    }
    return OK;
}

Status getFloat64(ModelInstance* comp, ValueReference vr, double values[], size_t nValues, size_t* index) {
    calculateValues(comp); // Update the output 'B' before getting values

    switch (vr) {
        case vr_time:
            ASSERT_NVALUES(1);
            values[(*index)++] = comp->time;
            return OK;
        case vr_A:
            ASSERT_NVALUES(M(A_rows) * M(A_cols));
            for (size_t i = 0; i < M(A_rows); i++) {
                for (size_t j = 0; j < M(A_cols); j++) {
                    values[(*index)++] = M(A)[i][j];
                }
            }
            return OK;
        case vr_B:
            ASSERT_NVALUES(M(B_rows) * M(B_cols));
            for (size_t i = 0; i < M(B_rows); i++) {
                for (size_t j = 0; j < M(B_cols); j++) {
                    values[(*index)++] = M(B)[i][j];
                }
            }
            return OK;
        default:
            logError(comp, "Get Float64 is not allowed for value reference %u.", vr);
            return Error;
    }
}

Status setFloat64(ModelInstance* comp, ValueReference vr, const double values[], size_t nValues, size_t* index) {
    switch (vr) {
        case vr_A:
            ASSERT_NVALUES(M(A_rows) * M(A_cols));
            for (size_t i = 0; i < M(A_rows); i++) {
                for (size_t j = 0; j < M(A_cols); j++) {
                    M(A)[i][j] = values[(*index)++];
                }
            }
            calculateValues(comp); // Update the output 'B' after setting 'A'
            return OK;
        default:
            logError(comp, "Set Float64 is not allowed for value reference %u.", vr);
            return Error;
    }
}


void eventUpdate(ModelInstance *comp) {
    comp->valuesOfContinuousStatesChanged   = false;
    comp->nominalsOfContinuousStatesChanged = false;
    comp->terminateSimulation               = false;
    comp->nextEventTimeDefined              = false;
}
