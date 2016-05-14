# huo
A simple c interpreter.

I suddenly realized how interpreters work so I figured I'd implement one in C. Not sure how far I'll go with it.

##compile
clang main.c -o huo

##run
create a file with some simple lisp-like syntax:   
test.txt
```code
(+ 1 3 ( * 2 3 ) ( / 6 2 ))
```

run the file:
```shell
$ huo ./test.txt
$ 13
```

Amazing... more to come.
