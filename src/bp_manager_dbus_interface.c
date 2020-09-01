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

const char* bp_manager_bus_name =       "org.tizen.bp.manager.tizen";
const char* bp_manager_object_path =    "/org/tizen/bps/bp/tizen/manager/object";
const char* bp_manager_interface_name = "org.tizen.bps.bp.manager.interface";
const char* bp_manager_method_name_1 = "get";
const char* param = "This must not be null";
DBusPendingCall* pending;
#if 0
static int bptizen_handle_dbus_signal() 
{
	DBusMessage* msg;
	DBusMessageIter args;
	DBusConnection* conn;
	DBusError err;

	int ret = 0;
	dbus_uint32_t serial = 0;
	static int count = 0;

	dlog_print(DLOG_INFO, LOG_TAG, "FUCK려차FUCK bptizen_handle_dbus_signal\n");
    
	// Initialize the error
	dbus_error_init(&err);

	// Connect to the bus and check for errors
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);

	// Check for dbus error
	if (dbus_error_is_set(&err)) 
	{
		dlog_print(DLOG_INFO, LOG_TAG, "Connection Error (%s)\n", err.message);
		dbus_error_free(&err);
	}

	if (NULL == conn) 
	{
		dlog_print(DLOG_INFO, LOG_TAG, "Connection Null\n");
		exit(1);
	}


    // Try a method call
	dlog_print(DLOG_INFO, LOG_TAG, "Start remote method call:: %s\n", bp_manager_bus_name);
	msg = dbus_message_new_method_call(bp_manager_bus_name, // target for the method call
	   	 	 	 	 	 	 	bp_manager_object_path, // object to call on
	                            bp_manager_interface_name, // interface to call on
    	                        bp_manager_method_name_1); // method name
    dlog_print(DLOG_INFO, LOG_TAG, "End remote method call %s\n", bp_manager_bus_name);
	// append arguments
	dbus_message_iter_init_append(msg, &args);

	if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &param)) {
		dlog_print(DLOG_INFO, LOG_TAG, "Out Of Memory!\n");
		exit(1);
	}

	// send message and get a handle for a reply
	if (!dbus_connection_send_with_reply (conn, msg, &pending, -1)) { // -1 is default timeout
		dlog_print(DLOG_INFO, LOG_TAG, "[Client] Out Of Memory!\n");
		exit(1);
	}
	
	dlog_print(DLOG_INFO, LOG_TAG, "sent message..dbus_connection_send_with_reply");

	if (NULL == pending) {
		dlog_print(DLOG_INFO, LOG_TAG, "[Client] Pending Call Null\n");
		exit(1);
	}
	dbus_connection_flush(conn);

	if (NULL == msg) {
	    dlog_print(DLOG_INFO, LOG_TAG, "[Client] Message Null\n");
		exit(1);
	}
}

void reply_to_method_call(DBusMessage* msg, DBusConnection* conn)
{
   DBusMessage* reply;
   DBusMessageIter args;
   bool stat = true;
   dbus_uint32_t level = 21614;
   dbus_uint32_t serial = 0;
   char* param;
   dbus_uint32_t retVal = -1;

   //Read the arguments. This method assumes it has one String and Integer value
   //and returns one string value to the requester
   if (!dbus_message_iter_init(msg, &args)) 
   {
       dlog_print(DLOG_INFO, LOG_TAG, "[SystemServerD] Message has no arguments!\n");
       return;
   }
   else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) 
   {
       dlog_print(DLOG_INFO, LOG_TAG, "[SystemServerD] Argument is not string!\n");
       return;
   }
   else 
   {
       dbus_message_iter_get_basic(&args, &param);
       dlog_print(DLOG_INFO, LOG_TAG, "[SystemServerD] METHOD received string is %s\n", param);
       if (!strcmp(param, "This must not be null")) 
	   {
    	   retVal = 1;
       }
       else {
    	   retVal = 2;
       }
   }

   // create a reply from the message
   reply = dbus_message_new_method_return(msg);

   // add the arguments to the reply
   dbus_message_iter_init_append(reply, &args);

   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &retVal)) 
   {
      dlog_print(DLOG_INFO, LOG_TAG, "[SystemServerD] Out Of Memory!\n");
      exit(1);
   }

   // send the reply && flush the connection
   if (!dbus_connection_send(conn, reply, &serial)) 
   {
      dlog_print(DLOG_INFO, LOG_TAG, "[SystemServerD] Out Of Memory!\n");
      exit(1);
   }
   dbus_connection_flush(conn);
   dlog_print(DLOG_INFO, LOG_TAG, "[SystemServerD] METHOD replied INT \n");
   //Unref is done below
   //dbus_message_unref(reply);
}
#endif

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
	//Check for primary for the requested name
	//if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) 
	//{
	//	dlog_print(DLOG_INFO, LOG_TAG,  "Not Primary Owner (%d)\n", ret);
	//	exit(1);
	//}

	//The below two line are very important steps to received this signal "request.to.launcherdaemon"
	//dbus_bus_add_match(conn, "type='signal',interface='request.to.launcherdaemon'", &err); // see signals from the given interface
	//dbus_connection_flush(conn);

