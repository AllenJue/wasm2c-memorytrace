/* Entry point for the mmm example.
 *
 * This example shows how you can fulfill wasm module imports in your C
 * program, and access linear memory.
 *
 * The program reads matrices from the command line, and multiplies them,
 * e.g.:
 *
 * ```
 * $ mmm 2 1 1 1 1 2 1 1 1 1
 * 
 * A: 2x2 matrix with values: 1 1 1 1
 * B: 2x2 matrix with values: 1 1 1 1
 * 
 * ```
 *
 */
#include <stdio.h>
#include <stdlib.h>

#include "mmm.h"

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
  /* Make sure there is at least one command-line argument. */
  // if (argc < 2) {
  //   printf("Invalid argument. Expected '%s [number sequence e.g. '2 1 1 1 1 2 1 1 1 1']'\n", argv[0]);
  //   printf("Instead, got '%s'\n", argv[1]);
  //   return 1;
  // }
  clock_t start = clock();
  /* Initialize the Wasm runtime. */
  wasm_rt_init();

  /* Create a structure to store the memory and current string, allocating 1
    page of Wasm memory (64 KiB) that the mmm module instance will import. */
  struct w2c_host host;
  wasm_rt_allocate_memory(&host.memory, 1, 1, false);

  // Construct an instance of the `mmm` module, which imports from the host.
  w2c_mmm mmm;
  wasm2c_mmm_instantiate(&mmm, &host);
  wasm2c_mmm_file_open();
  
  // Move to next arg. Do this first, so the program name is skipped.
  // argc--;
  // argv++;
  // int* input = malloc(argc * sizeof(int));
  // for (int i = 0; i < argc; i++) {
  //   input[i] = atoi(argv[i]);
  // }

  // instead of inputting these manually, try to make it so that I can read directly from a file
  int input[20] = {3, 9383, 886, 2777, 6915, 7793, 8335, 5386, 492, 6649, 3, 1421, 2362, 27, 8690, 59, 7763, 3926, 540, 3426 };

  host.input = input;
  w2c_mmm_mmm(&mmm);
  // There should be the following checks:
  // 2*n^2 elements are rechecked n-1 times
  // n^2 + 3 elements are rechecked 0 times (n^2 for the result, plus 2 loads for A's size and B's size, and 1 store for C's size)
  // for input sizes of over 8 i think this fills up the map.
  wasm2c_mmm_print_map();

  /* Free the rot13 module. */
  wasm2c_mmm_free(&mmm);
  wasm2c_mmm_file_close();
  /* Free the Wasm runtime state. */
  wasm_rt_free();
  clock_t end = clock();
  double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("Time taken: %f seconds\n", time_taken);
  return 0;
}

/* Fill the wasm buffer with the input to be mmm'd.
 *
 * params:
 *   instance: An instance of the w2c_host structure
 *   ptr: The wasm memory address of the buffer to fill data.
 *   size: The size of the buffer in wasm memory.
 * result:
 *   The number of bytes filled into the buffer. (Must be <= size).
 */
u32 w2c_host_fill_buf(struct w2c_host* instance, u32 ptr, u32 size) {
  u32 count = size / 4; 

  // fill buffer with input
  for (size_t i = 0; i < count; i++) {
    memcpy(&instance->memory.data[ptr + i * 4], &instance->input[i], 4);
  }
  
  return size;
}

/* Called when the wasm buffer has been finished.
 *
 * params:
 *   w2c_host: An instance of the w2c_host structure
 *   ptr: The wasm memory address of the buffer.
 *   size: The size of the buffer in wasm memory.
 */
void w2c_host_buf_done(struct w2c_host* instance, u32 ptr, u32 size) {
  /* The output buffer is not necessarily null-terminated, so use the %*.s
   * printf format to limit the number of characters printed. */

  // print matrix A (4 bytes per int)
  u32 size_a = instance->memory.data[ptr];
  printf("A: %d x %d matrix with values: \n", size_a, size_a);
  for (size_t i = 0; i < size_a; i++) {
    for (size_t j = 0; j < size_a; j++) {
      // printf("%d ", instance->memory.data[i * size_a + j + 1]);
      // reconstruct the value since u32 is 4 bytes
      u32 val = 0;
      memcpy(&val, &instance->memory.data[(i * size_a + j + 1) * 4], 4);
      printf("%d ", val);
    }
    printf("\n");
  }
  // print matrix B
  u32 b_offs = ptr + 4 + size_a * size_a * 4;
  u32 size_b = 0;
  memcpy(&size_b, &instance->memory.data[b_offs], 4);
  printf("B: %d x %d matrix with values: \n", size_b, size_b);
  for (size_t i = 0; i < size_b; i++) {
    for (size_t j = 0; j < size_b; j++) {
      // printf("%d ", instance->memory.data[b_offs + i * size_b + j + 1]);
      // reconstruct the value since u32 is 4 bytes
      u32 val = 0;
      memcpy(&val, &instance->memory.data[b_offs + (i * size_a + j + 1) * 4], 4);
      printf("%d ", val);
    }
    printf("\n");
  }
  // print result
  u32 res_offs = b_offs + 4 + size_b * size_b * 4;
  u32 size_res = 0;
  memcpy(&size_res, &instance->memory.data[res_offs], 4);
  printf("Result: %d x %d matrix with values: \n", size_res, size_res);
  // return;
  for (size_t i = 0; i < size_res; i++) {
    for (size_t j = 0; j < size_res; j++) {
      // printf("%d ", instance->memory.data[res_offs + i * size_res + j + 1]);
      // reconstruct the value since u32 is 4 bytes
      u32 val = 0;
      memcpy(&val, &instance->memory.data[res_offs + (i * size_a + j + 1) * 4], 4);
      printf("%d ", val);
    }
    printf("\n");
  }
}

/* Print a value from wasm.
 *
 * params:
 *   w2c_host: An instance of the w2c_host structure
 *   val: The value to print.
 */
void w2c_host_printval(struct w2c_host* instance, u32 val) {
  printf("value: %d\n", val);
}

/* Print an error due to mismatched matrix sizes.
 *
 * params:
 *   w2c_host: An instance of the w2c_host structure
 *   size_a: The size of Matrix A
 *   size_b: The size of Matrix B
*/
void w2c_host_error(struct w2c_host* instance, u32 size_a, u32 size_b) {
  printf("Matrix A and B are not the same size. A: %d, B: %d\n", size_a, size_b);
}
