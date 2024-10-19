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
  if (argc < 2) {
    printf("Invalid argument. Expected '%s [number sequence e.g. '2 1 1 1 1 2 1 1 1 1']'\n", argv[0]);
    return 1;
  }
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
  argc--;
  argv++;
  printf("filling buffer\n");
  int* input = malloc(argc * sizeof(int));
  for (int i = 0; i < argc; i++) {
    input[i] = atoi(argv[i]);
    printf("%d ", input[i]);
  }
  host.input = input;
  printf("filling buffer done\n\n");
  w2c_mmm_mmm(&mmm);
  printf("done\n");

  /* Free the rot13 module. */
  wasm2c_mmm_free(&mmm);
  wasm2c_mmm_file_close();
  /* Free the Wasm runtime state. */
  wasm_rt_free();

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
  for (size_t i = 0; i < size; ++i) {
    if (instance->input[i] == 0) {
      return i;
    }
    instance->memory.data[ptr + i] = instance->input[i];
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

  // print matrix A
  u32 size_a = instance->memory.data[0];
  printf("A: %dx%d matrix with values: \n", size_a, size_a);
  for (size_t i = 0; i < size_a; i++) {
    for (size_t j = 0; j < size_a; j++) {
      printf("%d ", instance->memory.data[i * size_a + j + 1]);
    }
    printf("\n");
  }
  // print matrix B
  u32 b_offs = size_a * size_a + 1;
  u32 size_b = instance->memory.data[b_offs];
  printf("B: %dx%d matrix with values: \n", size_b, size_b);
  for (size_t i = 0; i < size_b; i++) {
    for (size_t j = 0; j < size_b; j++) {
      printf("%d ", instance->memory.data[b_offs + i * size_b + j + 1]);
    }
    printf("\n");
  }
  // print result
  u32 res_offs = b_offs + 1 + size_b * size_b;
  u32 size_res = instance->memory.data[res_offs];
  printf("Result: %dx%d matrix with values: \n", size_res, size_res);
  for (size_t i = 0; i < size_res; i++) {
    for (size_t j = 0; j < size_res; j++) {
      printf("%d ", instance->memory.data[res_offs + i * size_res + j + 1]);
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
  printf("%d\n", val);
}

/* Return the size of Matrix A from the wasm module. For simplicity,
 * this is the first value in the input buffer as we assume 
 * the matrices are always square.
 *
 * params:
 *   w2c_host: An instance of the w2c_host structure
 * result:
 *   The size of Matrix A.
 */
u32 w2c_host_get_size_a(struct w2c_host* instance) {
  return instance->input[0];
}

/* Return the size of Matrix B from the wasm module.
 * B's size comes after all the values of A.
 *
 * params:
 *   w2c_host: An instance of the w2c_host structure
 * result:
 *   The size of Matrix B.
 */
u32 w2c_host_get_size_b(struct w2c_host* instance) {
  return instance->input[instance->input[0] * instance->input[0] + 1];
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