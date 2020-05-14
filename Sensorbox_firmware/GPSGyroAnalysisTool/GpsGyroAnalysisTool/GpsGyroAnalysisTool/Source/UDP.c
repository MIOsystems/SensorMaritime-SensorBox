/*
 * UDP.c
 *
 *  Created on: 13 May 2020
 *      Author: Beheerder
 */


#include "Header/UDP.h"

void udp_initialize()
{
	IP_ADDR4(&returnaddr, ip1,ip2,ip3,ip4);
	com_pcb = udp_new();
}




void udp_send_event_r(angle_data* strct)
{

//	struct pbuf* b = pbuf_alloc(PBUF_TRANSPORT, 200, PBUF_RAM);
//	((char*)b->payload)[0] = 7;
//	//ADMS,avg_x,avg_y,avg_z,min_x,min_y,min_z,max_x,max_y,max_z$
//	sprintf(r_msg, "ADR,%f,%f,%f,%f,%f,%f,%f,%f,%f$", strct->avg_x, strct->avg_y, strct->avg_z, strct->min_x, strct->min_y, strct->min_z, strct->max_x, strct->max_y, strct->max_z);


	sprintf(r_msg, "ADR,%f,%f,%f,%f,%f,%f,%f,%f,%f$", strct->avg_x, strct->avg_y, strct->avg_z, strct->min_x, strct->min_y, strct->min_z, strct->max_x, strct->max_y, strct->max_z);
	struct pbuf* b = pbuf_alloc(PBUF_TRANSPORT, strlen(r_msg), PBUF_RAM);
	memcpy (b->payload, r_msg, strlen(r_msg));
	char* c = (char*)b->payload;
	err_t e = udp_sendto(com_pcb, b, &returnaddr, PORT_COM_OUT);
	if(e == ERR_OK){
		printf("ok");
	}

	pbuf_free(b);
}


void udp_send_event_ms(ms_data* strct)
{



	sprintf(ms_msg, "ADMS,%f,%f,%f,%f,%f,%f,%f,%f,%f$", strct->avg_x, strct->avg_y, strct->avg_z, strct->min_x, strct->min_y, strct->min_z, strct->max_x, strct->max_y, strct->max_z);

	struct pbuf* b = pbuf_alloc(PBUF_TRANSPORT, strlen(ms_msg), PBUF_RAM);
	memcpy (b->payload, ms_msg, strlen(ms_msg));
	char* c = (char*)b->payload;
	err_t e = udp_sendto(com_pcb, b, &returnaddr, PORT_COM_OUT);
	if(e == ERR_OK){
		printf("ok");
	}

	pbuf_free(b);
}

void udp_send_event(char* message)
{


	u16_t l = strlen(message);
	struct pbuf* b = pbuf_alloc(PBUF_TRANSPORT, l, PBUF_RAM);
	//((char*)b->payload)[0] = 3;
	memcpy (b->payload, message, l);
	char* c = (char*)b->payload;
	err_t e = udp_sendto(com_pcb, b, &returnaddr, PORT_COM_OUT);
	if(e == ERR_OK){
		printf("ok");
	}

	pbuf_free(b);
}
