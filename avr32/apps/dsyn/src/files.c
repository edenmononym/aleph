// asf
#include "print_funcs.h"

// aleph-avr32
#include "app.h"
#include "bfin.h"
#include "flash.h"
#include "filesystem.h"
#include "render.h"

// lppr
#include "ctl.h"
#include "files.h"
#include "util.h"

#define LDR_PATH "/dsp/aleph-drumsyn.ldr"
#define CONFIG_PATH "/app/dsyn/dsyn_conf.txt"

// fread: no size arg
static void fake_fread(volatile u8* dst, u32 size, void* fp) {
  u32 n = 0;
  //  print_dbg("\r\n\r\n read: \r\n");
  while(n < size) {
    *dst = fl_fgetc(fp);
    /* print_dbg_ulong(n); */
    /* print_dbg(","); */
    /* print_dbg_hex( ((u32)dst) & 0xff ); */
    /* print_dbg(" \r\n"); */
    n++;
    dst++;
  }
}

u8 files_search_dsp(void) {
  void* fp;
  //  FL_DIR dirstat;
  u32 size;
  // open file pointer
  print_dbg("\r\n opening dsp file at path: ");
  print_dbg(LDR_PATH);

  // we don't want to be handling UI interrupts while working with the filesystem
  app_pause();

  fp = fl_fopen(LDR_PATH, "r");
  if( fp != NULL) {
    size = ((FL_FILE*)(fp))->filelength;
    print_dbg("\r\n opened file, size: ");
    print_dbg_ulong(size);
    print_dbg(" ; pointer: ");
    print_dbg_hex((u32)fp);

    render_status("loading sdcard -> RAM...     ");
    render_update();

    // read .ldr data to RAM buffer
    //    fl_fread((void*)bfinLdrData, 1, size, fp);
    fake_fread((void*)bfinLdrData, size, fp);

    fl_fclose(fp);

    print_dbg("\r\n finished reading .ldr file to RAM");
    bfinLdrSize = size;


    render_status("writing RAM -> flash...     ");
    render_update();

    // write buf to flash
    flash_write_ldr();
    print_dbg("\r\n finished writing .ldr file to flash");
    // reboot the DSP from RAM
    print_dbg("\r\n booting DSP from RAM");

    render_status("booting DSP from RAM...     ");
    render_update();

    bfin_load_buf();
    app_resume();

    return 1; // ok
  } else {
    print_dbg("\r\n null file pointer to .ldr");

    app_resume();
    return 0; // error opening
  }
}


// write parameter values to file
void files_write_params(void) {
  u32 i, j;
  void* fp;
  char str[32];
  app_pause();

  fp = fl_fopen(CONFIG_PATH, "w");

  for(i=0; i<DSYN_NVOICES; i++) {
    for(j=0; j<PARAM_VOICE_NPARAMS; j++) {
      // 8 character hex string per parameter
      uint_to_hex_ascii(str, ctl_get_voice_param(i, j));
      str[8] = '\0';
      fl_fputs(str, fp);
      // the following text should be ignored
      fl_fputs(" , ", fp);
      fl_fputs(paramStrings[i * PARAM_VOICE_NPARAMS + j], fp);
      // the newline is important
      fl_fputs("\r\n", fp);
    }
  }
  fl_fclose(fp);
}

// read parameter values from file
void files_read_params(void) {
  u32 i, j, k;
  u32 val;
  void* fp;
  char str[8];
  char ch;
  u8 eof = 0;

  app_pause();

  fp = fl_fopen(CONFIG_PATH, "r");
  if(fp == NULL) {
    print_dbg("\r\n dsyn config file was NULL");
    app_resume();
    return;
  }

  for(i=0; i<DSYN_NVOICES; i++) {
    if(eof) { break; }
    for(j=0; j<PARAM_VOICE_NPARAMS; j++) {      
      if(eof) { break; }
      // 8 character hex string -> param value
      for(k=0; k<8; k++) {
	str[k] = fl_fgetc(fp);
      }
      val = hex_ascii_to_uint(str);
      // set the parameter
      ctl_voice_param(i, j, val);
      // search for the next newline, 
      // ignore everything in between
      while(1) {
	ch = fl_fgetc(fp);
	if(ch == '\n') { break; }
	if(ch == EOF) { eof = 1; break; }
      }
    }
  }
  fl_fclose(fp);
  app_resume();
}
