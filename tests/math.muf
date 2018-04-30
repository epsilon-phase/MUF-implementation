: main
    1 2 * dup
    3 + 5 = swap 2 = and
    "Addition and multiplication across integers seems fine." "Addition and multiplication seems off" assert
    4 5 / 0 = 
    "Integer division works" "Integer division fails." assert
    12 10 % 2 = "Modulus works" "Modulus doesn't work" assert
;
