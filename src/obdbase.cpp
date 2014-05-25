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
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
    #include <wx/string.h>
#endif

#include <wx/tokenzr.h>
#include <cstring>
#include <fcntl.h>
#include "obdbase.h"
#include "logPanel.h"
#include "ctb-0.15/ctb.h"

using namespace std;
using namespace ctb;

obdbase::obdbase ()
{
    // create a new serial port object
	port = new ctb::SerialPort();

	// setup the defaults
	this->obd_use_checksums(true);
	this->obd_use_imperial(false);

	this->logger = NULL;
	this->logExtra = true;
}

obdbase::obdbase (const wxString& SerialPort)
{
    // create a new serial port object
	port = new ctb::SerialPort();
	// and connect it to the chosen port
    this->obd_device_connect(SerialPort);

	// setup the defaults
	this->obd_use_checksums(true);
	this->obd_use_imperial(false);

	this->logger = NULL;
	this->logExtra = true;
	this->lastErrorCount = 0;
}

obdbase::~obdbase ()
{
    this->obd_device_disconnect();
    delete port;
}

void obdbase::obd_device_connect (const wxString& SerialPort)
{
    int baudrate = 38400;

    // open the serial port
    port->Open(SerialPort.mb_str(wxConvUTF8), baudrate);
}

void obdbase::obd_device_disconnect ()
{
    // close the port
    port->Close();

    // reset the list of supported pids
    pidList.clear();
}

void obdbase::obd_use_checksums (bool use)
{
	this->useChecksum = use;
}

void obdbase::obd_use_imperial (bool use)
{
	this->useImperial = use;
}

bool obdbase::obd_is_imperial ()
{
	return this->useImperial;
}

void obdbase::obd_set_logger (logPanel* log)
{
	this->logger = log;
}

int obdbase::obd_mil_status()
{
	int result = -1;
	int toks[10];
	wxString msg;

	// write to log if necessary
	if (logger) {
		msg = _("Requesting MIL Status\n");
		logger->appendLog(msg, logPanel::LOG_OUT);
	}

	// return -1 if we haven't got a well formed response
	if (this->obd_pid_get_raw(PID_DTC_STATUS, toks, sizeof(toks))) {

		// get the value of the '3rd byte' and apply conversion formula
		if (toks[2] > 0x80) {
			result = toks[2] - 0x80;
		} else {
			result = 0;
		}

		// write to log if necessary
		if (logger) {
			msg.Printf(_("Received MIL Response: %d.\n"), result);
			logger->appendLog(msg, logPanel::LOG_IN);
		}
	}

    // update the stored number of error codes
    this->lastErrorCount = result;

    // return the number of codes
	return result;
}

/// \brief Retrieve the error codes from the interface
///
/// A variable number of error codes may be returned.  Each code must
/// be decoded from the returned 'bytes'.  Will return an empty array
/// if the MIL status has not been checked first
///
/// \see obd_mil_status()
/// \return An array of strings with the error codes
wxArrayString obdbase::obd_mil_error_codes ()
{
    int toks[30];
    int byteOne;
    int byteTwo;
	wxString msg;
	wxArrayString errors;
	wxString currError;

	// bail out early if no error codes to be retrieved.
	if (this->lastErrorCount == 0) {
	    // write to log if necessary
        if (logger) {
            msg = _("No MIL Codes have been reported\n");
            logger->appendLog(msg, logPanel::LOG_ERROR);
        }
	} else {
        // write to log if necessary
        if (logger) {
            msg = _("Requesting MIL Codes\n");
            logger->appendLog(msg, logPanel::LOG_OUT);
        }

        if (this->obd_pid_get_raw(0x03, toks, sizeof(toks))) {

            // for each code, convert to string
            for (int i = 0; i < this->lastErrorCount; i++) {
                // make sure we have an empty string
                currError.Clear();

                // work out the first and second 'byte'
                int frame = i / 3;
                byteOne = toks[i+i+1+frame];
                byteTwo = toks[i+i+2+frame];

                // char 1
                // defined by first 2 bits of first byte
                int x = (byteOne & 0xC0) >> 6;

                // select the correct char based upon the value of
                // the first two bits
                switch (x) {
                    case 1:
                        currError = _T("C");
                        break;
                    case 2:
                        currError = _T("B");
                        break;
                    case 3:
                        currError = _T("U");
                        break;
                    default:
                        currError = _T("P");
                }

                // char 2
                // defined by 3rd & 4th bits of first byte
                currError += wxString::Format(_T("%d"), (byteOne & 0x30) >> 4);

                // char 3
                // defined by 5th, 6th, 7th, and 8th bit of first byte
                currError += wxString::Format(_T("%d"), byteOne & 0x0F);

                // chars 4 & 5
                // defined by second byte
                currError += wxString::Format(_T("%.2d"), byteTwo);

                // now we have a code, add string to array
                errors.Add(currError);
            }
        }
	}

    // return the array of errors
	return errors;
}

