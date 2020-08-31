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
#include <dlog.h>
#include <glib.h>
#include <bp_manager_common.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "BPS_bpm_TIZEN"

extern int bpm_tizen_dbus_server_run();

int bpm_tizen_get_one()
{
	dlog_print(DLOG_INFO, LOG_TAG, "bptizen_get_one is called.......");
	return 0;
}

int _bpm_initialize() 
{
	dlog_print(DLOG_INFO, LOG_TAG, "_bpm_initialize is called.......Geeee");
	bpm_tizen_dbus_server_run();
	return 0;
}

int main()
{
	GMainLoop *mainloop = NULL;

	LOGD("Enter main loop\n");

#ifdef TIZEN_TEST_GCOV
	setenv("GCOV_PREFIX", "/tmp/daemon", 1);
#endif
    _bpm_initialize();
	// TODO(vincent): Why we need g_main_loop?
	mainloop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(mainloop);

	return 0;
}
