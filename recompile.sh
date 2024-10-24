bin/wat2wasm wasm2c/examples/mmult/mmm.wat -o wasm2c/examples/mmult/mmm.wasm
bin/wasm2c wasm2c/examples/mmult/mmm.wasm -o wasm2c/examples/mmult/mmm.c -t
cc -o mmm wasm2c/examples/mmult/main.c wasm2c/examples/mmult/mmm.c  wasm2c/wasm-rt-impl.c wasm2c/wasm-rt-mem-impl.c -Iwasm2c -lm -pthread -DWASM_RT_USE_MMAP=0
