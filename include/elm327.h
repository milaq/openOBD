/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * openobd
 * Copyright (C) Simon Booth 2010 <simesb@users.sourceforge.net>
 *
 * openobd is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * openobd is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _ELM327_H_
#define _ELM327_H_

#include "obdbase.h"

#define PROTO_AUTOMATIC			0x00    ///< Automatic protocol
#define PROTO_J1850_PWM			0x01    ///<
#define PROTO_J1850_VPW			0x02    ///<
#define PROTO_9141				0x03    ///<
#define PROTO_14230_BAUD_INIT   0x04    ///<
#define PROTO_14230_FAST_INIT   0x05    ///<
#define PROTO_15765_11_500		0x06    ///<
#define PROTO_15765_29_500		0x07    ///<
#define PROTO_15765_11_250		0x08    ///<
#define PROTO_15765_29_250		0x09    ///<
#define PROTO_J1939				0x0A    ///<
#define PROTO_USER_1			0x0B    ///<
#define PROTO_USER_2			0x0C    ///<

class elm327: public obdbase
{
public:
	elm327 (const wxString& serialPort);
	bool obd_set_protocol (int OBDprotocol);
	wxString obd_identify_device ();
	wxString obd_get_protocol();
	void obd_init_slow ();
	void obd_init_fast ();
	void elm_set_headers (bool show);
	void elm_set_echo (bool show);

protected:

private:
	wxString obd_send_AT_command (const wxString& command);
};

#endif // _ELM327_H_
