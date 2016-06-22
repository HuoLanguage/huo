# huo
This is my first interpreter which I started as a hobby project. I'm doing it because I like the challenge of C and I think implementing language features is fun. My dream for it is to have a kind of minimal simplicity and cool native features like matrix math and parallel processing. There are still a number of fairly major issues to be overcome before Huo could be a stable and useful language, but I will tackle them one at a time. Contributions are also welcome. Huo means "living" or "fire" in Chinese. It's pretty hard to pronounce, so if you don't know Chinese you can just say "hoo-ah".

##features
It has lisp-like syntax for now because it's easy to parse into a tree.
```lisp
(+ 3 (* 4 3))
```
Whole numbers are longs, numbers with decimals are floats.
```lisp
(+ 3 3.5) ; 6.500000

(+ 3 (/ 10 4)) ; 5.500000

(* 3 (/ 10 4)) ; 7.500000
```
basic math
```lisp
(+ 1 1) ; -> 2
(* 2 6) ; -> 12
(/ 9 7) ; -> 1.285714
(- 10 1); -> 9
```
printing values to the console
```lisp
(print (* 3 6)) ; prints 18 to the console
(print "hello world") ; prints "hello world" to the console
```
string functions
```lisp
(cat "hello " "world!") ; -> "hello world!"
(substring 0 4 "hello world") ; -> "hell"
(length "hello world") ; -> 11

; the split function takes a single character to split on
(split "o" "hello world"); -> [ "hell", " w", "rld" ]

```
booleans
```lisp
(= 3 2) ; -> false
(= 1 1) ; -> true
(= "Hey" "hey") ; -> true

(! 1 3) ; -> true
(! "Hey" (cat "He" "y")) ; -> false

(> 4 3) ; -> true
(< 10 1) ; -> false
```
if block
```lisp
; the if block requires three functions: conditional, true result and false result
; to do an else-if just put another if block in the false result position
(if (> 3 4)
    (print "normal math")
    (print "backwards land math")
)
```
let bindings
```lisp
; let bindings are stored in a key-value structure and are passed
; by reference which means they are mutable
(let x 5)
(let y [])
(push 5 y)
(print (+ x 3) ; -> 8
(print y) ; -> [ 5 ]
```
arrays
```lisp
(let x [1,2,3,4])
(print x) ; prints [1, 2, 3, 4]
(let y [[1,2], [3,4]]) ; nesting is fine
(let z [[3,4], [5,6]])
(print (+ y z)) ; adding nested arrays; prints [[4,6], [8,10]]

; use index to return a single array value
(index 2 [1, 2, 3]) ; -> 3

; push and length are straight-forward
(push 5 [1, 2, 3, 4]) ; -> [1, 2, 3, 4, 5]
(length [1, 2, 3]) ; -> 3

; use set to change a value at a specific index
; it takes index, item, and array as args
(set 0 10 [1,2,3,4]) ; -> [ 10, 2, 3, 4 ]

; map and each both take four arguments:
; first the array you want to iterate over
; the names for the current item and the index
; and then the function to call each iteration
(let x [1, 2, 3])
(let y 0)
(each x num idx (let y (+ y num)))
(print y) ; -> 6
(map x num idx (+ num idx))
(print x) ; -> [1, 3, 5]

; reduce is different, it takes the array to iterate over,
; the names of the accumulator and the current item, the function to
; call each iteration, and an optional initial value for the accumulator
(def avg x
    (/
        (reduce x acc cur (+ acc cur))
        (length x)
    )
)
(let x [1,2,3,4,13])
(print (reduce (push 4 [1,2,3]) acc cur (+ acc cur) 0)) ; -> 10
(print (avg x)) ; -> 4.600000
```
for loop
```lisp
; the for loop takes three arguments
; start number, end number, function to call each iteration
; start and end can be functions that return numbers
(let x [])
(for 0 10 
    (set (length x) 0 x)
)
(print x) ; -> [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
```
do block
```lisp
; because each, reduce, and for loop only take one function
; the do block is a function that takes n functions and executes them
; in order, returning the value from the last function inside it
(def factorial x
    (do
        (let z [ ])
        (for 0 x
            (set (length z) (+ (length z) 1) z)
        )
        (reduce z acc cur (* acc cur) 1)
    )
)
```
parallel execution
```lisp
; a parallel block takes any number of functions and executes them
; in parallel. The parallel block returns undefined. Beware of passing
; the same variable to two different functions in a parallel block!

(let x 0)
(let y [])
(parallel 
    (for 0 100 (let x (+ x 1)))
    (for 0 100 (set (length y) (length y) y))
)
; the above code mutates x and y in parallel
```
read file
```lisp
; reading a file is simple and returns a string

(let file (read "./files/example.txt"))
(print file) ; prints contents of example.txt
```
imports
```lisp
; you can import a huo file that contains functions definitions
; the import function will ignore everything but functions
(import "./math.huo") ; import file containing average function
(let x (average [1,2,3,4,5,6] )) ; use imported function
```
return
```lisp
; sometimes you just want to return a value or the composition
; of a few values, for that you can use return
(def pair x y (return [x, y]))
(let x (pair 0 "start"))
(print x) ; -> [ 0, "start" ]
```
##compile
make   
make clean   

##run
create a huo file:   
test.huo
```code
(print (+ 1 3 ( * 2 3 ) ( / 6 2 )))
```

run the file:
```shell
$ ./huo test.huo
```
