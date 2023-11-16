#include "config.h"
#include "model.h"


void setStartValues(ModelInstance *comp) {
    M(m) = 3;
    M(n) = 3;
    M(u_m) = 3;
    M(u_n) = 3;

    // Initialize A as a 2-diagonal matrix
    for (int i = 0; i < M_MAX; i++) {
        for (int j = 0; j < N_MAX; j++) {
            M(A)[i][j] = i == j ? 2 : 0;
        }
    }

    // Initialize u as identity matrix (will be input of other FMU)
    for (int i = 0; i < U_MAX_ROWS; i++) {
        for (int j = 0; j < U_MAX_COLS; j++) {
            M(u)[i][j] = i == j ? 1 : 0; 
        }
    }

    for (int i = 0; i < Y_MAX_ROWS; i++) {
        for (int j = 0; j < Y_MAX_COLS; j++) {
            M(y)[i][j] = 0; 
        }
    }
}

Status calculateValues(ModelInstance *comp) {
    // y = A * u (matrix multiplication)
    for (size_t i = 0; i < M(m); i++) {
        for (size_t j = 0; j < M(n); j++) {
            M(y)[i][j] = 0;
            for (size_t k = 0; k < M(n); k++) {
                M(y)[i][j] += M(A)[i][k] * M(u)[k][j];
            }
        }
    }
    return OK;
}

Status getFloat64(ModelInstance* comp, ValueReference vr, double values[], size_t nValues, size_t* index) {

    calculateValues(comp);

    switch (vr) {
        case vr_time:
            ASSERT_NVALUES(1);
            values[(*index)++] = comp->time;
            return OK;
        case vr_u:
            ASSERT_NVALUES(M(u_m) * M(u_n));
            for (size_t i = 0; i < M(u_m); i++) {
                for (size_t j = 0; j < M(u_n); j++) {
                    values[(*index)++] = M(u)[i][j];
                }
            }
            return OK;
        case vr_A:
            ASSERT_NVALUES(M(m) * M(n));
            for (size_t i = 0; i < M(m); i++)
            for (size_t j = 0; j < M(n); j++) {
                values[(*index)++] = M(A)[i][j];
            }
            return OK;
        case vr_y:
            ASSERT_NVALUES(M(m) * M(n));  // Adjust for matrix size
            for (size_t i = 0; i < M(m); i++) {
                for (size_t j = 0; j < M(n); j++) {
                    values[(*index)++] = M(y)[i][j];
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
            case vr_u:
        ASSERT_NVALUES(M(u_m) * M(u_n));
        for (size_t i = 0; i < M(u_m); i++) {
            for (size_t j = 0; j < M(u_n); j++) {
                M(u)[i][j] = values[(*index)++];
            }
        }
        calculateValues(comp);
        return OK;
        case vr_A:
            ASSERT_NVALUES(M(m) * M(n));
            for (size_t i = 0; i < M(m); i++)
            for (size_t j = 0; j < M(n); j++) {
                M(A)[i][j] = values[(*index)++];
            }
            return OK;
        default:
            logError(comp, "Set Float64 is not allowed for value reference %u.", vr);
            return Error;
    }
}

Status getUInt64(ModelInstance* comp, ValueReference vr, uint64_t values[], size_t nValues, size_t* index) {

    ASSERT_NVALUES(1);

    calculateValues(comp);

    switch (vr) {
        case vr_m:
            values[(*index)++] = M(m);
            return OK;
        case vr_n:
            values[(*index)++] = M(n);
            return OK;
        default:
            logError(comp, "Get UInt64 is not allowed for value reference %u.", vr);
            return Error;
    }
}

Status setUInt64(ModelInstance* comp, ValueReference vr, const uint64_t values[], size_t nValues, size_t* index) {

    ASSERT_NVALUES(1);

    if (comp->state != ConfigurationMode && comp->state != ReconfigurationMode) {
        logError(comp, "Structural variables can only be set in Configuration Mode or Reconfiguration Mode.");
        return Error;
    }

    const uint64_t v = values[(*index)++];

    switch (vr) {
        case vr_m:
            if (v < 1 || v > M_MAX) return Error;
            M(m) = v;
            return OK;
        case vr_n:
            if (v < 1 || v > N_MAX) return Error;
            M(n) = v;
            return OK;
        default:
            logError(comp, "Set UInt64 is not allowed for value reference %u.", vr);
            return Error;
    }
}

void eventUpdate(ModelInstance *comp) {
    comp->valuesOfContinuousStatesChanged   = false;
    comp->nominalsOfContinuousStatesChanged = false;
    comp->terminateSimulation               = false;
    comp->nextEventTimeDefined              = false;
}
