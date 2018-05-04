: main
  { 1 2 3 } array_make
  1 array_delitem dup 0 array_getitem 1 = 
  swap 
  1 array_getitem 3 = and
  "Array deleted item properly." "Array did not delete item properly."  assert
  { 1 1 2 2 3 3 } 2 / array_make_dict
  2 array_delitem dup 1 array_getitem 1 = swap 3 array_getitem 3 = and
   "Dictionary deleted item properly." "Dictionary did not delete item properly."  assert
;