/// \brief Ask the ECU to clear the DTCs and switch off the MIL
///
/// The MIL (Malfunction Indicator Lamp) may be illuminated due to one
/// or more error codes which are stored by the ECU.  This function
/// asks the ECU to clear these codes and switch off the lamp.
///
/// \return True if the ECU has reponded and cleared the codes.
bool obdbase::obd_clear_dtc ()
{
	bool result = false;
	int toks[3];
	wxString msg;

	// write to log if necessary
	if (logger) {
		msg = _("Requesting clearing of DTC's\n");
		logger->appendLog(msg, logPanel::LOG_OUT);
	}

	// return -1 if we haven't got a well formed response
	if (this->obd_pid_get_raw(0x04, toks, sizeof(toks))) {
		result = true;

		// write to log if necessary
		if (logger) {
			msg = _("DTC's successfully cleared\n");
			logger->appendLog(msg, logPanel::LOG_IN);
		}
	} else {
		// write to log if necessary
		if (logger) {
			msg = _("Could not clear DTC's\n");
			logger->appendLog(msg, logPanel::LOG_ERROR);
		}
	}

	return result;
}

bool obdbase::obd_write(const wxString& command, int count)
{
	int len;
	char cstring[256] = "";
	bool retval = false;

	strncat(cstring, (const char*)command.mb_str(wxConvUTF8), command.Length());

	// end the command correctly with a CR
	len = strlen(cstring);
	cstring[len] = 0x0d;
	cstring[len+1] = 0x00;

	if (port->Write(cstring, strlen(cstring)) == (count + 1)) {
		retval = true;
	}

	return retval;
}

wxString obdbase::obd_read()
{
	wxString result;
	char * buff = NULL;
	char * p = (char *)">";
	int read;
	size_t size;

	read = port->ReadUntilEOS(buff, &size, p, 5000, 0);

	// strip out non printing chars
	for (int i = 0; i < sizeof(buff); i++) {
		if (iscntrl(buff[i]))
			buff[i] = 0x20;
	}

	// set up the return string
	result = wxString::From8BitData(buff);
	result = result.Strip(wxString::both);

	delete buff;

	return result;
}

void obdbase::obd_calculate_checksum ()
{
	//TODO Calculate checksum from tokens
}

bool obdbase::obd_validate_checksum ()
{
	//TODO Validate checksum against calculation
	return true;
}

void obdbase::obd_append_checksum ()
{
}

wxString obdbase::obd_identify_device()
{
    wxString result = wxT("Superclass for devices");
	return result;
}

wxString obdbase::obd_get_protocol()
{
	wxString result = wxT("Superclass for devices");
	return result;
}

bool obdbase::obd_pid_get_raw(int pid, int tokens[], int toksize)
{
	wxString raw;
	wxString msg;
	bool chkHead = false;
	bool chkSum = false;
	int scale;
	int index = 0;
	int expHead;
	int actHead;

	// convert pid to wxString
	scale = ((pid / 255) + 1) * 2;		// number of 'bytes' * 2
	wxString pidString = wxString::Format(_T("%0*x"), scale, pid);

	// send to device
	if (this->obd_write(pidString, pidString.length())) {

//		sleep(10);
//		usleep(20000);
		// read return from device
		raw = this->obd_read();

		// write the raw response to the log if needed
		if (logger && logExtra) {
			msg.Printf(_("Raw data: %s\n"), raw.c_str());
			logger->appendLog(msg, logPanel::LOG_IN);
		}

		// tokenise the string
		wxStringTokenizer tkz(raw, wxT(" "));
		while ( tkz.HasMoreTokens()  && index <= toksize )
		{
			wxString token = tkz.GetNextToken();
			token.ToLong((long int*)&tokens[index], 16);
			index++;
		}

		// check header
		scale = (scale / 2);			// redefine scale as no of 'bytes' expected
		expHead = pid + (64 * pow(256, (scale - 1)));
		actHead = 0;
		for (int i = 0; i < scale; i++) {
			actHead += pow(256, (scale - 1 - i)) * tokens[i];
		}
		if (actHead == expHead) {
			chkHead = true;
		}

		// check checksum
		if (useChecksum) {
			// TODO: Calculate and check the checksum
			chkSum = true;
		} else {
			chkSum = true;
		}
	}

	return (chkHead && chkSum);
}

