: main
    "hello" "" explode
    5 = 6 rotate
    "h" strcmp not 6 rotate
    "e" strcmp not 6 rotate
    "l" strcmp not 6 rotate
    "l" strcmp not 6 rotate
    "o" strcmp not 6 rotate
    and and and and
    "hello split correctly into 5 strings." "hello not split correctly with empty signifier" assert
    "hello there fellow kids" " " explode
    4 = 5 rotate
    "hello" strcmp not 5 rotate
    "there" strcmp not 5 rotate
    "fellow" strcmp not 5 rotate
    "kids" strcmp not 5 rotate
    and and and and
    "Split worked on simple single character separator" "Split failed on a simple single-character separator" assert
;
