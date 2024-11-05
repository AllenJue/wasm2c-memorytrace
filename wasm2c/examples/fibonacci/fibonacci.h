/* Automatically generated by wasm2c */
#ifndef FIBONACCI_H_GENERATED_
#define FIBONACCI_H_GENERATED_

#include "wasm-rt.h"

#include <stdint.h>
#include <uthash.h>

#ifndef WASM_RT_CORE_TYPES_DEFINED
#define WASM_RT_CORE_TYPES_DEFINED
typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;
typedef float f32;
typedef double f64;
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct w2c_host;
extern wasm_rt_memory_t* w2c_host_mem(struct w2c_host*);

typedef struct w2c_fibonacci {
  struct w2c_host* w2c_host_instance;
  /* import: 'host' 'mem' */
  wasm_rt_memory_t *w2c_host_mem;
} w2c_fibonacci;

void wasm2c_fibonacci_instantiate(w2c_fibonacci*, struct w2c_host*);
void wasm2c_fibonacci_free(w2c_fibonacci*);
void wasm2c_fibonacci_file_open();
void wasm2c_fibonacci_file_close();
void wasm2c_fibonacci_load_instrumentation(w2c_fibonacci*, uint32_t);
void wasm2c_fibonacci_store_instrumentation(w2c_fibonacci*, uint32_t);
typedef struct MemoryInfo{
  void *key;
  size_t bounds;
  bool dirty;
  size_t clean_rechecks;
  UT_hash_handle hh; /* makes this structure hashable */
} MemoryInfo;
// Memory Info Func Decls
void wasm2c_fibonacci_map_insert(MemoryInfo *memInfo);
MemoryInfo *wasm2c_fibonacci_map_find(void *key);
void wasm2c_fibonacci_print_map();

wasm_rt_func_type_t wasm2c_fibonacci_get_func_type(uint32_t param_count, uint32_t result_count, ...);

/* import: 'host' 'buf_done' */
void w2c_host_buf_done(struct w2c_host*, u32, u32);

/* import: 'host' 'fill_buf' */
u32 w2c_host_fill_buf(struct w2c_host*, u32, u32);

extern const u64 wasm2c_fibonacci_min_host_mem;
extern const u64 wasm2c_fibonacci_max_host_mem;
extern const u8 wasm2c_fibonacci_is64_host_mem;

/* export: 'fibonacci_loop' */
void w2c_fibonacci_fibonacci_loop(w2c_fibonacci*, u32);

#ifdef __cplusplus
}
#endif

#endif  /* FIBONACCI_H_GENERATED_ */