/// \brief Get the value associated with a PID from the ECU
///
/// \param pid The pid you wish to enquire
/// \param result Pointer to receive the calculated value
/// \result True if a value has been successfully retreived
bool obdbase::obd_pid_value(int pid, obdbase::pidInfo* result)
{
    bool retVal = false;
    int toks[10];
    wxString msg;

    // set sensible defaults
    result->pid_flag = PID_FLAG_SINGLE;
    result->resultMain = 0;
    result->resultSecondary = 0;
    result->resultString.clear();

	// write to log if necessary
	if (logger) {
		msg.Printf(_("Requesting PID: %#.4x\n"), pid);
		logger->appendLog(msg, logPanel::LOG_OUT);
	}

    // ask for the raw data
    if (this->obd_pid_get_raw(pid, toks, sizeof(toks))) {
        // we have a good result so...
        retVal = true;

        switch (pid) {
            case PID_INTAKE_PRESS:
            case PID_VEHICLE_SPEED:
            case PID_ENGINE_CYCLES:
            case PID_BARO_PRESSURE:
                result->resultMain = toks[2];
                break;
            case PID_BANK1_STFT:
            case PID_BANK1_LTFT:
            case PID_BANK2_STFT:
            case PID_BANK2_LTFT:
                result->resultMain = (toks[2] - 128) * 100 / 128;
                break;
            case PID_COOLANT_TEMP:
            case PID_INTAKE_TEMP:
            case PID_AMB_AIR_TEMP:
                result->resultMain = toks[2] - 40;
                break;
            case PID_MIL_DIST:
            case PID_MIL_CLEAR_DIST:
            case PID_MIL_TIME:
            case PID_MIL_CLEAR_TIME:
            case PID_ENGINE_RUN_TIME:
                result->resultMain = (toks[2] * 256) + toks[3];
                break;
            case PID_FUEL_PRESS:
                result->resultMain = toks[2] * 3;
                break;
            case PID_TIMING_ADV:
                result->resultMain = (toks[2] / 2) - 64;
                break;
            case PID_MAF_RATE:
                result->resultMain = ((toks[2] * 256) + toks[3]) / 100;
                break;
            case PID_THROTTLE_POS:
            case PID_REL_THRO_POS:
            case PID_ABS_THRO_B:
            case PID_ABS_THRO_C:
            case PID_ACC_PEDAL_D:
            case PID_ACC_PEDAL_E:
            case PID_ACC_PEDAL_F:
            case PID_ETHANOL_FUEL_PC:
            case PID_ENGINE_LOAD:
                result->resultMain = toks[2] * 100 / 255;
                break;
            case PID_ENGINE_RPM:
                result->resultMain = ((toks[2] * 256) + toks[3]) / 4;
                break;
            case PID_O2_SEN_B1S1:
            case PID_O2_SEN_B1S2:
            case PID_O2_SEN_B1S3:
            case PID_O2_SEN_B1S4:
            case PID_O2_SEN_B2S1:
            case PID_O2_SEN_B2S2:
            case PID_O2_SEN_B2S3:
            case PID_O2_SEN_B2S4:
                result->resultMain = toks[2] * 0.005;
                result->resultSecondary = (toks[3] - 128) * 100 / 128;
                result->pid_flag = PID_FLAG_DOUBLE;
                break;
            case PID_CAT_TEMP_B1S1:
            case PID_CAT_TEMP_B2S1:
            case PID_CAT_TEMP_B1S2:
            case PID_CAT_TEMP_B2S2:
                result->resultMain = ((toks[2] * 256) + toks[3]) / 10 - 40;
                break;
            default:
                // bail out if we don't support the PID in this function
                retVal = false;
        }

        // write to log if necessary
        if (logger) {
            msg.Printf(_("Result for PID(%#.4x): %f\n"), pid, result->resultMain);
            logger->appendLog(msg, logPanel::LOG_IN);
        }
    } else {
		// write to log if necessary
		if (logger) {
			msg.Printf(_("Could not get result for PID: %#.4x\n"), pid);
			logger->appendLog(msg, logPanel::LOG_ERROR);
		}
	}

    if (this->useImperial) {
        switch (pid) {
            case PID_COOLANT_TEMP:
            case PID_INTAKE_TEMP:
            case PID_CAT_TEMP_B1S1:
            case PID_CAT_TEMP_B1S2:
            case PID_CAT_TEMP_B2S1:
            case PID_CAT_TEMP_B2S2:
            case PID_AMB_AIR_TEMP:
                result->resultMain = this->convert_c_to_f(result->resultMain);
                break;
            case PID_VEHICLE_SPEED:
            case PID_MIL_CLEAR_DIST:
            case PID_MIL_DIST:
                result->resultMain = this->convert_kph_to_mph(result->resultMain);
                break;
        }
    }

    return retVal;
}   // obd_pid_value()

