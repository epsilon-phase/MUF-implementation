: main
    1 2 3 0 array_make
    0 2 1 for
        array_setitem
    repeat
    dup
    0 array_getitem
    2 popn
    1 2 3 4 5 6 
    9 10 7 8 5 array_make_dict
    11 25 2 for
        swap over 1 + array_setitem
    repeat
    2 swap "5" array_setitem dup
    "file.dot" array_dump
;
