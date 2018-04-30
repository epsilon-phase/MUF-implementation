: main (for toupper and tolower)
  "HeLlo" dup toupper "HELLO" strcmp not swap
  tolower "hello" strcmp not and
  "Both toupper and tolower seem to work fine." "Something's wrong doc." assert
;
