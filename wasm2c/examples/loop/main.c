#include <stdio.h>
#include <stdlib.h>

#include "loop.h"

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
  w2c_loop loop;
  wasm2c_loop_instantiate(&loop, &host);
  wasm2c_loop_file_open();

  w2c_loop_loop(&loop);

  wasm2c_loop_print_map();
  wasm2c_loop_free(&loop);
  wasm2c_loop_file_close();

  wasm_rt_free();
  return 0;
}

u32 w2c_host_fill_buf(struct w2c_host* instance, u32 ptr, u32 size) {
  u32 count = size / 4;
  // change the seed 
  srand(time(0)); 

  for(uint32_t i = 0; i < count; i++) {
    *(uint32_t*)(&instance->memory.data[ptr + i * 4]) = rand() % 100;
  }
  return size;
}

void w2c_host_buf_done(struct w2c_host* instance, u32 ptr, u32 size) {
    printf("Buffer:\n");

    // Loop through the memory buffer to print Fibonacci numbers.
    for (u32 i = 0; i < size / 4; ++i) { // Each Fibonacci number is 4 bytes
        uint32_t num = *(uint32_t*)(&instance->memory.data[ptr + i * 4]);
        printf("Arr(%u) = %u\n", i, num);
    }

    printf("Buffer processing completed.\n");
}