#if 0
	//Wait for new message
	while (true) 
	{
		int received_val = 0;
		char *received_str = NULL;
		int current_type;

		//non blocking read of the next available message
		
		dbus_connection_read_write(conn, 0);
		msg = dbus_connection_pop_message(conn);
		
    	//loop again if we haven't got a message
		if (NULL == msg) 
		{
			continue;
		}
        dlog_print(DLOG_INFO, LOG_TAG, "FUCK bpm method call received request is not null.\n");
        dlog_print(DLOG_INFO, LOG_TAG, "bptizen_handle_dbus_signal call\n");
		{
			bptizen_handle_dbus_signal();
		}
		sleep(3000);

		//check this is a method call for the right interface & method
		if (dbus_message_is_method_call(msg, bpm_interface_name, bpm_method_name_1)) 
		{
		     //reply_to_method_call(msg, conn);
			dlog_print(DLOG_INFO, LOG_TAG, "bpm received request for %s %s\n", bpm_interface_name, bpm_method_name_1);
			reply_to_method_call(msg, conn);
			break;
		}
#if 0
		//if not a method, then signal?
		else if (dbus_message_is_signal(msg, interface_name_of_the_signal, name_of_the_signal)) 
		{
			dlog_print(DLOG_INFO, LOG_TAG, "[SystemServerD] SIGNAL received request for %s\n", name_of_the_signal);
			int still_left = FALSE;

			if (!dbus_message_iter_init(msg, &args)) 
			{
				dlog_print(DLOG_INFO, LOG_TAG, "[SystemServerD] SIGNAL has no message to get \n");
				continue;
			}
 
			do 
			{
				//read the parameters
				int args_type = dbus_message_iter_get_arg_type(&args);
				switch(args_type) {
				case DBUS_TYPE_UINT32 :
					dbus_message_iter_get_basic(&args, &received_val);
					dlog_print(DLOG_INFO, LOG_TAG, "[SystemServerD] SIGNAL parsed int %d\n", received_val);
					break;
				case DBUS_TYPE_DOUBLE:
					break;
				case DBUS_TYPE_STRING:
					dbus_message_iter_get_basic(&args, &received_str);
					dlog_print(DLOG_INFO, LOG_TAG, "[SystemServerD] SIGNAL parsed string %s\n", received_str);
					break;
				default:
					break;
				}
				still_left = dbus_message_iter_next(&args); //TRUE = 1 , FALSE = 0
			} while (still_left == TRUE);

			// TODO(vincent): Action should be defined.
		}
		else 
		{
			dlog_print(DLOG_INFO, LOG_TAG, "DBus type others\n");
			//Ignore something not filtered. May think about what can be placed here.
		}
		// free the message
		dbus_message_unref(msg);
#endif		
    }
#endif	
    // close the connection
	//dbus_connection_close(conn);
    return 0;
}



