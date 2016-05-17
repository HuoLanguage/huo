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



##compile
clang main.c -o huo

##run
create a file with some simple lisp-like syntax:   
test.txt
```code
(print (+ 1 3 ( * 2 3 ) ( / 6 2 )))
```

run the file:
```shell
$ huo ./test.txt
```
