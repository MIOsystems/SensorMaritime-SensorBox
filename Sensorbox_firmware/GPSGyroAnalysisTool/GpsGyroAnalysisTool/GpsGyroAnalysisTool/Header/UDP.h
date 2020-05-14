/*
 * UDP.h
 *
 *  Created on: 13 May 2020
 *      Author: Beheerder
 */

#ifndef HEADER_UDP_H_
#define HEADER_UDP_H_

#include <DAVE.h>

#define ip1 192							/** Firstbyte of the IP address **/
#define ip2 168							/** Second byte of the IP address **/
#define ip3 0							/** Third byte of the IP address **/
#define ip4 9

#define PORT_COM_OUT    52256			/** The port at which the receiver receives the UDP packets **/
#define PORT_COM_IN     62509			/** The port at which the chip transmits the UDP packets **/


struct udp_pcb*  com_pcb;
ip_addr_t returnaddr;

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



typedef struct ms_data {
	double avg_x;
	double avg_y;
	double avg_z;
	double min_x;
	double min_y;
	double min_z;
	double max_x;
	double max_y;
	double max_z;


} ms_data;


angle_data bmi_gps_data;
ms_data bmi_ms_data;

char ms_msg[104] , r_msg[104];

void udp_initialize();


void udp_send_event_r(angle_data* strct);


void udp_send_event_ms(ms_data* strct);

void udp_send_event(char* message);


#endif /* HEADER_UDP_H_ */
