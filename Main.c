#include "rl_net.h"                     // Keil.MDK-Pro::Network:CORE
#include "Board_LED.h" 									// ::Board Support:LED
#include "Board_Buttons.h"              // ::Board Support:Buttons
#include "Board_ADC.h"                  // ::Board Support:A/D Converter

uint8_t etat_batterie[4];
short pot;
char Cconversion;
int i,conversion;
int32_t udp_sock;                       // UDP socket handle
// Send UDP data to destination client.
void send_udp_data (void) {
 
  if (udp_sock > 0) {
     
    // IPv4 address: 192.168.0.2
    NET_ADDR addr = { NET_ADDR_IP4, 2000 , 192, 168, 0, 2 };

    uint8_t *sendbuf;
		ADC_StartConversion();
		//while(ADC_ConversionDone!=0);
		conversion=ADC_GetValue();
		
		etat_batterie[0]=conversion>>8;
		etat_batterie[1]=conversion;
		//printf(etat_batterie, conversion);
		
		
    sendbuf = netUDP_GetBuffer (3);
    sendbuf[0] = 0xAA;
    sendbuf[1] = etat_batterie[0];
		sendbuf[2] = etat_batterie[1];

 
    netUDP_Send (udp_sock, &addr, sendbuf, 3);
    
  }
}
 

uint32_t udp_cb_func (int32_t socket, const  NET_ADDR *addr, const uint8_t *buf, uint32_t len) {
 
  // Data received
  
  if (buf[0] == 0xAA) {
    LED_On (0);
		send_udp_data();
  }
	else LED_On(buf[1]);
	//send_udp_data();
  
  return (0);
}

int main()
{//Initialisation

int status;
LED_Initialize();
netInitialize();
Buttons_Initialize();
ADC_Initialize();

	  //Initialize UDP socket and open port 2000
	udp_sock = netUDP_GetSocket (udp_cb_func);
  if (udp_sock > 0) {
    netUDP_Open (udp_sock, 2000);}
	
	
	while(1)
	{
	/*	 status=Buttons_GetState ();
	
	if(status == 1)
	{send_udp_data();
	LED_On (0x02);}*/
	}
}


 
