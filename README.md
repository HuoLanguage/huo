# huo
Writing an interpreted language in C for fun.

##features
It has lispy syntax because it's easy to parse into a tree.
```code
(+ 3 (* 4 3))
```
Numbers are all doubles unless you divide them. Then they are all floats.
```code
(+ 3 3) // 6

(+ 3 (/ 10 4)) // 5.500000

(* 3 (/ 10 4)) // 7.500000
```
Huo currently supports the following functions:
```code
basic math
+ - * /
```
print: print a value to the console
```code
(print (* 3 6)) // prints 18 to the console
```
cat: concatenate strings
```code
(cat "hello " "world!) // returns "hello world!"
```
booleans
```code
(= 3 2) // returns false
(= 1 1) // returns true
(= "Hey" "hey") // returns true

(! 1 3) // returns true
(! "Hey" (cat "He" "y")) // returns false
```
let bindings
```code
(let x 5)
(print (+ x 3) // prints 8
```
arrays
```code
(let x [1,2,3,4])
(print x) // prints [1, 2, 3, 4]
(let y [[1,2], [3,4]]) // nesting is fine
(let z [[3,4], [5,6]])
(print (+ y z)) // adding nested arrays; prints [[4,6], [8,10]]

// use index to return a single array value
(index 2 [1, 2, 3]) // -> 2

// push and length are straight-forward
(push 5 [1, 2, 3, 4]) // -> [1, 2, 3, 4, 5]
(length [1, 2, 3]) // -> 3

// map and each must take two arguments, the name of
// the current item and a variable for current index
(let x [1, 2, 3])
(let y 0)
(each x num idx (let y (+ y num)))
(print y) // -> 6
(map x num idx (+ num idx))
(print x) // -> [1, 3, 5]
```

##compile
make   
make clean   

##run
create a huo file syntax:   
test.txt
```code
(print (+ 1 3 ( * 2 3 ) ( / 6 2 )))
```

run the file:
```shell
$ ./huo test.txt
```
