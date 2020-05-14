/*
 * BMI.c
 *
 *  Created on: 13 May 2020
 *      Author: Beheerder
 */
#include "Header/BMI.h"



void cycledelay(uint32_t cycles)
{
	volatile uint32_t i;

	for(i = 0UL; i < cycles ;++i)
	{
		__NOP();
	}
}

void bmi055_apply_gyro_angle(float* gyro_a, double* gyro_intergral_a, double delta){
	//*gyro_intergral_a += (* gyro_a)*delta;
	*gyro_intergral_a += (* gyro_a)*delta;
}


void bmi055_apply_gyro_all_angles(){
	bmi055_apply_gyro_angle(&gyro_y, &complement_xValue, -0.004);
	bmi055_apply_gyro_angle(&gyro_x, &complement_yValue, 0.004);
	bmi055_apply_gyro_angle(&gyro_y, &complement_zValue, -0.004);

}
//

void bmi055_apply_accel_all_angles(){
		num_of_samples++;
		//you can if needed use the following commented if statement to accelerometer data
		//double filterValue = fabs(bmi055_mat.acc_x)+fabs(bmi055_mat.acc_y)+fabs(bmi055_mat.acc_z);
		//if(filterValue > 0.1 &&  filterValue < 1.9 )
		//{
			//add angle code here...
		//}

		double accel_angle_div_x = sqrt((acc_y)*(acc_y)+ (acc_z)*(acc_z));
		// this if statement prevent division by zero.
		if(accel_angle_div_x > 0.0){
			double accel_angle_x = atan((acc_x)/ accel_angle_div_x);
			complement_xValue = ((complement_xValue*0.8)+ (accel_angle_x*0.2));
		}
		else{
			complement_xValue = (complement_xValue*0.8);
		}

		//first atan2 was used but atan is faster. You can always revert this code
		//double accel_angle_x = atan2((bmi055_mat.acc_x), (bmi055_mat.acc_y)*(bmi055_mat.acc_y)+ (bmi055_mat.acc_z)*(bmi055_mat.acc_z));

		double accel_angle_div_y = sqrt((acc_z)*(acc_z)+ (acc_x)*(acc_x));
		if(accel_angle_div_y > 0.0){
			double accel_angle_y = atan((acc_y)/accel_angle_div_y);
			complement_yValue = ((complement_yValue*0.8)+ (accel_angle_y*0.2));
		}
		else{
			complement_yValue = (complement_yValue*0.8);
		}

		//first atan2 was used but atan is faster. You can always revert this code
		//double accel_angle =  atan2(sqrt(acc_u*acc_u + acc_v*acc_v), acc_w);

		double accel_angle_div_z = sqrt((acc_x)*(acc_x) + (acc_y)*(acc_y));
		if(accel_angle_div_z > 0.0){
		double accel_angle_z  =  atan((acc_z)/accel_angle_div_z);
			complement_zValue = ((complement_zValue*0.8)+ (accel_angle_z*0.2));
		}
		else{

			complement_zValue = -(complement_zValue*0.8);
		}

//		angle_vec.x = intergrated_complement_x;
//		angle_vec.y = intergrated_complement_y;
//		angle_vec.z = intergrated_complement_z;

}


void bmi055_start(void)
{
		SPI_MASTER_SetBaudRate(&SPI_MASTER_0, SPI_MASTER_0.config->channel_config->baudrate);
		XMC_GPIO_SetMode(BMI_CSA_PORT, BMI_CSA_PIN, BMI_CSA_MODE);
		XMC_GPIO_SetMode(BMI_CSG_PORT, BMI_CSG_PIN, BMI_CSG_MODE);

		cycledelay((uint32_t)(145 * 2));

		XMC_GPIO_SetOutputHigh(BMI_CSA_PORT, BMI_CSA_PIN);
		XMC_GPIO_SetOutputHigh(BMI_CSG_PORT, BMI_CSG_PIN);

		spi_flushrxbuffer(&SPI_MASTER_0);

		memset(acceleration_register, 0, 4);
		memset(gyro_register, 0, 4);
}


