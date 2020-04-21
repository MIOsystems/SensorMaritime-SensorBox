/*
 * main.c
 *
 *  Created on: 2020 Mar 20 15:00:36
 *  Author: Beheerder
 */


#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))


#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)
#include <math.h>




/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */

#define ip1 192							/** Firstbyte of the IP address **/
#define ip2 168							/** Second byte of the IP address **/
#define ip3 0							/** Third byte of the IP address **/
#define ip4 9

#define PORT_COM_OUT    52256			/** The port at which the receiver receives the UDP packets **/
#define PORT_COM_IN     62509			/** The port at which the chip transmits the UDP packets **/

#define BMI_CSA_PORT XMC_GPIO_PORT2
#define BMI_CSA_MODE XMC_GPIO_MODE_OUTPUT_PUSH_PULL
#define BMI_CSA_PIN 6

#define BMI_CSG_PORT XMC_GPIO_PORT5
#define BMI_CSG_MODE XMC_GPIO_MODE_OUTPUT_PUSH_PULL
#define BMI_CSG_PIN 6

#define INTEGRAL_DT 0.05
#define BMI_DATAREG_START 0x8200


uint16_t acceleration_register[4];
uint16_t gyro_register[4];

uint8_t* acceleration_register_copy;
uint8_t* gyro_register_copy;


struct udp_pcb*  com_pcb;
ip_addr_t returnaddr;




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


int num_of_samples = 0;

double max_complement_xValue = 0;
double max_complement_yValue = 0;
double max_complement_zValue = M_PI/2;

double min_complement_xValue = 0;
double min_complement_yValue = 0;
double min_complement_zValue = M_PI/2;

double avg_complement_xValue = 0;
double avg_complement_yValue = 0;
double avg_complement_zValue = M_PI/2;

double complement_xValue = 0;
double complement_yValue = 0;
double complement_zValue = M_PI/2;


//typedef struct gyro_gps_data {
//	float min_x;
//	float min_y;
//	float min_z;
//	float max_x;
//	float max_y;
//	float max_z;
//	float avg_x;
//	float avg_y;
//	float avg_z;
//
//
//} gyro_gps_data;


//gyro_gps_data v;



typedef struct angle_data {
	double avg_x;
	double avg_y;
	double avg_z;
	double min_x;
	double min_y;
	double min_z;
	double max_x;
	double max_y;
	double max_z;


} angle_data;


angle_data bmi_gps_data;


float gyro_x, gyro_y, gyro_z;
float acc_x, acc_y, acc_z;

int16_t gyro_xValue = 0;
int16_t gyro_yValue = 0;
int16_t gyro_zValue = 0;

int messageState = 0;


bool outputBufferReady 	= false;

bool gpsOutputready    	= false;

bool minInitialized 	= false;

bool maxInitialized 	= false;

char * headerStateMachineFilter = "$GNRMC\n";
char  receivedSentence[200];
char  receivedSentence2[200];

//uint8_t* currentSentenceBuffer;

uint8_t* firstSentenceBuffer;
uint8_t* secondSentenceBuffer;

//char  sprintString[200];



//uint8_t* outputBufferStart = receivedSentence;
//uint8_t* outputBufferCurrent = receivedSentence;
//uint8_t* outputBufferEnd = receivedSentence+199;




uint8_t* outputBufferStart; //= currentSentenceBuffer;
uint8_t* outputBufferCurrent; //= currentSentenceBuffer;
uint8_t* outputBufferEnd; //= currentSentenceBuffer+199;

uint8_t* secondOutputBufferStart; //= currentSentenceBuffer;
uint8_t* secondOutputBufferCurrent; //= currentSentenceBuffer;
uint8_t* secondOutputBufferEnd; //= currentSentenceBuffer+199;

void gyroTimerInterrupt(){
	gpsOutputready    = true;
}

void swapGPSBuffers(){

	uint8_t * tempSentence = firstSentenceBuffer;
	firstSentenceBuffer = secondSentenceBuffer;
	secondSentenceBuffer = tempSentence;

	//currentSentenceBuffer = firstSentenceBuffer;

	outputBufferStart = firstSentenceBuffer;
	outputBufferCurrent = firstSentenceBuffer;
	outputBufferEnd = firstSentenceBuffer+199;

	secondOutputBufferStart = secondSentenceBuffer;
	secondOutputBufferCurrent = secondSentenceBuffer;
	secondOutputBufferEnd = secondSentenceBuffer+199;
}


