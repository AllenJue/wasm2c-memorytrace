(import "host" "mem" (memory $mem 1))
(import "host" "fill_buf" (func $fill_buf (param i32 i32) (result i32)))
(import "host" "buf_done" (func $buf_done (param i32 i32)))

;; Recursive Fibonacci calculation with memoization.
(func $fibonacci (param $n i32) (result i32)
  (local $result i32)

  ;; Base cases: fib(0) = 0, fib(1) = 1
  (if (i32.eq (local.get $n) (i32.const 0))
    (then (return (i32.const 0)))
  )
  (if (i32.eq (local.get $n) (i32.const 1))
    (then (return (i32.const 1)))
  )

  ;; Check if already calculated and stored in memory
  ;; Multiplying by 4, since memory is linear and integers are every 4 bytes
  (local.set $result (i32.load (i32.mul (local.get $n) (i32.const 4))))
  ;; UNCOMMENT THESE TO MEMOIZE
  (if (i32.ne (local.get $result) (i32.const 0))
    (then (return (local.get $result)))
  )

  ;; Recursive Fibonacci: fib(n) = fib(n-1) + fib(n-2)
  (local.set $result (i32.add 
                        (call $fibonacci (i32.sub (local.get $n) (i32.const 1))) 
                        (call $fibonacci (i32.sub (local.get $n) (i32.const 2)))
                      ))

  ;; Store the result in memory at index n
  (i32.store (i32.mul (local.get $n) (i32.const 4)) (local.get $result))

  ;; Return the result
  (return (local.get $result))
)

;; Main function to process Fibonacci series in memory.
(func (export "fibonacci_loop") (param $n i32) 
  (local $i i32) ;; Loop counter
  (local $result i32) ;; To store the final Fibonacci number

  ;; Initialize the buffer with base cases
  (call $fill_buf (i32.const 0) (i32.mul (local.get $n) (i32.const 4)))

  ;; Initialize loop counter
  (local.set $i (i32.const 0))

  ;; Loop over to calculate and store Fibonacci numbers in memory.
  (block $exit
    (loop $top
      ;; If we've reached the limit, exit the loop.
      (if (i32.ge_u (local.get $i) (local.get $n)) (then (br $exit)))

      ;; Calculate and store Fibonacci for the current index.
      (call $fibonacci (local.get $i))

      ;; Increment the loop counter
      (local.set $i (i32.add (local.get $i) (i32.const 1)))
      (br $top)
    )
  )

  ;; Notify the host that Fibonacci calculation is done.
  (call $buf_done (i32.const 0) (i32.mul (local.get $n) (i32.const 4)))

  ;; Return the last Fibonacci number calculated (fib(n-1) as per the loop)
  (local.set $result (call $fibonacci (i32.sub (local.get $n) (i32.const 1))))
  (return (local.get $result))
)

