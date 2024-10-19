;; debugging loop to print all values of A and B
;;---------------------
    (block $exit
    (loop $top
      ;; if (i >= size) break
      (if (i32.ge_u (local.get $i) (local.get $elems_a)) (then (br $exit)))

      ;;(call $printval (i32.add (local.get $a) (i32.mul (local.get $i) (i32.const 4))))
      ;;(call $printval (local.get $elems_a))
      ;;(call $printval (i32.const 0xFFFF))
      ;;(call $printval (local.get $a))
      ;; mem[i] = A[i] where i is the offset
      
      ;; push the memory address of A[i] to the stack. each integer is 4 bytes long so we multiply by 4
      ;; (local.set $a_offset (i32.add (local.get $a) (i32.mul (local.get $i) (i32.const 4))))
      (local.set $a_offset (i32.add (local.get $a) (local.get $i)))
      (local.set $b_offset (i32.add (local.get $b) (local.get $i)))

      ;; load the value at the memory address
      (call $printval (i32.load8_u (local.get $a_offset)))
      (call $printval (local.get $a_offset))
      (call $printval (i32.load8_u (local.get $b_offset)))
      (call $printval (local.get $b_offset))

      ;; i++
      (local.set $i (i32.add (local.get $i) (i32.const 1)))
      (br $top)
    )
  )
;;---------------------