#include "calorie_estimator.h"
#include "qmi8658.h"
#include "math.h"

// this should eventually become a struct tracking stuff
static double calories = 0.f;

void initialize_calorie_estimator(ce_alg_state_t* alg_state);

double get_calorie_estimate() {

}

void reset_calorie_estimate() {

}

void stop_calorie_estimator() {

}

void push_imu_data(imu_pkt_t* accl, imu_pkt_t* gyro) {
  accl->x *= 9.81;
  accl->y *= 9.81;
  accl->z *= 9.81;
  accl->x *= accl->x;
  accl->y *= accl->y;
  accl->z *= accl->z;
  double amag = sqrt(accl->x + accl->y + accl->z)-9.81;
  double met = 1 + amag / 10.f;
  double calories_in_10ms = met * 80 * (0.010 / 3600) * 1.05;
  calories += 100*calories_in_10ms;
  if (calories > 4000)
    calories = 0.f;
  gprintf(DEBUG, "calories: %.3lf\r", calories);
}

