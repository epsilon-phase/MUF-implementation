: main
  "1 2 3 4 5 6" "_" " " subst
  "1_2_3_4_5_6" strcmp not
  "Subst works alright"
  "Subst fails" assert  
  "1 2 3 4 5 6 7" "__" " " subst
  "1__2__3__4__5__6__7" strcmp not
  "Subst works with longer stuff"
  "Subst fails on longer stuff" assert
  "1_2 3 4 5" "1 " "1_" subst 
;
