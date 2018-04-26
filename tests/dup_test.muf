: main
  "Entering Integer dup test" notify
  1 dup = 
  "seems to work" "Huh. That's broken :O" assert
  "Entering string duplication test" notify
  "test" dup strcmp not "String duplication results in equivalent strings." "String duplication does not result in equal strings O.o" assert
  "String modification and duplication test" notify
  "test" dup "f" strcat strcmp "Strings do not modify other strings." "Strings are modified! HELP!" assert
  "Floating point dup check" notify
  1.2 dup = "Floating points seem to dup fine." "Floating points seem to be broken." assert
;
