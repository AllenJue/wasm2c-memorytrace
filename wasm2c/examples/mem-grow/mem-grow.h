/* Automatically generated by wasm2c */
#ifndef MEM_GROW_H_GENERATED_
#define MEM_GROW_H_GENERATED_

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

struct w2c_console;

typedef struct w2c_mem0x2Dgrow {
  struct w2c_console* w2c_console_instance;
  wasm_rt_memory_t w2c_M0;//WriteMemory

} w2c_mem0x2Dgrow;

void wasm2c_mem0x2Dgrow_instantiate(w2c_mem0x2Dgrow*, struct w2c_console*);
void wasm2c_mem0x2Dgrow_free(w2c_mem0x2Dgrow*);
void wasm2c_mem0x2Dgrow_file_open();
void wasm2c_mem0x2Dgrow_file_close();
typedef struct CalleeNode{
  char *callee;
  struct CalleeNode *next;
} CalleeNode;

typedef struct FunctionNode{
  char *caller;
  CalleeNode *callees;
  UT_hash_handle hh;
} FunctionNode;

extern FunctionNode* graph;

void wasm2c_mem0x2Dgrow_add_callee(FunctionNode**, const char*, const char*);void wasm2c_mem0x2Dgrow_parse_line(FunctionNode**, char*);void wasm2c_mem0x2Dgrow_free_graph(FunctionNode*);void wasm2c_mem0x2Dgrow_print_graph(FunctionNode*);bool wasm2c_mem0x2Dgrow_contains_edge(const char *, const char *);
typedef struct StackNode {
  const char *caller;
  const char *prev_caller;
  void *key;
  struct StackNode *next;
} StackNode;

typedef struct Stack {
  StackNode *top;
} Stack;

extern struct Stack *call_stack;
Stack *wasm2c_mem0x2Dgrow_create_stack();
void wasm2c_mem0x2Dgrow_push_stack(Stack *, const char *, const char *, void *);
StackNode *wasm2c_mem0x2Dgrow_pop_stack(Stack *);
void wasm2c_mem0x2Dgrow_free_stack_node(StackNode *);
void wasm2c_mem0x2Dgrow_destroy_stack(Stack *);
void wasm2c_mem0x2Dgrow_print_stack(Stack *);
wasm_rt_memory_t* wasm2c_mem0x2Dgrow(w2c_mem0x2Dgrow* instance);
void wasm2c_mem0x2Dgrow_mem_instrumentation(w2c_mem0x2Dgrow*, u64, const char *);
typedef struct MemoryInfo{
  void *key;
  size_t clean_rechecks;
  const char *last_verified;
  UT_hash_handle hh; /* makes this structure hashable */
} MemoryInfo;

// Memory Info Func Decls
void wasm2c_mem0x2Dgrow_map_insert(MemoryInfo *memInfo);
void wasm2c_mem0x2Dgrow_free_info_map();
MemoryInfo *wasm2c_mem0x2Dgrow_map_find(void *key);
void wasm2c_mem0x2Dgrow_print_map();

wasm_rt_func_type_t wasm2c_mem0x2Dgrow_get_func_type(uint32_t param_count, uint32_t result_count, ...);

/* import: 'console' 'log' */
void w2c_console_log(struct w2c_console*, u32);

#ifdef __cplusplus
}
#endif

#endif  /* MEM_GROW_H_GENERATED_ */