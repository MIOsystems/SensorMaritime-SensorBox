/*
 * BMI.h
 *
 *  Created on: 13 May 2020
 *      Author: Beheerder
 */

#ifndef HEADER_BMI_H_
#define HEADER_BMI_H_
#include <Header/UDP.h>
#include <DAVE.h>
#include <math.h>

#define  GRAVITY_MS 9.80665

uint16_t acceleration_register[4];
uint16_t gyro_register[4];

uint8_t* acceleration_register_copy;
uint8_t* gyro_register_copy;



#define BMI_CSA_PORT XMC_GPIO_PORT2
#define BMI_CSA_MODE XMC_GPIO_MODE_OUTPUT_PUSH_PULL
#define BMI_CSA_PIN 6

#define BMI_CSG_PORT XMC_GPIO_PORT5
#define BMI_CSG_MODE XMC_GPIO_MODE_OUTPUT_PUSH_PULL
#define BMI_CSG_PIN 6

#define INTEGRAL_DT 0.05
#define BMI_DATAREG_START 0x8200







//int16_t max_gyro_xValue = 0;
//int16_t max_gyro_yValue = 0;
//int16_t max_gyro_zValue = 0;
//
//int16_t min_gyro_xValue = 0;
//int16_t min_gyro_yValue = 0;
//int16_t min_gyro_zValue = 0;
//
//int16_t avg_gyro_xValue = 0;
//int16_t avg_gyro_yValue = 0;
//int16_t avg_gyro_zValue = 0;


int num_of_samples;// = 0;

double max_complement_xValue;// = 0;
double max_complement_yValue;// = 0;
double max_complement_zValue;// = M_PI/2;

double min_complement_xValue;// = 0;
double min_complement_yValue;// = 0;
double min_complement_zValue;// = M_PI/2;

double avg_complement_xValue;// = 0;
double avg_complement_yValue;// = 0;
double avg_complement_zValue;// = M_PI/2;

double complement_xValue;// = 0;
double complement_yValue;// = 0;
double complement_zValue;// = M_PI/2;



double max_ms_xValue;
double max_ms_yValue;
double max_ms_zValue;

double min_ms_xValue;
double min_ms_yValue;
double min_ms_zValue;

double avg_ms_xValue;
double avg_ms_yValue;
double avg_ms_zValue;

double ms_xValue;
double ms_yValue;
double ms_zValue;




float gyro_x, gyro_y, gyro_z;
float acc_x, acc_y, acc_z;
float acc_x_ms2, acc_y_ms2, acc_z_ms2;

int16_t gyro_xValue;// = 0;
int16_t gyro_yValue;// = 0;
int16_t gyro_zValue;// = 0;

void cycledelay(uint32_t cycles);

void bmi055_apply_gyro_angle(float* gyro_a, double* gyro_intergral_a, double delta);

void bmi055_apply_gyro_all_angles();

void bmi055_apply_accel_all_angles();

void bmi055_start(void);

void bmi055_poll_data();

void spi_flushrxbuffer(const SPI_MASTER_t *const handle);

#endif /* HEADER_BMI_H_ */
