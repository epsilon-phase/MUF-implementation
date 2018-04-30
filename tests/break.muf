: main
  begin
    break
  repeat
  2
  1 depth 2 = swap 1 = and  swap 2 = and
  "Break does the right thing." "Break doesn't do the right thing" assert
  1 5 1 for
    pop
  repeat 
 2 1 1 = swap 2 = and
  "For works" "For doesn't work" assert
  5 0 -1 for
    while 
  repeat
  depth not "while seems to work" "While doesn't work" assert
;
