#include <stdint.h>
#define SRAM_ORIGIN 0x20000000U   // start of SRAM stack
#define SRAM_LENGTH (1024U * 96U) // 96Kb
#define SRAM_END_ADDR ((SRAM_ORIGIN) + (SRAM_LENGTH))
#define INITIAL_SP (SRAM_END_ADDR)

// Exceptions Called From Vector Table
void reset_handler(void);
void default_handler(void) {
  while (1)
    ;
}

// exception handlers for vector table
// alias -> makes the symbol (other handlers) point to another symbol in memory
//  (i.e. default_handler)
// __weak__ = symbol can be overriden by another definition in the linker
void nmi_handler(void) __attribute__((weak, alias("default_handler")));
void hard_fault_handler(void) __attribute__((weak, alias("default_handler")));
void mem_management_fault_handler(void) __attribute__((weak, alias("default_handler")));
void bus_fault_handler(void) __attribute__((weak, alias("default_handler")));
void usage_fault_handler(void) __attribute__((weak, alias("default_handler")));
void svcall_handler(void) __attribute__((weak, alias("default_handler")));
void debug_monitor_handler(void) __attribute__((weak, alias("default_handler")));
void pendsv_handler(void) __attribute__((weak, alias("default_handler")));
void systick(void) __attribute__((weak, alias("default_handler")));
void irq_handler(void) __attribute__((weak, alias("default_handler")));

// Vector Table
uint32_t isr_vector[] __attribute__((section(".isr_vector"))) = {
    INITIAL_SP,
    (uint32_t)&reset_handler,
    (uint32_t)&nmi_handler,
    (uint32_t)&mem_management_fault_handler,
    (uint32_t)&bus_fault_handler,
    (uint32_t)&usage_fault_handler,
    0,
    0,
    0,
    0,
    (uint32_t)&svcall_handler,
    (uint32_t)&debug_monitor_handler,
    0,
    (uint32_t)&pendsv_handler,
    (uint32_t)&systick,
    (uint32_t)&irq_handler
};

void main(void);

// declare symbols from sections in linker
extern uint32_t _etext, _sdata, _edata, _sbss, _ebss;
void reset_handler(void) {

  // copy data to sram
  uint32_t data_size = (uint32_t)&_edata - (uint32_t)&_sdata;
  uint8_t *sram_data = (uint8_t *)&_sdata;
  uint8_t *flash_data = (uint8_t *)&_etext;
  for (uint32_t i = 0; i < data_size; i++) {
    sram_data[i] = flash_data[i];
  }

  // clear out bss
  uint32_t bss_size = (uint32_t)&_ebss - (uint32_t)&_sbss;
  uint8_t *bss_data = (uint8_t *)&_sbss;
  for (uint32_t i = 0; i < bss_size; i++) {
    bss_data[i] = 0;
  }

  // run main
  main();
}