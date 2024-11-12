# a script to verify that I actually do the matrix multiplications correctly.
# A stress test of sorts - don't actually use inputs of over 50 numbers. (lol)

# go into the main directory
cd ../../..

pwd

# call the recompile script
./recompile.sh
echo "Done recompiling"

# compile the matrix test generator
g++ wasm2c/examples/mmult/gen_mat.cpp -o wasm2c/examples/mmult/gen_mat
echo "Done compiling test inputs"

# clear output and time files
> wasm2c/examples/mmult/mmm_out.txt
> wasm2c/examples/mmult/mmm_times.txt

# now run this in a loop .;.
input_sizes=(2 4 8 16 32 64 128 256 512 1024 )
for i in "${input_sizes[@]}"; do
  # run the gen_mat binary to actually get matrices
  ./wasm2c/examples/mmult/gen_mat $i

  # run the mmm binary with the inputted matrices
  for j in $(seq 1 10); do
    ./mmm > wasm2c/examples/mmult/mmm_out.txt 2>> wasm2c/examples/mmult/mmm_times.txt
  done

  echo "" >> wasm2c/examples/mmult/mmm_times.txt

  echo "Done with $i"
done;

# clear mmm_out.txt (way too big)
> wasm2c/examples/mmult/mmm_out.txt