void gpsInterrupt(){


	UART_Receive(&UART_0, outputBufferCurrent, 1);


	if(headerStateMachineFilter[messageState] == *outputBufferCurrent){ //sequence correct go to next position;
		messageState++;
		outputBufferCurrent++;
		if(messageState == 7){
			swapGPSBuffers();
			outputBufferCurrent = outputBufferStart;
			messageState = 0;
			outputBufferReady = true;

		}

	}
	else if (messageState  < 6){ // sequence incorrect oh mi god
		outputBufferCurrent = outputBufferStart;
		messageState = 0;

	}
	else{ //data part of sequence just increment here;
		outputBufferCurrent++;
	}



//	if(messageState > 0){

//	}

}

void spi_flushrxbuffer(const SPI_MASTER_t *const handle)
{
	/* Clear the receive FIFO */
	XMC_USIC_CH_RXFIFO_Flush(handle->channel);
	while(!SPI_MASTER_IsRxFIFOEmpty(handle)) { SPI_MASTER_GetReceivedWord(handle); };
}


void bmi_gyro_poll_data(){


}

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


void udp_initialize()
{
	IP_ADDR4(&returnaddr, ip1,ip2,ip3,ip4);
	com_pcb = udp_new();
}


void udp_send_event2(char* message)
{

	struct pbuf* b = pbuf_alloc(PBUF_TRANSPORT, 75, PBUF_RAM);
	((char*)b->payload)[0] = 5;
	memcpy (b->payload+1, message, 75);
	char* c = (char*)b->payload;
	err_t e = udp_sendto(com_pcb, b, &returnaddr, PORT_COM_OUT);
	if(e == ERR_OK){
		printf("ok");
	}

	pbuf_free(b);
}

void udp_send_event(char* message)
{


	u16_t l = strlen(message)+1;
	struct pbuf* b = pbuf_alloc(PBUF_TRANSPORT, l+1, PBUF_RAM);
	((char*)b->payload)[0] = 3;
	memcpy (b->payload+1, message, l+1);
	char* c = (char*)b->payload;
	err_t e = udp_sendto(com_pcb, b, &returnaddr, PORT_COM_OUT);
	if(e == ERR_OK){
		printf("ok");
	}

	pbuf_free(b);
}