/// \brief Get the Mode 0x01 PID. supported by the ECU
///
/// Not all ECUs support the whole range of Mode 0x01 PIDs.  This
/// function will parse the 3 PID which provide the full list of PIDS
/// supported by this particular ECU.
///
/// \param pids A vector to receive the supported PIDs.
void obdbase::obd_pid_supported_pids (std::vector<int>& pids)
{
    int toks1[10];
    int toks2[10];
    int toks3[10];
    std::vector<int> retVal;

    // if we still have a valid list of PIDS, return that instead
    // A vehicle doesn't suddenly support more whilst connected!
    if (pidList.empty()) {
        // get the first set of pids
        if (this->obd_pid_get_raw(PID_SUPPORTED1, toks1, sizeof(toks1))) {

            // bitmask to check the bit encoding of the returned 'bytes'
            unsigned int bitMask = 0x80000000;
            unsigned int indicator;
            // assemble the 'bytes' into a single int
            // will always be positive
            unsigned int pidEncoded = (toks1[2] * pow(256, 3)) + (toks1[3] * pow(256, 2)) + (toks1[4] * 256) + toks1[5];

            // process each bit
            for (int i = 0x01; i <= 0x20; i++) {
                // apply the bit mask
                indicator = pidEncoded & bitMask;
                // result will be 0, or > 0
                if (indicator > 0) {
                    retVal.push_back(256 + i);
                }
                // get the bitmask ready for the next loop
                bitMask = bitMask / 2;
            }
        }

        // get second set of pids if necessary
        if ((!retVal.empty()) &&
            (retVal.back() == 0x0120) &&
            this->obd_pid_get_raw(PID_SUPPORTED2, toks2, sizeof(toks2))) {

            // remove the last pid, which is get supported pids
            retVal.pop_back();
            unsigned int bitMask = 0x80000000;
            unsigned int indicator;
            unsigned int pidEncoded = (toks2[2] * pow(256, 3)) + (toks2[3] * pow(256, 2)) + (toks2[4] * 256) + toks2[5];

            for (int i = 0x21; i <= 0x40; i++) {
                indicator = pidEncoded & bitMask;
                if (indicator > 0) {
                    retVal.push_back(256 + i);
                }
                bitMask = bitMask / 2;
            }
        }

        // get the third set of pids if necessary
        if ((!retVal.empty()) &&
            (retVal.back() == 0x0140) &&
            this->obd_pid_get_raw(PID_SUPPORTED3, toks3, sizeof(toks3))) {

            // remove the last pid, which is get supported pids
            retVal.pop_back();
            unsigned int bitMask = 0x80000000;
            unsigned int indicator;
            unsigned int pidEncoded = (toks3[2] * pow(256, 3)) + (toks3[3] * pow(256, 2)) + (toks3[4] * 256) + toks3[5];

            for (int i = 0x41; i <= 0x60; i++) {
                indicator = pidEncoded & bitMask;
                if (indicator > 0) {
                    retVal.push_back(256 + i);
                }
                bitMask = bitMask / 2;
            }
        }

        // copy the retrieved list into the class variable
        pidList.swap(retVal);

    }

    pids.swap(pidList);
}   // obd_pid_supported_pids()

/// \brief Determine if the serial port is open
///
///
///
/// \return True if the serial port is open
bool obdbase::obd_is_connected ()
{
    bool retval;

    if (port->IsOpen() == 1) {
        retval = true;
    } else {
        retval = false;
    }

    return retval;
}

/// \brief Convert temperature from centigrade to farenheit
///
/// \param centigrade Temperature in degrees centigrade.
/// \return Temperature in degrees farenheit.
int obdbase::convert_c_to_f (int centigrade)
{
	return (9 / 5) * centigrade + 32;
}

/// \brief Convert kilometres to miles
///
/// \param kilometres Distance or speed in kilometres.
/// \return distance or speed in miles.
int obdbase::convert_kph_to_mph (int kph)
{
	return kph / 1.60934400061;
}
