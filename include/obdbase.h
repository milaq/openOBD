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

#ifndef _OBDBASE_H_
#define _OBDBASE_H_

#include <vector>
#include "ctb-0.15/ctb.h"
#include "logPanel.h"

using namespace std;
using namespace ctb;

#define PID_SUPPORTED1		0x0100
#define PID_DTC_STATUS		0x0101
#define PID_DTC_FREEZE		0x0102
#define PID_FUEL_SYSTEM		0x0103
#define PID_ENGINE_LOAD		0x0104
#define PID_COOLANT_TEMP	0x0105
#define PID_BANK1_STFT		0x0106
#define PID_BANK1_LTFT		0x0107
#define PID_BANK2_STFT		0x0108
#define PID_BANK2_LTFT		0x0109
#define PID_FUEL_PRESS		0x010A
#define PID_INTAKE_PRESS	0x010B
#define PID_ENGINE_RPM		0x010C
#define PID_VEHICLE_SPEED   0x010D
#define PID_TIMING_ADV		0x010E
#define PID_INTAKE_TEMP		0x010F
#define PID_MAF_RATE		0x0110
#define PID_THROTTLE_POS	0x0111
#define PID_ENGINE_RUN_TIME 0x011F
#define PID_O2_SEN_B1S1     0x0114
#define PID_O2_SEN_B1S2     0x0115
#define PID_O2_SEN_B1S3     0x0116
#define PID_O2_SEN_B1S4     0x0117
#define PID_O2_SEN_B2S1     0x0118
#define PID_O2_SEN_B2S2     0x0119
#define PID_O2_SEN_B2S3     0x011A
#define PID_O2_SEN_B2S4     0x011B
#define PID_SUPPORTED2      0x0120
#define PID_MIL_DIST		0x0121
#define PID_EGR_COMMANDED   0x012C
#define PID_EGR_ERROR		0x012D
#define PID_ENGINE_CYCLES   0x0130
#define PID_MIL_CLEAR_DIST  0x0131
#define PID_BARO_PRESSURE   0x0133
#define PID_CAT_TEMP_B1S1   0x013C
#define PID_CAT_TEMP_B2S1   0x013D
#define PID_CAT_TEMP_B1S2   0x013E
#define PID_CAT_TEMP_B2S2   0x013F
#define PID_SUPPORTED3      0x0140
#define PID_REL_THRO_POS    0x0145
#define PID_AMB_AIR_TEMP	0x0146
#define PID_ABS_THRO_B      0x0147
#define PID_ABS_THRO_C      0x0148
#define PID_ACC_PEDAL_D     0x0149
#define PID_ACC_PEDAL_E     0x014A
#define PID_ACC_PEDAL_F     0x014B
#define PID_MIL_TIME		0x014D
#define PID_MIL_CLEAR_TIME  0x014E
#define PID_ETHANOL_FUEL_PC 0x0151

#define PID_VIN				0x0902

class obdbase
{
public:

    struct pidInfo {
        int pid_flag;
        double resultMain;
        double resultSecondary;
        wxString resultString;
    };

    enum pidFlag {
        PID_FLAG_SINGLE,
        PID_FLAG_DOUBLE,
        PID_FLAG_STRING
    };

    obdbase ();
	obdbase (const wxString& serialPort);
	~obdbase ();

	// admin functions
	virtual wxString obd_identify_device ();
	virtual void obd_device_soft_reset () {};
	virtual void obd_init_fast () {};
	virtual void obd_init_slow () {};
	virtual void obd_set_protocol () {};
	virtual wxString obd_get_protocol();
	virtual void obd_device_disconnect ();
	virtual void obd_use_checksums (bool use);
	virtual void obd_use_imperial (bool use);
	virtual bool obd_is_imperial ();
	bool obd_is_connected();
	void obd_set_logger (logPanel* log);

	// error code functions
	virtual int obd_mil_status();
	virtual wxArrayString obd_mil_error_codes ();
	virtual bool obd_clear_dtc ();

	// PID functions
	virtual bool obd_pid_get_raw (int pid, int tokens[], int toksize);
	virtual bool obd_pid_value(int pid, obdbase::pidInfo* result);
    virtual void obd_pid_supported_pids (std::vector<int>& pids);

protected:
	ctb::SerialPort* port;
	logPanel* logger;
	bool logExtra;

	bool useChecksum;
	bool useImperial;

	// connection functions
	virtual void obd_device_connect (const wxString& SerialPort);
	virtual bool obd_write(const wxString& command, int count);
	virtual wxString obd_read();

	// checksum functions
	void obd_calculate_checksum ();
	bool obd_validate_checksum ();
	void obd_append_checksum ();

	// convertion functions
	int convert_c_to_f (int centigrade);
	int convert_kph_to_mph (int kph);

private:

    // number of error codes retreived at last attempt.
    int lastErrorCount;
    std::vector<int> pidList;
};

#endif // _OBDBASE_H_
