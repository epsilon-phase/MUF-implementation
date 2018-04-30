: main
  "     hello" striplead "hello" strcmp not
  "Striplead seems to be working" "Striplead doesn't seem to be working" assert
  "hello   " striptail "hello" strcmp not
  "Striptail works fine, it seems." "Striptail fails!" assert
;
