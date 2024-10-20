#include <stdio.h>
#include <stdlib.h>

#include "if.0.h"

/* Define structure to hold the imports */
struct w2c_host {
  wasm_rt_memory_t memory;
  int* input;
};

/* Accessor to access the memory member of the host */
wasm_rt_memory_t* w2c_host_mem(struct w2c_host* instance) {
  return &instance->memory;
}

int main(int argc, char** argv) {
    /* Initialize the Wasm runtime. */
  wasm_rt_init();

   /* Create a structure to store the memory and current string, allocating 1
     page of Wasm memory (64 KiB) that the rot13 module instance will import. */
  struct w2c_host host;
  wasm_rt_allocate_memory(&host.memory, 1, 1, false);


  // Construct an instance of the `rot13` module, which imports from the host.
  w2c_if0x2E0 if0x2E0;
  wasm2c_if0x2E0_instantiate(&if0x2E0, &host);
  wasm2c_if0x2E0_file_open();

  printf("%d\n", w2c_if0x2E0_as0x2Dlocal0x2Eset0x2Dvalue(&if0x2E0, 100));

  wasm2c_if0x2E0_print_map();
  wasm2c_if0x2E0_free(&if0x2E0);
  wasm2c_if0x2E0_file_close();

  wasm_rt_free();
  return 0;
}

/* Fill the wasm buffer with Fibonacci numbers.
 *
 * params:
 *   instance: An instance of the w2c_host structure
 *   ptr: The wasm memory address of the buffer to fill data.
 *   size: The size of the buffer in wasm memory (in bytes).
 * result:
 *   The number of bytes filled into the buffer. (Must be <= size).
 */
u32 w2c_host_fill_buf(struct w2c_host* instance, u32 ptr, u32 size) {
    u32 count = size / 4; 

    // Initialize the first two Fibonacci numbers
    *(uint32_t*)(&instance->memory.data[ptr]) = 0; // Fib(0)
    if (count > 1) {
        *(uint32_t*)(&instance->memory.data[ptr + 4]) = 1; // Fib(1)
    }

    // printf("Buffer before\n");
    // w2c_host_buf_done(instance, ptr, size);
    return size; // Total bytes filled
}

/* Called when the wasm buffer has been fibonaccid'd.
 *
 * params:
 *   w2c_host: An instance of the w2c_host structure
 *   ptr: The wasm memory address of the buffer.
 *   size: The size of the buffer in wasm memory (in bytes).
 */
void w2c_host_buf_done(struct w2c_host* instance, u32 ptr, u32 size) {
    printf("Fibonacci Sequence:\n");

    // Loop through the memory buffer to print Fibonacci numbers.
    for (u32 i = 0; i < size / 4; ++i) { // Each Fibonacci number is 4 bytes
        uint32_t fib_number = *(uint32_t*)(&instance->memory.data[ptr + i * 4]);
        printf("Fib(%u) = %u\n", i, fib_number);
    }

    printf("Buffer processing completed.\n");
}
