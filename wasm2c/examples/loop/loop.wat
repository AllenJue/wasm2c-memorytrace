(module
  (import "host" "mem" (memory $mem 1))
  (import "host" "fill_buf" (func $fill_buf (param i32 i32) (result i32)))
  (import "host" "buf_done" (func $buf_done (param i32 i32)))

  (func (export "loop") ;; (param $n i32) 
    (call $fill_buf (i32.const 0) (i32.mul (i32.const 10) (i32.const 4)))
    (call $buf_done (i32.const 0) (i32.mul (i32.const 10) (i32.const 4)))

    (return (i32.const 10))

  )


)
