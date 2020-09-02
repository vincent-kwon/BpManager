/*
 * Copyright (c) 2020 hobum.kwon Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <dlog.h>
#include <unistd.h>
#include <dbus/dbus.h>

#define LOG_TAG "BP_MANAGER"

const char* bpm_bus_name =       "org.tizen.bp.manager";
const char* bpm_object_path =    "/org/tizen/bp/manager";
const char* bpm_interface_name = "org.tizen.bp.manager.interface";
const char* bpm_method_name_1 = "get";

//const char* bp_manager_bus_name =       "org.tizen.bp.manager.tizen";
//const char* bp_manager_object_path =    "/org/tizen/bps/bp/tizen/manager/object";
//const char* bp_manager_interface_name = "org.tizen.bps.bp.manager.interface";
//const char* bp_manager_method_name_1 = "get";
//const char* param = "This must not be null";

DBusPendingCall* pending;

int bpm_tizen_dbus_server_run() 
{
	DBusMessage* msg;
	DBusMessageIter args;
	DBusConnection* conn;
	DBusError err;
	int ret = 0;

    dlog_print(DLOG_INFO, LOG_TAG, "bpm_tizen_dbus_server_run\n");
    //initialize the error
	dbus_error_init(&err);

	//connect to the bus and check for errors
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);

	//Check for dbus error
	if (dbus_error_is_set(&err)) 
	{
		dlog_print(DLOG_INFO, LOG_TAG, "Connection Error (%s)\n", err.message);
		dbus_error_free(&err);
	}

	if (NULL == conn) 
	{
		dlog_print(DLOG_INFO, LOG_TAG,  "Connection Null\n");
		exit(1);
	}

	//request our name on the bus and check for errors
	ret = dbus_bus_request_name(conn, bpm_bus_name, DBUS_NAME_FLAG_REPLACE_EXISTING , &err);

	//For request name, check error
	if (dbus_error_is_set(&err)) 
	{
		dlog_print(DLOG_INFO, LOG_TAG, "Fuck Name Error (%s)\n", err.message);
		dbus_error_free(&err);
	}

    dlog_print(DLOG_INFO, LOG_TAG, "GOT Here !!!!!!!!!!!!!!!!!!!!\n");

	//Wait for new message
	while (true) 
	{
		int received_val = 0;
		char *received_str = NULL;
		int current_type;

		//non blocking read of the next available message
		dbus_connection_read_write(conn, 0);
		msg = dbus_connection_pop_message(conn);
		
		//dlog_print(DLOG_INFO, LOG_TAG, "BP Got message.............\n");
		
    	//loop again if we haven't got a message
		if (NULL == msg) 
		{
			continue;
		}
        
		dlog_print(DLOG_INFO, LOG_TAG, "BP Fuck Got message.............\n");
		
		//check this is a method call for the right interface & method
		if (dbus_message_is_method_call(msg, bpm_interface_name, bpm_method_name_1)) 
		{
		     //reply_to_method_call(msg, conn);
			dlog_print(DLOG_INFO, LOG_TAG, "Finally!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!bpm received request for %s %s\n", bpm_interface_name, bpm_method_name_1);
			//reply_to_method_call(msg, conn);
			break;
		}
    }
    // close the connection
	//dbus_connection_close(conn);
    return 0;
}



