/*****************************************************************************
 *
 * SRETENTION.C - State retention routines for Nagios
 *
 * Copyright (c) 1999-2006 Ethan Galstad (nagios@nagios.org)
 * Last Modified:   03-05-2006
 *
 * License:
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *****************************************************************************/

/*********** COMMON HEADER FILES ***********/

#include "../include/config.h"
#include "../include/common.h"
#include "../include/objects.h"
#include "../include/statusdata.h"
#include "../include/nagios.h"
#include "../include/sretention.h"
#include "../include/broker.h"

extern int            retain_state_information;



/**** IMPLEMENTATION SPECIFIC HEADER FILES ****/
#ifdef USE_XRDDEFAULT
#include "../xdata/xrddefault.h"		/* default routines */
#endif






/******************************************************************/
/************* TOP-LEVEL STATE INFORMATION FUNCTIONS **************/
/******************************************************************/


/* initializes retention data at program start */
int initialize_retention_data(char *config_file){
	int result=OK;

	/**** IMPLEMENTATION-SPECIFIC CALLS ****/
#ifdef USE_XRDDEFAULT
	result=xrddefault_initialize_retention_data(config_file);
#endif

	return result;
        }



/* cleans up retention data before program termination */
int cleanup_retention_data(char *config_file){
	int result=OK;

	/**** IMPLEMENTATION-SPECIFIC CALLS ****/
#ifdef USE_XRDDEFAULT
	result=xrddefault_cleanup_retention_data(config_file);
#endif

	return result;
        }



/* save all host and service state information */
int save_state_information(int autosave){
	char *temp_buffer=NULL;
	int result=OK;

#ifdef DEBUG0
	printf("save_state_information() start\n");
#endif

	if(retain_state_information==FALSE)
		return OK;

#ifdef USE_EVENT_BROKER
	/* send data to event broker */
	broker_retention_data(NEBTYPE_RETENTIONDATA_STARTSAVE,NEBFLAG_NONE,NEBATTR_NONE,NULL);
#endif

	/********* IMPLEMENTATION-SPECIFIC OUTPUT FUNCTION ********/
#ifdef USE_XRDDEFAULT
	result=xrddefault_save_state_information();
#endif

#ifdef USE_EVENT_BROKER
	/* send data to event broker */
	broker_retention_data(NEBTYPE_RETENTIONDATA_ENDSAVE,NEBFLAG_NONE,NEBATTR_NONE,NULL);
#endif

	if(result==ERROR)
		return ERROR;

	if(autosave==TRUE){
		asprintf(&temp_buffer,"Auto-save of retention data completed successfully.\n");
		write_to_all_logs(temp_buffer,NSLOG_PROCESS_INFO);
		my_free((void **)&temp_buffer);
	        }

#ifdef DEBUG0
	printf("save_state_information() end\n");
#endif

	return OK;
        }




/* reads in initial host and state information */
int read_initial_state_information(void){
	int result=OK;

#ifdef DEBUG0
	printf("read_initial_state_information() start\n");
#endif

	if(retain_state_information==FALSE)
		return OK;

#ifdef USE_EVENT_BROKER
	/* send data to event broker */
	broker_retention_data(NEBTYPE_RETENTIONDATA_STARTLOAD,NEBFLAG_NONE,NEBATTR_NONE,NULL);
#endif

	/********* IMPLEMENTATION-SPECIFIC INPUT FUNCTION ********/
#ifdef USE_XRDDEFAULT
	result=xrddefault_read_state_information();
#endif

#ifdef USE_EVENT_BROKER
	/* send data to event broker */
	broker_retention_data(NEBTYPE_RETENTIONDATA_ENDLOAD,NEBFLAG_NONE,NEBATTR_NONE,NULL);
#endif

	if(result==ERROR)
		return ERROR;

#ifdef DEBUG0
	printf("read_initial_state_information() end\n");
#endif

	return OK;
        }



