(import "host" "mem" (memory $mem 1))
(import "host" "fill_buf" (func $fill_buf (param i32 i32) (result i32)))
(import "host" "buf_done" (func $buf_done (param i32 i32)))

;; debugging/error handling
(import "host" "printval" (func $printval (param i32)))
(import "host" "error" (func $error (param i32 i32)))

;; get the size of matrix A and B. you cannot seem to load and store to local variables in wat, so we will have to do this in the main function
(import "host" "get_size_a" (func $get_size_a (result i32)))
(import "host" "get_size_b" (func $get_size_b (result i32)))

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
  (local $sum i32) ;; all values are 32 bit integers, so we can use i32 for the sum
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

          ;; initialize the sum to 0
          (local.set $sum (i32.const 0))
          (local.set $k (i32.const 0))

          ;; debug print
          ;; (call $printval (i32.const 0xFFFF))
          ;; (call $printval (local.get $i))
          ;; (call $printval (local.get $j))

          ;; loop over the columns of A
          (block $A_cols
            (loop $A_cols_loop
              (if (i32.ge_u (local.get $k) (local.get $mat_size)) (then (br $A_cols)))

              ;; load A[i * size + k] and B[k * size + j]
              (local.set $a_offset (i32.add (local.get $a) (i32.add (i32.mul (local.get $i) (local.get $mat_size)) (local.get $k))))
              (local.set $b_offset (i32.add (local.get $b) (i32.add (i32.mul (local.get $k) (local.get $mat_size)) (local.get $j))))
              ;; 4 bytes per integer
              local.set $a_offset (i32.mul (local.get $a_offset) (i32.const 4))
              local.set $b_offset (i32.mul (local.get $b_offset) (i32.const 4))

              ;; debug print
              ;; (call $printval (local.get $k))
              ;; (call $printval (local.get $mat_size))
              ;; (call $printval (local.get $a_offset))
              ;; (call $printval (local.get $b_offset))
              
              ;; load the values
              (local.set $a_val (i32.load (local.get $a_offset)))
              (local.set $b_val (i32.load (local.get $b_offset)))

              ;; sum += A[i * size + k] * B[k * size + j]
              (local.set $sum (i32.add (local.get $sum) (i32.mul (local.get $a_val) (local.get $b_val))))
              ;; (call $printval (local.get $sum))
              ;; increment k
              (local.set $k (i32.add (local.get $k) (i32.const 1)))
              (br $A_cols_loop)
            )
          )

          ;; store the sum in C[i * size + j]
          (local.set $c_offset (i32.add (local.get $output_offset) (i32.add (i32.mul (local.get $i) (local.get $mat_size)) (local.get $j))))
          
          
          ;; (call $printval (local.get $c_offset))
          (i32.store (local.get $c_offset) (local.get $sum))

          ;; increment j
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

  

  ;; Ask host to fill memory [0, 1024) with data. Some data may not be used.
  (call $fill_buf (i32.const 0) (i32.const 35000))

  ;; The host returns the size filled.
  (local.set $size)

  ;; Get the sizes of A and B from the host buffer. for some reason I cannot load from memory and store to a local variable in wat.
  (call $get_size_a)
  (local.set $size_a)
  (call $get_size_b)
  (local.set $size_b)

  ;; Check if the matrices are compatible for multiplication
  (if (i32.ne (local.get $size_a) (local.get $size_b))
    (then
      (call $error (local.get $size_a) (local.get $size_b))
      (return)
    )
  )

  ;; Get the number of elements (both matrices are square and of the same size)
  (local.set $elems_a (i32.mul (local.get $size_a) (local.get $size_a)))

  ;; Get the starting address of the matrices. c will be stored in a separate buffer, so offset is just 0. integers are 4 bytes long
   (local.set $a (i32.const 4))
   (local.set $b (i32.add (i32.const 4) (i32.mul (local.get $elems_a) (i32.const 4))))
   (local.set $c (i32.add (local.get $b) (i32.mul (local.get $elems_a) (i32.const 4)))

  ;; right now I can only read u8s??
  ;;(local.set $a (i32.const 1))
  ;;(local.set $b (i32.add (i32.const 2) (local.get $elems_a)))
  ;;(local.set $c (i32.add (local.get $b) (local.get $elems_a)))
  ;; store the resulting matrix size first, then move the pointer to the start of the matrix
  (i32.store (local.get $c) (local.get $size_a))
  (local.set $c (i32.add (local.get $c) (i32.const 4)))

  ;; (call $printval (local.get $a))
  ;; (call $printval (local.get $b))
  ;; (call $printval (local.get $c))

  ;; Call the matrix multiplication function
  (call $mmm_body (local.get $a) (local.get $b) (local.get $size_a) (local.get $c))
  
  ;; Call the host to indicate that the buffer is done
  local.get $size
  local.get $elems_a
  i32.add
  local.set $total_buf_size
  (call $buf_done (i32.const 0) (local.get $total_buf_size))
)