(import "host" "mem" (memory $mem 1))
(import "host" "fill_buf" (func $fill_buf (param i32 i32) (result i32)))
(import "host" "buf_done" (func $buf_done (param i32 i32)))

;; debugging/error handling
;; (import "host" "printval" (func $printval (param i32))) ;; uncomment to be able to debug print values
(import "host" "error" (func $error (param i32 i32)))

;; MMM: Matrix Multiplication in WebAssembly
;;
;; This program reads two square matrices from the host, multiplies them, and writes the result back.
;; The matrices are stored in memory as follows:
;;
;;   A: [size, A_1, A_2, ..., A_{size * size}]
;;   B: [size, B_1, B_2, ..., B_{size * size}]
;;   C: [size, C_1, C_2, ..., C_{size * size}]
;;
;; The matrices are stored in row-major order.
;; The matrices are compatible for multiplication if the number of columns of A is equal to the number of rows of B.
;; The result matrix C is of size rows_A x cols_B.

;; The matrix multiplication function.
(func $mmm_body (param $a i32) (param $b i32) (param $mat_size i32) (param $output_offset i32)
  (local $i i32)
  (local $j i32)
  (local $k i32)
  (local $sum i32) ;; all values are 32 bit integers, so we can use i32 for the sum. assume no overflow
  (local $elems i32)
  (local $a_val i32)
  (local $b_val i32)
  (local $a_offset i32)
  (local $b_offset i32)
  (local $c_offset i32)
  (local $debug_val i32)

  ;; Get the number of elements in the matrices
  (local.set $elems (i32.mul (local.get $mat_size) (local.get $mat_size)))

  ;; initialize loop variables
  (local.set $i (i32.const 0))
  (local.set $j (i32.const 0))
  (local.set $k (i32.const 0))

  ;; The main loop for matrix multiplication
  (block $A_rows
    ;; loop over A's rows
    (loop $A_rows_loop
      (if (i32.ge_u (local.get $i) (local.get $mat_size)) (then (br $A_rows)))

      ;; loop over B's columns
      (block $B_cols
        (loop $B_rows_loop
          (if (i32.ge_u (local.get $j) (local.get $mat_size)) (then (br $B_cols)))
          (local.set $sum (i32.const 0))
          (local.set $k (i32.const 0))

          ;; loop over the columns of A
          (block $A_cols
            (loop $A_cols_loop
              (if (i32.ge_u (local.get $k) (local.get $mat_size)) (then (br $A_cols)))
              ;; load A[i * size + k] and B[k * size + j].
              ;; A_offs = a + (i * size + k) * 4
              ;; B_offs = b + (k * size + j) * 4
              (local.set $a_offset (i32.add (local.get $a) (i32.mul (i32.add (i32.mul (local.get $i) (local.get $mat_size)) (local.get $k)) (i32.const 4))))
              (local.set $b_offset (i32.add (local.get $b) (i32.mul (i32.add (i32.mul (local.get $k) (local.get $mat_size)) (local.get $j)) (i32.const 4))))
              
              ;; load the values
              (local.set $a_val (i32.load (local.get $a_offset)))
              (local.set $b_val (i32.load (local.get $b_offset)))

              ;; sum += A[i * size + k] * B[k * size + j]
              (local.set $sum (i32.add (local.get $sum) (i32.mul (local.get $a_val) (local.get $b_val))))
              ;; increment k
              (local.set $k (i32.add (local.get $k) (i32.const 1)))
              (br $A_cols_loop)
            )
          )

          ;; store the sum in C[i * size + j]
          (local.set $c_offset 
              (i32.add (local.get $output_offset) 
                       (i32.mul
                          (i32.add (i32.mul (local.get $i) 
                                            (local.get $mat_size)
                                    ) 
                                    (local.get $j)
                            )
                          (i32.const 4)
                       )
              )
          )
          (i32.store (local.get $c_offset) (local.get $sum))

          ;; increment j and continue loop
          (local.set $j (i32.add (local.get $j) (i32.const 1)))
          (br $B_rows_loop)
        )
      )

      ;; increment i
      (local.set $i (i32.add (local.get $i) (i32.const 1)))
      (local.set $j (i32.const 0))
      (br $A_rows_loop)

    )
  )

)

;; the main function responsible for the matrix multiplication
(func (export "mmm") 
  (local $size i32)
  (local $size_a i32)
  (local $size_b i32)
  (local $elems_a i32)
  (local $a i32)
  (local $b i32)
  (local $c i32)
  (local $total_buf_size i32)
  (local $mat_size_tmp i32)

  ;; Ask host to fill up to 64KB of memory with data. Some data may not be used.
  (call $fill_buf (i32.const 0) (i32.const 40000))

  ;; The host returns the size filled.
  (local.set $size)

  ;; Get the size of A, the first element in the buffer.
  i32.const 0
  i32.load
  local.set $size_a
  ;; set first addr of A (starts @ byte 4)
  (local.set $a (i32.const 4))
  ;; Get the number of elements (both matrices are square and of the same size)
  (local.set $elems_a (i32.mul (local.get $size_a) (local.get $size_a)))
  ;; Get the offset for the size of B, which is A(or 4) + 4 * (elems_a)
  (local.set $b (i32.add (i32.const 4) (i32.mul (local.get $elems_a) (i32.const 4))))
  ;; load from offset and store into size_b
  local.get $b
  i32.load
  (local.set $b (i32.add (local.get $b) (i32.const 4)))
  local.set $size_b
  
  ;; Check if the matrices are compatible for multiplication
  (if (i32.ne (local.get $size_a) (local.get $size_b))
    (then
      (call $error (local.get $size_a) (local.get $size_b))
      (return)
    )
  )

  ;; Get the starting address of the matrices. c will be stored in a separate buffer, so offset is just 0. integers are 4 bytes long
  (local.set $c (i32.add (local.get $b) (i32.mul (local.get $elems_a) (i32.const 4))))
  ;; store the resulting matrix size first, then move the pointer to the start of the matrix
  (i32.store (local.get $c) (local.get $size_a))
  (local.set $c (i32.add (local.get $c) (i32.const 4)))

  ;; Call the matrix multiplication function
  (call $mmm_body (local.get $a) (local.get $b) (local.get $size_a) (local.get $c))

  ;; Call the host to indicate that the buffer is done
  (local.set $mat_size_tmp (i32.add (i32.mul (local.get $elems_a) (i32.const 4)) (i32.const 4)))
  (local.set $total_buf_size (i32.mul (local.get $mat_size_tmp) (i32.const 3)))

  (call $buf_done (i32.const 0) (local.get $total_buf_size))
)