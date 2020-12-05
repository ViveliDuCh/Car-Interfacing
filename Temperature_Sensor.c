//==============================================================================
//
// Title:		Temperature_Sensor
// Purpose:		Read Temperature of a LM35 sensor connected to Channel 12
//              on a IO-14. Run "IO14GUI.exe" application found in
//              DLPdesign.com before running this code to check if 
//              the board is OK.
// Author:      Elmer Homero
// Created on:	10/18/2016 at 9:30:21 PM by Calypso.
// Copyright:	ITESM. All Rights Reserved.
// Note:        Written in LabWindows/CVI 2015
//==============================================================================

//==============================================================================
// Include files

#include <rs232.h>
#include <ansi_c.h>
#include <math.h>
#include <cvirte.h>		
#include <userint.h>
#include "Temperature_Sensor.h"
#include "toolbox.h"

//==============================================================================
// Constants. Make sure the port is selected before running code!
#define COM_PORT 3

//==============================================================================
// Types

//==============================================================================
// Static global variables

static int panelHandle = 0;
	   
 	   int selTemp     = 1;
	   int selRPM      = 0; 
	   int selSpeed    = 0; 
	   int selThrottle = 0; 
	   int selPressure = 0; 	
	  

//==============================================================================
// Function prorotypes
int Initialize(void); // Function that opens comm port and initializes
                      // IO-14 by means of handshaking (see IO-14 pdf file)

//==============================================================================
// Static functions

//==============================================================================
// Global variables
unsigned char Enable = 0; // Initialization of comm port flag 

//==============================================================================
// Global functions
// BYTES TRANSLATOR, FROM ASCII TO ITS DECIMAL ACTUAL VALUE
//'6''E'>>6Eh>> 110d
 int cambioDec (char B1ascii, char B2ascii){
	int result, B1dec,B2dec; 
	//Transformación de B1
	//Num- cuya base es 48
	if (B1ascii>=48 && B1ascii<=57){
		B1dec=B1ascii-48;
	}else{ //Letra - cuya base es 55
		B1dec=B1ascii-55;
	}
	//Transformación de B2
	if (B2ascii>=48 && B2ascii<=57){
		B2dec=B2ascii-48;
	}else{ //Letra - cuya base es 55
		B2dec=B2ascii-55;
	}
	/*
	If the hexadecimal number is 1A.
	dec_value = 1*(16^1) + 10*(16^0) = 26
	*/
	result=B1dec*pow(16,1)+B2dec*pow(16,0);

	return result; 
}
/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
	int error = 0;
	
	
	/* initialize and load resources */
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (panelHandle = LoadPanel (0, "Temperature_Sensor.uir", PANEL));
	
	
	/* display the panel and run the user interface */
	errChk (DisplayPanel (panelHandle));
	errChk (RunUserInterface ());
	

Error:
	/* clean up */
	if (panelHandle > 0)
		DiscardPanel (panelHandle);
	return 0;
}

//==============================================================================
// UI callback function prototypes

/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK panelCB (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	if (event == EVENT_CLOSE)
		QuitUserInterface (0);
	return 0;
}

