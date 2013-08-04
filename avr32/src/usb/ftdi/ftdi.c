/*
  ftdi.c
  aleph-avr32

  FTDI usb functions.
*/

// asf
#include "delay.h"
#include "print_funcs.h"
// aleph
#include "conf_usb_host.h"
#include "events.h"
#include "event_types.h"
#include "ftdi.h"
#include "global.h"
#include "monome.h"
#include "uhi_ftdi.h"

//---- defines



//---- extern vars
u8 ftdiPlug = 0;

//----- static vars
static volatile u8 rxBuf[FTDI_IN_BUF_SIZE];
static u32 rxBytes = 0;
static u8 rxBusy = 0;
static u8 txBusy = 0;
static uhd_trans_status_t status = 0;
static event_t e;

//------- static functions

static void ftdi_rx_done(
			       usb_add_t add,
			       usb_ep_t ep,
			       uhd_trans_status_t stat,
			       iram_size_t nb) {
  status = stat;
  rxBusy = 0;
  rxBytes = nb - FTDI_STATUS_BYTES;
  if(rxBytes) {
    // check for monome events
    (*monome_read_serial)();
    ///... TODO: other protocols
  } 
}

static void ftdi_tx_done(
			       usb_add_t add,
			       usb_ep_t ep,
			       uhd_trans_status_t stat,
			       iram_size_t nb) {
  status = stat;
  txBusy = 0;
  if (status != UHD_TRANS_NOERROR) {
    print_dbg("\r\n ftdi tx error");
    return;
  }
  
}

//-------- extern functions
void ftdi_write(u8* data, u32 bytes) {
  txBusy = 1;
  if(!uhi_ftdi_out_run(data,
		       bytes,
		       &ftdi_tx_done)) {

    print_dbg("\r\n error requesting ftdi output pipe");
  }
}
    
void ftdi_read(void) {
  rxBytes = 0;
  rxBusy = true;
  if (!uhi_ftdi_in_run((u8*)rxBuf,
		       FTDI_IN_BUF_SIZE, &ftdi_rx_done)) {
    print_dbg("\r\n ftdi rx transfer error");
    //    return 0;
  }
  //  while (ftdiRxBusy) { ;; }
  /* if (status != UHD_TRANS_NOERROR) { */
  /*   print_dbg("\r\n ftdi input transfer error, exiting poll function"); */
  /*   return 0; */
  //  }
  //    print_arr((u32)ftdi_in_buf, FTDI_IN_BUF_SIZE);
//  return rxBytes - FTDI_RX_BUF_OFFSET;
}


// respond to connection or disconnection of ftdi device.
// may be called from an interrupt
void ftdi_change(uhc_device_t* dev, u8 plug) {
  print_dbg("\r\n changed FTDI connection status");
  if(plug) { 
    e.eventType = kEventFtdiConnect; 
  } else {
    e.eventType = kEventFtdiDisconnect;
  }
  // posting an event so the main loop can respond
  post_event(&e); 
}

// setup new device connection
void ftdi_setup(void) {
  char * manstr;
  char * prodstr;
  char * serstr;
  print_dbg("\r\n FTDI setup routine");

  // get string data...
  ftdi_get_strings(&manstr, &prodstr, &serstr);
  
  // print the strings
  // print_unicode_string(manstr, FTDI_STRING_MAX_LEN);
  //  print_unicode_string(prodstr, FTDI_STRING_MAX_LEN);
  //  print_unicode_string(serstr, FTDI_STRING_MAX_LEN);

  check_monome_device_desc(manstr, prodstr, serstr);

  // set connection flag
  ftdiPlug = 1;
}


// rx buffer (no status bytes)
extern u8* ftdi_rx_buf() {
  return rxBuf + 2;
}

// number of bytes from last rx trasnfer
extern u8 ftdi_rx_bytes() {
  return rxBytes;
}

// busy flags
extern u8 ftdi_rx_busy() {
  return rxBusy;
}

extern u8 ftdi_tx_busy() {
  return txBusy;
}

// device plugged flag
extern u8 ftdi_plug() {
  return ftdiPlug;
}
