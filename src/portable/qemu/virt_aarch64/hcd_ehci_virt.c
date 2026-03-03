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
#include <sddf/util/cache.h>

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

#define STRICT_ALIGN __attribute__((target("strict-align")))

// Writes values from the cache back into memory but keep a copy in the cache.
STRICT_ALIGN void data_clean(volatile void* starting_address, size_t size) {
    unsigned long start = (unsigned long) starting_address;
    unsigned long end = (unsigned long) starting_address + size;
    cache_clean(start, end);
}

// Writes values from the cache back into memory and remove it from the cache.
STRICT_ALIGN void data_clean_and_invalidate(volatile void* starting_address, size_t size) {
    unsigned long start = (unsigned long) starting_address;
    unsigned long end = (unsigned long) starting_address + size;
    cache_clean_and_invalidate(start, end);
}

// Remove values from the cache because the value in memory may have changed.
STRICT_ALIGN void data_invalidate(volatile void* starting_address, size_t size) {
    unsigned long start = (unsigned long) starting_address;
    unsigned long end = (unsigned long) starting_address + size;
    cache_clean_and_invalidate(start, end);
}

bool hcd_dcache_clean(const void* addr, uint32_t data_size) {
    TU_LOG3("EHCI BOARD: clean 0x%p\n", addr);
  data_clean((volatile void *) addr, data_size);
  return true;
}

bool hcd_dcache_invalidate(const void* addr, uint32_t data_size) {
    TU_LOG3("EHCI BOARD: invalidate 0x%p\n", addr);
  data_invalidate((volatile void *) addr, data_size);
  return true;
}

bool hcd_dcache_clean_invalidate(const void* addr, uint32_t data_size) {
    TU_LOG3("EHCI BOARD: clean and invalidate 0x%p\n", addr);
  data_clean_and_invalidate((volatile  void*) addr, data_size);
  return false;
}

#endif