void bmi055_poll_data(){


		uint16_t opcode = BMI_DATAREG_START;

		//poll data from accelerometer
		spi_flushrxbuffer(&SPI_MASTER_0);
		XMC_GPIO_SetOutputLow(BMI_CSA_PORT, BMI_CSA_PIN);
		SPI_MASTER_Transfer(&SPI_MASTER_0, (uint8_t *)&opcode, (uint8_t*)acceleration_register, 4);
		XMC_GPIO_SetOutputHigh(BMI_CSA_PORT, BMI_CSA_PIN);

		//poll data from gyroscope
		spi_flushrxbuffer(&SPI_MASTER_0);
		XMC_GPIO_SetOutputLow(BMI_CSG_PORT, BMI_CSG_PIN);
		SPI_MASTER_Transfer(&SPI_MASTER_0, (uint8_t *)&opcode, (uint8_t*)gyro_register, 4);
		XMC_GPIO_SetOutputHigh(BMI_CSG_PORT, BMI_CSG_PIN);


		acceleration_register_copy = (uint8_t*)acceleration_register;
		gyro_register_copy = (uint8_t*)gyro_register;

		//merge accelerometer data NOTE: data may not be merged as expected because the first byte in the array is a dummy byte.
		int16_t acc_xValue = (((int16_t)(acceleration_register_copy[3] << 8) | acceleration_register_copy[0]) >> 4);
		int16_t acc_yValue = (((int16_t)(acceleration_register_copy[5] << 8) | acceleration_register_copy[2]) >> 4);
		int16_t acc_zValue = (((int16_t)(acceleration_register_copy[7] << 8) | acceleration_register_copy[4]) >> 4);


		acc_x = -(acc_xValue*(2.0f / 2048.0f)) * 0.98;
		acc_y = (acc_yValue*(2.0f / 2048.0f)) * 0.98;
		acc_z = -(acc_zValue*(2.0f / 2048.0f)) * 0.98;

		acc_x_ms2 = acc_x*GRAVITY_MS;
		acc_y_ms2 = acc_y*GRAVITY_MS;
		acc_z_ms2 = acc_z*GRAVITY_MS;

		ms_xValue = acc_x_ms2;
		ms_yValue = acc_y_ms2;
		ms_zValue = acc_z_ms2;


		//merge gyroscope data NOTE: data may not be merged as expected because the first byte in the array is a dummy byte.
		gyro_xValue = (((int16_t)(gyro_register_copy[3] << 8) | gyro_register_copy[0]));
		gyro_yValue = (((int16_t)(gyro_register_copy[5] << 8) | gyro_register_copy[2]));
		gyro_zValue = (((int16_t)(gyro_register_copy[7] << 8) | gyro_register_copy[4]));



		gyro_x = -(gyro_xValue* (2000.0f / 32767.0f)) * (M_PI/180.0f);
		gyro_y = (gyro_yValue* (2000.0f / 32767.0f)) * (M_PI/180.0f);
		gyro_z = -(gyro_zValue* (2000.0f / 32767.0f)) * (M_PI/180.0f);

//		bmi055_mat.gyro_x = (gyro_xValue* (2000.0f / 32767.0f)) * (M_PI/180.0f);
//		bmi055_mat.gyro_y = (gyro_yValue* (2000.0f / 32767.0f)) * (M_PI/180.0f);
//		bmi055_mat.gyro_z = (gyro_zValue* (2000.0f / 32767.0f)) * (M_PI/180.0f);
//
//		bmi055_mat.acc_x = (acc_xValue*(2.0f / 2048.0f)) * 0.98;
//		bmi055_mat.acc_y = (acc_yValue*(2.0f / 2048.0f)) * 0.98;
//		bmi055_mat.acc_z = (acc_zValue*(2.0f / 2048.0f)) * 0.98;

}



void spi_flushrxbuffer(const SPI_MASTER_t *const handle)
{
	/* Clear the receive FIFO */
	XMC_USIC_CH_RXFIFO_Flush(handle->channel);
	while(!SPI_MASTER_IsRxFIFOEmpty(handle)) { SPI_MASTER_GetReceivedWord(handle); };
}
