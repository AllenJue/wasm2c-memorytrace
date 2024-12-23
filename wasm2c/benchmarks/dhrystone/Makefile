WABT_ROOT=../../..
CC=clang
CFLAGS=-I$(WABT_ROOT)/wasm2c -I $(WABT_ROOT)/third_party/uvwasi/include/ -I $(WABT_ROOT)/third_party/uthash/src -O3
CFLAGS_SEGUE=-DWASM_RT_ALLOW_SEGUE=1 -mfsgsbase
LDFLAGS=-L$(WABT_ROOT)/third_party/uvwasi/build -L$(WABT_ROOT)/third_party/libuv/build -O3
LDLIBS=$(WABT_ROOT)/third_party/uvwasi/build/libuvwasi_a.a $(WABT_ROOT)/third_party/libuv/build/libuv.a -lm -lpthread -ldl -DWASM_RT_USE_MMAP=0


all: benchmark

clean:
	rm -rf dhrystone dhrystone.wasm dhrystone.c dhrystone.h

dhrystone.wasm: src/dhry_1.c src/dhry_2.c
	$(WABT_ROOT)/wasi-sdk-20.0/bin/clang -O3 $^ -o $@

dhrystone.c: dhrystone.wasm $(WABT_ROOT)/bin/wasm2c
	$(WABT_ROOT)/bin/wasm2c $< -o $@ --disable-simd -t

dhrystone_native: src/dhry_1.c src/dhry_2.c
	clang -O3 $^ -o $@

dhrystone: main.c dhrystone.c $(WABT_ROOT)/wasm2c/wasm-rt-impl.c $(WABT_ROOT)/wasm2c/wasm-rt-mem-impl.c 
	$(CC) $(LDFLAGS) $(CFLAGS) $^ -o $@ $(LDLIBS)

dhrystone_segue: main.c dhrystone.c $(WABT_ROOT)/wasm2c/wasm-rt-impl.c $(WABT_ROOT)/wasm2c/wasm-rt-mem-impl.c
	$(CC) $(LDFLAGS) $(CFLAGS) $(CFLAGS_SEGUE) $^ -o $@ $(LDLIBS)

benchmark: dhrystone_native dhrystone dhrystone_segue
	@echo "Starting Dhrystone benchmark. (Smaller number is better)"
	@sleep 2
	@echo "Native"
	@./dhrystone_native | grep "one run through Dhrystone"
	@sleep 2
	@echo "Wasm"
	@./dhrystone | grep "one run through Dhrystone"
	@sleep 2
	@echo "Wasm+Segue"
	@./dhrystone_segue | grep "one run through Dhrystone"
