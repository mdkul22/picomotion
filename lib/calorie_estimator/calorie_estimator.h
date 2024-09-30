#pragma once 

#include <stdio.h>
#include <stdbool.h>
#include "pico/time.h"
#include "qmi8658.h"

#define CC_QUEUE_SIZE 100

typedef bool(*ce_alg_callback)(repeating_timer_t*);

typedef struct _ce_alg_state_t_ {
    double calories;
    imu_pkt_t accel[CC_QUEUE_SIZE];
    imu_pkt_t gyro[CC_QUEUE_SIZE];
    double lpf_accel;
    double lpf_gyro;
    ce_alg_callback cb;
} ce_alg_state_t;

bool update_arc_calories_cb(struct repeating_timer* t);
void initialize_calorie_estimator(ce_alg_state_t* alg_state);
double get_calorie_estimate();
void reset_calorie_estimate();
void stop_calorie_estimator();
void push_imu_data(imu_pkt_t*, imu_pkt_t*);

