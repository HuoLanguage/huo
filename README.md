# huo
Writing an interpreted language in C for fun.

##features
It has lispy syntax because it's easy to parse into a tree.
```code
(+ 3 (* 4 3))
```
Numbers are all doubles unless you divide them. Then they are all floats.
```code
(+ 3 3)
returns 6

(+ 3 (/ 10 4))
returns 5.500000

(* 3 (/ 10 4))
returns 7.500000
```
Huo currently supports the following functions:
```code
basic operators
+ - * /
```
```code
print for printing values
(print (* 3 6))
outputs 18 to the console

cat for concatenating strings
(cat "hello " "world!)
returns "hello world!"
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
