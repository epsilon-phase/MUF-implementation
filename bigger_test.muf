: auxiliary
    whoa
;
: try-test
    try
        1
    catch
        2
    endcatch
    try
        1
    catch_detailed
        2
    endcatch
;
: nested_if
    if if if then then then ;
: if-in-loop
    begin if then 1 repeat ;
: double-nested
    1 if if if 1 if then then then then ;
: double
    1.2
;
: string 
    " "
;
: main
    var a
    1 a @ 1 for
        pop
    repeat
    1 a @ 1 for
        dup 2 % if 
            1 +
        then
        pop
    repeat
;