int main(void)
{
  DAVE_STATUS_t status;

  status = DAVE_Init();           /* Initialization of DAVE APPs  */

  firstSentenceBuffer = receivedSentence;
  secondSentenceBuffer = receivedSentence2;


  outputBufferStart = firstSentenceBuffer;
  outputBufferCurrent = firstSentenceBuffer;
  outputBufferEnd = firstSentenceBuffer+199;

  secondOutputBufferStart = secondSentenceBuffer;
  secondOutputBufferCurrent = secondSentenceBuffer;
  secondOutputBufferEnd = secondSentenceBuffer;


  cycledelay((uint32_t)(100000 * 2));

  udp_initialize();

  cycledelay(140000* 50);

  bmi055_start();

  if(status != DAVE_STATUS_SUCCESS)
  {
    /* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
    XMC_DEBUG("DAVE APPs initialization failed\n");

    while(1U)
    {

    }
  }

  long polltime = 0;


  /* Placeholder for user application code. The while loop below can be replaced with user application code. */
  while(1U)
  {

	  sys_check_timeouts();
	  polltime++;
	  if( polltime > 60000){//600000){
		  bmi055_poll_data();
		  bmi055_apply_gyro_all_angles();
		  bmi055_apply_accel_all_angles();



		  // min berekenen
if(maxInitialized){
	  max_complement_xValue =  max(complement_xValue, max_complement_xValue);
	  max_complement_yValue =  max(complement_yValue, max_complement_yValue);
	  max_complement_zValue =  max(complement_zValue, max_complement_zValue);
}
else{
	  max_complement_xValue = complement_xValue;
	  max_complement_yValue = complement_yValue;
	  max_complement_zValue = complement_zValue;
	  maxInitialized = true;
}

//		  max_complement_xValue = max(gyro_x, max_gyro_xValue);
//		  max_complement_yValue = max(gyro_y, max_gyro_yValue);
//		  max_complement_zValue = max(gyro_z, max_gyro_zValue);

		  //max berekenen

if(minInitialized){
//				min_complement_xValue = min(gyro_x, min_gyro_xValue);
//				min_complement_yValue = min(gyro_y, min_gyro_yValue);
//				min_complement_zValue = min(gyro_z, min_gyro_zValue);

				min_complement_xValue =  min(complement_xValue, min_complement_xValue);
				min_complement_yValue =  min(complement_yValue, min_complement_yValue);
				min_complement_zValue =  min(complement_zValue, min_complement_zValue);
		  }
		  else{
			  min_complement_xValue = complement_xValue;
			  min_complement_yValue = complement_yValue;
			  min_complement_zValue = complement_zValue;
			  minInitialized = true;

		  }

		  //avg berekenen

		  avg_complement_xValue += complement_xValue;
		  avg_complement_yValue += complement_yValue;
		  avg_complement_zValue += complement_zValue;

//		  __disable_irq();
//		  v.id[0] = 'G';
//		  v.id[1] = 'Y';
//		  v.g_x = gyro_x;
//		  v.g_y = gyro_y;
//		  v.g_z = gyro_z;
//		  char* c_v = &v;
//		  sprintf(sprintString,"gyro: %i, %i,  %i", gyro_xValue, gyro_yValue, gyro_zValue);
//		  udp_send_event(sprintString);
		 // udp_send_event2(c_v);
//		  __enable_irq();
		  polltime = 0;
	  }
	  if(gpsOutputready){
		  avg_complement_xValue = avg_complement_xValue/num_of_samples;
		  avg_complement_yValue = avg_complement_yValue/num_of_samples;
		  avg_complement_zValue = avg_complement_zValue/num_of_samples;

//		  bmi_gps_data.avg_x = avg_complement_xValue;
//		  bmi_gps_data.avg_y = avg_complement_yValue;
//		  bmi_gps_data.avg_z = avg_complement_zValue;
//		  bmi_gps_data.min_x = min_complement_xValue;
//		  bmi_gps_data.min_y = min_complement_yValue;
//		  bmi_gps_data.min_z = min_complement_zValue;
//		  bmi_gps_data.max_x = max_complement_xValue;
//		  bmi_gps_data.max_y = max_complement_yValue;
//		  bmi_gps_data.max_z = max_complement_zValue;

		  bmi_gps_data.avg_x = avg_complement_xValue;
		  bmi_gps_data.avg_y = avg_complement_yValue;
		  bmi_gps_data.avg_z = avg_complement_zValue;
		  bmi_gps_data.min_x = min_complement_xValue;
		  bmi_gps_data.min_y = min_complement_yValue;
		  bmi_gps_data.min_z = min_complement_zValue;
		  bmi_gps_data.max_x = max_complement_xValue;
		  bmi_gps_data.max_y = max_complement_yValue;
		  bmi_gps_data.max_z = max_complement_zValue;

//
//		  bmi_gps_data.avg_x = 3;
//		  bmi_gps_data.avg_y = 3;
//		  bmi_gps_data.avg_z = 3;
//		  bmi_gps_data.min_x = 4;
//		  bmi_gps_data.min_y = 4;
//		  bmi_gps_data.min_z = 4;
//		  bmi_gps_data.max_x = 5;
//		  bmi_gps_data.max_y = 5;
//		  bmi_gps_data.max_z = 5;


		  num_of_samples = 0;
		  max_complement_xValue = 0;
		  max_complement_yValue = 0;
		  max_complement_zValue = 0;

		  minInitialized = false;
		  maxInitialized = false;

		  min_complement_xValue = 0;
		  min_complement_yValue = 0;
		  min_complement_zValue = 0;

		  avg_complement_xValue = 0;
		  avg_complement_yValue = 0;
		  avg_complement_zValue = 0;

		  char* c_v = &bmi_gps_data;
		  udp_send_event2(c_v);
		  udp_send_event(secondOutputBufferStart);
		  gpsOutputready = false;
		  //sprintf(sprintString,"gyro: %i, %i,  %i", gyro_xValue, gyro_yValue, gyro_zValue);
	  }
	  if(outputBufferReady){
		 // udp_send_event(receivedSentence);
		  outputBufferReady = false;

	  }


	//  printf("g");


  }
}
