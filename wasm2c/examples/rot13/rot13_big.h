/* Automatically generated by wasm2c */
#ifndef ROT13_BIG_H_GENERATED_
#define ROT13_BIG_H_GENERATED_

#include "wasm-rt.h"

#include <stdint.h>

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

typedef struct w2c_rot13__big {
  struct w2c_host* w2c_host_instance;
  /* import: 'host' 'mem' */
  wasm_rt_memory_t *w2c_host_mem;
} w2c_rot13__big;

void wasm2c_rot13__big_instantiate(w2c_rot13__big*, struct w2c_host*);
void wasm2c_rot13__big_free(w2c_rot13__big*);
wasm_rt_func_type_t wasm2c_rot13__big_get_func_type(uint32_t param_count, uint32_t result_count, ...);

/* import: 'host' 'buf_done' */
void w2c_host_buf_done(struct w2c_host*, u32, u32);

/* import: 'host' 'fill_buf' */
u32 w2c_host_fill_buf(struct w2c_host*, u32, u32);

extern const u64 wasm2c_rot13__big_min_host_mem;
extern const u64 wasm2c_rot13__big_max_host_mem;
extern const u8 wasm2c_rot13__big_is64_host_mem;

/* export: 'rot13' */
void w2c_rot13__big_rot13(w2c_rot13__big*);

#ifdef __cplusplus
}
#endif

#endif  /* ROT13_BIG_H_GENERATED_ */
