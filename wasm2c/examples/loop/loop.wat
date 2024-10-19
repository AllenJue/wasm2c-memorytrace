(module
  (import "host" "mem" (memory $mem 1))
  (import "host" "fill_buf" (func $fill_buf (param i32 i32) (result i32)))
  (import "host" "buf_done" (func $buf_done (param i32 i32)))

  ;; Main function
  (func (export "loop") 
    (local $i i32)  ;; Loop index
    (local $j i32)  ;; Secondary loop index
    (local $val i32) ;; Temporary variable to store array value

    (call $fill_buf (i32.const 0) (i32.mul (i32.const 10) (i32.const 4)))
    (call $buf_done (i32.const 0) (i32.mul (i32.const 10) (i32.const 4)))

    (local.set $i (i32.const 0)) ;; Initialize index to 0
    (loop $outer
      ;; Exit after array length 10
      (br_if $outer (i32.ge_u (local.get $i) (i32.const 10)))

      (local.set $val (i32.load (i32.add (i32.const 0) (i32.mul (local.get $i) (i32.const 4)))))

      (local.set $j (i32.const 0)) ;; Initialize inner loop counter
      (loop $inner
        ;; Repeat arr[i] times
        (br_if $inner (i32.ge_u (local.get $j) (local.get $val)))
        ;; (You could do something with the value here if needed)

        (local.set $j (i32.add (local.get $j) (i32.const 1)))
        (br $inner)
      )

      (local.set $i (i32.add (local.get $i) (i32.const 1)))
      (br $outer)
    )

    (call $buf_done (i32.const 0) (i32.mul (i32.const 10) (i32.const 4)))
    (return (i32.const 10))
  )
)
