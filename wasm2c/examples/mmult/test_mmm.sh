# a script to verify that I actually do the matrix multiplications correctly.
# A stress test of sorts - don't actually use inputs of over 50 numbers. (lol)

# call the recompile script
./recompile.sh

# compile the matrix test generator
g++ wasm2c/examples/mmult/gen_mat.cpp -o wasm2c/examples/mmult/gen_mat

# now run this in a loop .;.
input_sizes=(2 3 5 10 25 50 )
for i in "${input_sizes[@]}"; do
  # run the gen_mat binary to actually get matrices
  ./wasm2c/examples/mmult/gen_mat $i

  # run the mmm binary with the inputted matrices
  xargs -a input.txt ./mmm > out.txt

  # now, call the validator
  python3 wasm2c/examples/mmult/verify_solution.py out.txt

  echo "Done with $i"
done;
