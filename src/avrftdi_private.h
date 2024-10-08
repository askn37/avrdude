#pragma once
#include <ac_cfg.h>

#include <stdint.h>

#if defined(HAVE_LIBFTDI1) && defined(HAVE_LIBUSB_1_0)

#if defined(HAVE_LIBUSB_1_0_LIBUSB_H)
#include <libusb-1.0/libusb.h>
#else
#include <libusb.h>
#endif

#include <libftdi1/ftdi.h>
#undef HAVE_LIBFTDI_TYPE_232H
#define HAVE_LIBFTDI_TYPE_232H 1
#elif defined(HAVE_LIBFTDI)
#include <ftdi.h>
#else

#ifdef _MSC_VER
#pragma message("No libftdi or libusb support. Install libftdi1/libusb-1.0 or libftdi/libusb and run configure/make again.")
#else
#warning No libftdi or libusb support. Install libftdi1/libusb-1.0 or libftdi/libusb and run configure/make again.
#endif

#define DO_NOT_BUILD_AVRFTDI
#endif

#ifndef DO_NOT_BUILD_AVRFTDI
#define E(x, ftdi)                                             \
  do {                                                         \
    if((x)) {                                                  \
      pmsg_error("%s: %s (%d)\n", #x, strerror(errno), errno); \
      imsg_error("%s\n", ftdi_get_error_string(ftdi));         \
      return -1;                                               \
    }                                                          \
  } while(0)

#define E_VOID(x, ftdi)                                        \
  do {                                                         \
    if((x)) {                                                  \
      pmsg_error("%s: %s (%d)\n", #x, strerror(errno), errno); \
      imsg_error("%s\n", ftdi_get_error_string(ftdi));         \
    }                                                          \
  } while(0)

enum {
  JTAG_IR_EXTEST = 0,
  JTAG_IR_IDCODE = 1,
  JTAG_IR_SAMPLE_PRELOAD = 2,
  JTAG_IR_PROG_ENABLE = 4,
  JTAG_IR_PROG_COMMANDS = 5,
  JTAG_IR_PROG_PAGELOAD = 6,
  JTAG_IR_PROG_PAGEREAD = 7,
  JTAG_IR_PRIVATE0 = 8,
  JTAG_IR_PRIVATE1 = 9,
  JTAG_IR_PRIVATE2 = 10,
  JTAG_IR_PRIVATE3 = 11,
  JTAG_IR_AVR_RESET = 12,
  JTAG_IR_BYPASS = 15,
};

enum jtag_cmd {
  JTAG_DR_PROG_NONE = 0x00,
  JTAG_DR_PROG_FLASH_READ = 0x02,
  JTAG_DR_PROG_EEPROM_READ = 0x03,
  JTAG_DR_PROG_FUSE_READ = 0x04,
  JTAG_DR_PROG_SIGCAL_READ = 0x08,
  JTAG_DR_PROG_FLASH_WRITE = 0x10,
  JTAG_DR_PROG_EEPROM_WRITE = 0x11,
  JTAG_DR_PROG_LOCK_WRITE = 0x20,
  JTAG_DR_PROG_FUSE_WRITE = 0x40,
};

#define to_pdata(pgm) \
  ((Avrftdi_data *)((pgm)->cookie))

typedef struct avrftdi_s {
  // Pointer to struct maintained by libftdi to identify the device
  struct ftdi_context *ftdic;
  // Bitmask of values for pins; bit 0 represents pin 0 ([A|B]DBUS0)
  uint16_t pin_value;
  // Bitmask of pin direction; 1 makes a pin an output; bit 0 corresponds to pin 0
  uint16_t pin_direction;
  // Don't know; not useful? Someone put it in
  uint16_t led_mask;
  // Total number of pins supported by a programmerl varies with FTDI chips
  int pin_limit;
  // Internal RX buffer of the device; needed for INOUT transfers
  int rx_buffer_size;
  int tx_buffer_size;
  // Use bitbanging instead of MPSSE SPI
  bool use_bitbanging;
  // Bits 16-23 of extended 24-bit word flash address for parts with flash > 128k
  uint8_t lext_byte;

  char name_str[128];           // Used in ftdi_pin_name()
  struct pindef valid_pins;     // Used in avrftdi_check_pins_bb()
  struct pindef mpsse_pins[4];  // Used in avrftdi_check_pins_mpsse()
  struct pindef other_pins;     // Used in avrftdi_check_pins_mpsse()
} Avrftdi_data;
#endif                          // Do_not_build_avrfdti