// Function that open Comm port and initializes IO-14 using handshake
// Refer to IO-14 pdf file for details. (www.dlpdesign.com)
int Initialize ()
{
			 //CONFIGURANDO EL PUERTO
		    OpenComConfig (COM_PORT, "", 38400, 0, 8, 1, 512, 512);

		  // Set flag to indicate that initialization has been done and port is open
		  Enable = 1;

	return 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------TEMPERATURE**
int CVICALLBACK TimerCallback (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	
	
	//-----------------------------------------TEMPERATURA-------------------------------------------
	char CallTemp[6]     = {0x30,0x31,0x30,0x35,0x0D,'\0'}; // Request temperature reading, '\0' null character to end array
	
	
	unsigned char ReadTemp[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};       // Two bytes are read from IO-14, LSB first, followed by MSB
    int  bytes_sent,i;
	float Tmp, ValueTemp;
	 //--------------------------------------------------------------------------------------------------
	// Initialize comms port and IO-14 when program runs for the first time
	if (Enable == 0)
	  Initialize();
	// Enable flag different from 0, meaning that the port is open
	// and IO-14 has been initialialized
	else {
		
			if(selTemp==1){
		
			  		// Preventes program from getting a NON-FATAL RUN TIME ERROR Messages when program terminates
				  SetBreakOnLibraryErrors (0);
	  
				  //--------------------------------------TEMPERATURE-------------------------------------------------
	  
			      // Request Temperature reading from OBDII, 1 byte sent
	//	/*
				  for (i = 0; i < 6; i++) {
				    bytes_sent = ComWrtByte (COM_PORT, CallTemp[i]);
   
				  }
				  
				  // Read Temperature, 2 bytes of information
				  for (i = 0; i < 17; i++) {
				    ReadTemp[i] = ComRdByte (COM_PORT);
				    Delay(0.01);
					printf("%c",ReadTemp[i]);
					printf("\n");      
				  }

				  // See OBDII data sheet for formula
				  //Tmp   = (float)ReadTemp[2] * 1.0;
				  Tmp   = (float)cambioDec(ReadTemp[11],ReadTemp[12]);//METODO
			      ValueTemp = Tmp -40.0; ////Valor en grados cent
				  printf("El tmp es: %f",Tmp);
				  printf("\n");
			
				  // Display temperature
				  SetCtrlVal(panelHandle,PANEL_TEMPERATURE,ValueTemp);
				  SetCtrlVal(panelHandle,PANEL_COOLTEMP,ValueTemp);
				  
				  //Cleaning the array to read again
				  for (i = 0; i < 20; i++) {
				    ReadTemp[i] ='\0';
				  }
	  			 
//	   */
		
				selTemp     = 0;
				selRPM      = 1; 
				selSpeed    = 0; 
				selThrottle = 0; 
				selPressure = 0; 
	
			}
	  
			// Enable NON-FATAL RUN TIME ERROR Messages to show (default)
			SetBreakOnLibraryErrors (1);
	}
	
	return 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------RPM**    

int CVICALLBACK timer_rpm (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	 
	char CallRPM[6]     = {0x30,0x31,0x30,0x43,0x0D,'\0'}; // Request RPM reading, '\0' null character to end array
	
	
	unsigned char ReadRPM[30];      // bytes for reading from OBDII followed by an > char   
    int  bytes_sent,i;
	float TmpA,TmpB, ValueRPM;
	//Cleaning the array to read again
	for (i = 0; i < 30; i++) {
		ReadRPM[i] ='\0';
	}
	  			 
	// Initialize comms port and IO-14 when program runs for the first time
	if (Enable == 0)
	  Initialize();
	// Enable flag different from 0, meaning that the port is open
	// and IO-14 has been initialialized
	else {
		  
		   if(selRPM==1){
				  // Preventes program from getting a NON-FATAL RUN TIME ERROR Messages when program terminates
				  SetBreakOnLibraryErrors (0);
	 // /* 
			      // Request Temperature reading from IO-14, 1 byte sent
				  for (i = 0; i < 6; i++) {
				    bytes_sent = ComWrtByte (COM_PORT, CallRPM[i]);
				  }
				  // Read RPM, 2 bytes of information
				  for (i = 0; i < 20; i++) {
				    ReadRPM[i] = ComRdByte (COM_PORT);
				    Delay(0.01);
					printf("%c",ReadRPM[i]);
					printf("\n");      
				  }

				  // See OBDII data sheet for formula
				  //Tmp   = (float)ReadTemp[2] * 1.0;
				  TmpA   = (float)cambioDec(ReadRPM[11],ReadRPM[12]);
				  TmpB   = (float)cambioDec(ReadRPM[14],ReadRPM[15]);    
			      ValueRPM = ((TmpA*256.0)+TmpB*1.0)/4.0; ////Valor en rpm
				  printf("rpm A es: %f",TmpA);
				  printf("\n");
				  printf("rpm B es: %f",TmpB);
				  printf("\n");
				  // Value = Value * 100;
				  // Display rpm
				  SetCtrlVal(panelHandle,PANEL_RPM_2,ValueRPM);
				  SetCtrlVal(panelHandle,PANEL_RPMGAUGE,ValueRPM);  
				
	  	// */ 
	  
				 selTemp     = 0;
				 selRPM      = 0; 
				 selSpeed    = 1; 
				 selThrottle = 0; 
				 selPressure = 0; 
				 
				 

				
		   }
	  
	  // Enable NON-FATAL RUN TIME ERROR Messages to show (default)
	  SetBreakOnLibraryErrors (1);
	 
	 }
	 
	return 0;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------SPEED**    



int CVICALLBACK timer_speed (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	char CallSpeed[6]     = {0x30,0x31,0x30,0x44,0x0D,'\0'}; // Request SPEED reading, '\0' null character to end array
											
	
	unsigned char ReadSpeed[30];       // bytes for reading from OBDII followed by an > char
    int  bytes_sent,i;
	float speed,Value_Speed;
	
	//Cleaning the array to read
	for (i = 0; i < 30; i++) {
		ReadSpeed[i] ='\0';
	}
	// Initialize comms port and IO-14 when program runs for the first time
	if (Enable == 0)
	  Initialize();
	// Enable flag different from 0, meaning that the port is open
	// and IO-14 has been initialialized
	else {
		   
		   if(selSpeed==1){
				  // Preventes program from getting a NON-FATAL RUN TIME ERROR Messages when program terminates
				  SetBreakOnLibraryErrors (0);
  // /*
			      // Request Temperature reading from IO-14, 1 byte sent
				  for (i = 0; i < 6; i++) {
				    bytes_sent = ComWrtByte (COM_PORT, CallSpeed[i]);
				  }
				  
				  // Read Speed, 2 bytes of information
				  for (i = 0; i < 17; i++) {
				    ReadSpeed[i] = ComRdByte (COM_PORT);
				    Delay(0.01);
					printf("%c",ReadSpeed[i]);
					printf("\n");      
				  }

				  // See OBDII data sheet for formula
				  //Tmp   = (float)ReadTemp[2] * 1.0;
				  speed   = (float)cambioDec(ReadSpeed[11],ReadSpeed[12]);
			      Value_Speed = speed; ////Valor en km/h --- Rango de valores: 0-255
				  printf("speed es: %f",speed);
				  printf("\n");
				
				  // Display speed
				  SetCtrlVal(panelHandle,PANEL_SPEED,Value_Speed);
				  SetCtrlVal(panelHandle,PANEL_SPEEDMETER,Value_Speed);  
		
//	*/		
   				 
				 selTemp     = 0;
				 selRPM      = 0; 
				 selSpeed    = 0; 
				 selThrottle = 1; 
				 selPressure = 0;
				 
 
				
		   }
	  
	  // Enable NON-FATAL RUN TIME ERROR Messages to show (default)
	  SetBreakOnLibraryErrors (1);
	 
	 }
	return 0;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------THROTTLE**   
int CVICALLBACK timer_throttle (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	char CallThro[6]     = {0x30,0x31,0x31,0x31,0x0D,'\0'}; // Request THROTTLE reading, '\0' null character to end array
  										
	
	unsigned char ReadThro[30];       // bytes for reading from OBDII followed by an > char
    int  bytes_sent,i;
	float throttle,Value_Throttle;
	//Cleaning the array to read again
	for (i = 0; i < 30; i++) {
		ReadThro[i] ='\0';
	}
	// Initialize comms port and IO-14 when program runs for the first time
	if (Enable == 0)
	  Initialize();
	// Enable flag different from 0, meaning that the port is open
	// and IO-14 has been initialialized
	else {
		   
		 
		   if(selThrottle==1){
				  // Preventes program from getting a NON-FATAL RUN TIME ERROR Messages when program terminates
				  SetBreakOnLibraryErrors (0);
//	  /* 
			      // Request Throttle reading from OBDII, 1 byte sent
				  for (i = 0; i < 6; i++) {
				    bytes_sent = ComWrtByte (COM_PORT, CallThro[i]);
				  }
				  // Read Throttle, 2 bytes of information
				  for (i = 0; i < 17; i++) {
				    ReadThro[i] = ComRdByte (COM_PORT);
				    Delay(0.01);
					printf("%c",ReadThro[i]);
					printf("\n");      
				  }

				  // See OBDII data sheet for formula
				  
				  throttle   = (float)cambioDec(ReadThro[11],ReadThro[12]);
				  //Por orden de operaciones se hace primero la mult y luego la div
			      Value_Throttle = (throttle/255.0)*100.0; ////Valor en % --- Rango de valores: 0-100%
				  printf("throttle antes de formula es: %f",throttle);
				  printf("\n");
				  // Value = Value * 100;
				  // Display throttle
				  SetCtrlVal(panelHandle,PANEL_THROTTLE,Value_Throttle);
				  SetCtrlVal(panelHandle,PANEL_THROTTLETANK,Value_Throttle);  
				  
		 
//		*/ 
				 selTemp     = 0;
				 selRPM      = 0; 
				 selSpeed    = 0; 
				 selThrottle = 0; 
				 selPressure = 1;  

				
		   }
	  
	  // Enable NON-FATAL RUN TIME ERROR Messages to show (default)
	  SetBreakOnLibraryErrors (1);
	  
	}
	return 0;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------PRESSURE**    
int CVICALLBACK timer_pressure (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	char CallPres[6]     = {0x30,0x31,0x30,0x42,0x0D,'\0'}; // Request PRESSURE reading, '\0' null character to end array
											
	
	unsigned char ReadPres[30];       // bytes for reading from OBDII followed by an > char
    int  bytes_sent,i;
	float pressure,Value_Pressure;
	//Cleaning the array to read again
	for (i = 0; i < 30; i++) {
		ReadPres[i] ='\0';
	}
	// Initialize comms port and OBDII when program runs for the first time
	if (Enable == 0)
	  Initialize();
	// Enable flag different from 0, meaning that the port is open
	// and IO-14 has been initialialized
	else {
		
		   if(selPressure==1){
				  // Preventes program from getting a NON-FATAL RUN TIME ERROR Messages when program terminates
				  SetBreakOnLibraryErrors (0);
  // /* 
			      // Request Speed reading from OBDII, 1 byte sent
				  for (i = 0; i < 6; i++) {
				    bytes_sent = ComWrtByte (COM_PORT, CallPres[i]);
				  }

				  // Read Pressure, 1 byte of information will be read plus the endls and spaces in between
				  // Read Speed, 2 bytes of information
				  for (i = 0; i < 17; i++) {
				    ReadPres[i] = ComRdByte (COM_PORT);
				    Delay(0.01);
					printf("%c",ReadPres[i]);
					printf("\n");      
				  }
			     
				  //printf("\n");  

				  // See OBDII data sheet for formula
				  
				  pressure   = (float)cambioDec(ReadPres[11],ReadPres[12]); 
			      Value_Pressure = pressure; ////Valor en KPa (absoluto) --- Rango de valores: 0-255
				  printf("pressure antes de formula es: %f",pressure);
				  printf("\n");
				 
				  // Display the absolute value of the pressure in KPa
				  SetCtrlVal(panelHandle,PANEL_ABS_PRES,Value_Pressure);
				  SetCtrlVal(panelHandle,PANEL_PRESSMETER,Value_Pressure); 
				  
   
  //	 */ 
				 selTemp     = 1;
				 selRPM      = 0; 
				 selSpeed    = 0; 
				 selThrottle = 0; 
				 selPressure = 0; 

				
		   }
	  
	  // Enable NON-FATAL RUN TIME ERROR Messages to show (default)
	  SetBreakOnLibraryErrors (1);
	 
	 }
	return 0;
}





int CVICALLBACK QuitCallback (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			CloseCom (COM_PORT);
			QuitUserInterface (0);
			break;
		case EVENT_RIGHT_CLICK:

			break;
	}
	return 0;
}


