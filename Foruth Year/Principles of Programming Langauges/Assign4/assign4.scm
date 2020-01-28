#lang racket
(define (insert e L)
  (insert_aux e L (length L))
)

(define (insert_aux e L len)
  (if (> len 0)
    (cons (insert_aux2 e L len)  (insert_aux e L (- len 1)) )
    (list (insert_aux2 e L len))
  )
)

(define (insert_aux2 e L len)
  (if (> len 0)
    (append  (first L len) (cons e (last L len)))
    (cons e L)
  )
)

(define (first L len)
  (if (> len 0)
    (cons (car L) (first (cdr L) (- len 1)) )
    (list)
  )
)

(define (last L len)
  (if (> len 0)
    (last (cdr L) (- len 1))
    L
  )
)