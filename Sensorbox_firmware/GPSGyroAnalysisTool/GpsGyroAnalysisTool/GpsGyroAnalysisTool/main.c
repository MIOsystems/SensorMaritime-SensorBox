/*
 * main.c
 *
 *  Created on: 2020 Mar 20 15:00:36
 *  Author: Beheerder
 */


#include "Header/UDP.h"
#include "Header/GPS.h"
#include "Header/BMI.h"



#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))

#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)



/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */

void gyroTimerInterrupt(){


	pollcounter++;
	pollReady = true;
	if(pollcounter > 99){
		gpsOutputready   = true;
		pollcounter = 0;

	}


}

int main(void)
{
  DAVE_STATUS_t status;

  status = DAVE_Init();           /* Initialization of DAVE APPs  */

  initGPS();

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

  //long polltime = 0;


  /* Placeholder for user application code. The while loop below can be replaced with user application code. */
  while(1U)
  {

	  sys_check_timeouts();
	  if(pollReady){
	 // polltime++;
	  //if( polltime > 60000){
		  bmi055_poll_data();
		  bmi055_apply_gyro_all_angles();
		  bmi055_apply_accel_all_angles();





// ms berekenen

		  // min berekenen
				if(msMaxInitialized){
					max_ms_xValue =  max(ms_xValue, max_ms_xValue);
					max_ms_yValue =  max(ms_yValue, max_ms_yValue);
					max_ms_zValue =  max(ms_zValue, max_ms_zValue);
				}
				else{
					  max_ms_xValue = ms_xValue;
					  max_ms_yValue = ms_yValue;
					  max_ms_zValue = ms_zValue;
					  msMaxInitialized = true;
				}

				//max berekenen

				if(msMinInitialized){
					min_ms_xValue =  min(ms_xValue, min_ms_xValue);
					min_ms_yValue =  min(ms_yValue, min_ms_yValue);
					min_ms_zValue =  min(ms_zValue, min_ms_zValue);
				}
				else{
					  min_ms_xValue = ms_xValue;
					  min_ms_yValue = ms_yValue;
					  min_ms_zValue = ms_zValue;
					  msMinInitialized = true;

				}

				avg_ms_xValue += ms_xValue;
				avg_ms_yValue += ms_yValue;
				avg_ms_zValue += ms_zValue;



//~ ms berekenen


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


		  //max berekenen

		if(minInitialized){

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

		  //polltime = 0;
		  pollReady = false;
	  }
	  if(gpsOutputready){
		  avg_complement_xValue = avg_complement_xValue/num_of_samples;
		  avg_complement_yValue = avg_complement_yValue/num_of_samples;
		  avg_complement_zValue = avg_complement_zValue/num_of_samples;

		  avg_ms_xValue = avg_ms_xValue/num_of_samples;
		  avg_ms_yValue = avg_ms_yValue/num_of_samples;
		  avg_ms_zValue = avg_ms_zValue/num_of_samples;

		  bmi_gps_data.avg_x = avg_complement_xValue;
		  bmi_gps_data.avg_y = avg_complement_yValue;
		  bmi_gps_data.avg_z = avg_complement_zValue;
		  bmi_gps_data.min_x = min_complement_xValue;
		  bmi_gps_data.min_y = min_complement_yValue;
		  bmi_gps_data.min_z = min_complement_zValue;
		  bmi_gps_data.max_x = max_complement_xValue;
		  bmi_gps_data.max_y = max_complement_yValue;
		  bmi_gps_data.max_z = max_complement_zValue;

		  bmi_ms_data.avg_x = avg_ms_xValue;
		  bmi_ms_data.avg_y = avg_ms_yValue;
		  bmi_ms_data.avg_z = avg_ms_zValue;
		  bmi_ms_data.min_x = min_ms_xValue;
		  bmi_ms_data.min_y = min_ms_yValue;
		  bmi_ms_data.min_z = min_ms_zValue;
		  bmi_ms_data.max_x = max_ms_xValue;
		  bmi_ms_data.max_y = max_ms_yValue;
		  bmi_ms_data.max_z = max_ms_zValue;

		  udp_send_event_ms(&bmi_ms_data);
		  udp_send_event_r(&bmi_gps_data);

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

		  max_ms_xValue = 0;
		  max_ms_yValue = 0;
		  max_ms_zValue = 0;

		  msMinInitialized = false;
		  msMaxInitialized = false;

		  min_ms_xValue = 0;
		  min_ms_yValue = 0;
		  min_ms_zValue = 0;

		  avg_ms_xValue = 0;
		  avg_ms_yValue = 0;
		  avg_ms_zValue = 0;

		  udp_send_event(secondOutputBufferStart);
		  gpsOutputready = false;
	  }
	  if(outputBufferReady){
		  outputBufferReady = false;

	  }


  }
}
