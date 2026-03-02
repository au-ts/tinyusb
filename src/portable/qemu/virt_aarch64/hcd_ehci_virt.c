/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This file is part of the TinyUSB stack.
 */

#include "tusb_option.h"

/* TODO: this is a bit dodgy, probably breaks their build system */
#if CFG_TUH_ENABLED && defined(TUP_USBIP_EHCI)

#include "host/hcd.h"
#include "portable/ehci/ehci_api.h"

#include <sddf/timer/client.h>

//--------------------------------------------------------------------+
// Controller API
//--------------------------------------------------------------------+

#define EHCI_REGS 0x30000000

// Initialize controller to host mode
bool hcd_init(uint8_t rhport, const tusb_rhport_init_t* rh_init) {
  (void) rh_init;

  /* TODO: could reset the controller here? */

  uint8_t caplength = *(uint8_t*)(EHCI_REGS + 0);
  return ehci_init(rhport, EHCI_REGS, EHCI_REGS + caplength);
}

// Enable USB interrupt
void hcd_int_enable (uint8_t rhport) {
  (void) rhport;
}

// Disable USB interrupt
void hcd_int_disable(uint8_t rhport) {
  (void) rhport;
}

void board_init(void)
{
}

int board_uart_write(void const *buf, int len)
{
    microkit_dbg_puts((const char *) buf);
    return len;
}

int board_uart_read(uint8_t *buf, int len)
{
    return 0;
}


#endif