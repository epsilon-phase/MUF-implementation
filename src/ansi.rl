#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

%%{
  machine ansi_bullshit;
  write data;

}%%
int identify_basic_color(const char* i){
  printf("Identifying %s\n",i);
  if(strncmp(i,"red",3)==0){
    return 1;
  }else if(strncmp(i,"blue",4)==0){
    return 4;
  }else if(strncmp(i,"green",5)==0){
    return 2;
  }else if(strncmp(i,"yellow",6)==0){
    return 3;
  }else if(strncmp(i,"black",5)==0){
    return 0;
  }else if(strncmp(i,"white",5)==0){
    return 7;
  }else if(strncmp(i,"cyan",4)==0){
    return 6;
  }else if(strncmp(i,"magneta",7)==0){
    return 5;
  }
  return -1;
}
size_t parse_ansi(const char* input, char** output,size_t *output_size){
  int cs;
  const char *p = input,
             *pe = input + strlen(input)+1;
  char *ts=input, *te = input;
  bool background;
  int basic_color;
  int eight_bit_color;
  size_t output_capacity=strlen(input);
  output_capacity += output_capacity/2;
  *output=calloc(output_capacity,1);
  char* o= *output;
  struct {
    int red, green, blue;
  }twenty_four_bit;
  enum{
    basic,
    eight_bit,
    twenty_four
  } color_guess;
  %%{
  action emit_character {
    if(output_capacity==o-*output){
      //expand the array
      int offset = o-*output;
      output = realloc(output,output_capacity*2);
      o = *output+offset;
      output_capacity*=2;
      printf("array expanded");
    }
    *o = fc;
    o++;
  }
  basic_colors = ("black" | "red" | "green" | "yellow" | "blue" | "magenta" | "cyan" | "white") %{
    basic_color = identify_basic_color(ts+1);
    printf("Found offset %i\n",basic_color);
    color_guess = basic;
  };
  eight_bit_color = digit{1,3};
  twenty_four_bit_color = digit{1,3} ":" digit{1,3} ":" digit{1,3};
  
  ansi_sequence = "^" ('b' > { background = *p == 'b';})? (basic_colors | eight_bit_color | twenty_four_bit_color) "^";
  main := (ansi_sequence | [^\0] % emit_character )* 0 @{};
  write init;
  write exec noend;
  }%%
  return o - *output;
}

#ifdef ANSI_TEST
int main(){
  char* output;
  size_t output_size;
  parse_ansi("^green^hello",&output,&output_size);
  printf("%s\n",output);
  return 0;
}
#endif
