: main
  "Proper Split test" notify
  "Hello World" " " split "World" strcmp not swap "Hello" strcmp not and 
  "Hello World properly split at the space" "Hello World incorrectly split D:" assert
  "Null split test" notify
  "Hello World" "g" split "" strcmp not swap "Hello World" strcmp not and
  "Hello World was not split(this is good)." "Hello world was split along a g. Somehow" assert
  "rsplit test" notify
  "well hello there" " " rsplit
  "there" strcmp not swap "well hello" strcmp not and "rsplit split the meme well enough." "rsplit screwed up." assert
;
