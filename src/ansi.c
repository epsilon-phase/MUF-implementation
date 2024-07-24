
#line 1 "src/ansi.rl"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#line 10 "src/ansi.c"
static const char _ansi_bullshit_actions[] = {
	0, 1, 0, 1, 3, 2, 0, 3, 
	2, 1, 0, 2, 2, 0
};

static const char _ansi_bullshit_key_offsets[] = {
	0, 0, 2, 4, 13, 23, 26, 30, 
	33, 36, 38, 41, 44, 47, 50, 53, 
	56, 59, 62, 65, 68, 71, 74, 77, 
	80, 83, 86, 89, 92, 95, 98, 101, 
	104, 107
};

static const char _ansi_bullshit_trans_keys[] = {
	0, 94, 0, 94, 0, 94, 98, 99, 
	103, 109, 114, 119, 121, 0, 94, 98, 
	99, 103, 108, 109, 114, 119, 121, 0, 
	94, 108, 0, 94, 97, 117, 0, 94, 
	99, 0, 94, 107, 0, 94, 0, 94, 
	101, 0, 94, 121, 0, 94, 97, 0, 
	94, 110, 0, 94, 114, 0, 94, 101, 
	0, 94, 101, 0, 94, 97, 0, 94, 
	103, 0, 94, 101, 0, 94, 110, 0, 
	94, 116, 0, 94, 97, 0, 94, 101, 
	0, 94, 100, 0, 94, 104, 0, 94, 
	105, 0, 94, 116, 0, 94, 101, 0, 
	94, 108, 0, 94, 108, 0, 94, 111, 
	0, 94, 119, 0
};

static const char _ansi_bullshit_single_lengths[] = {
	0, 2, 2, 9, 10, 3, 4, 3, 
	3, 2, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 0
};

static const char _ansi_bullshit_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0
};

static const unsigned char _ansi_bullshit_index_offsets[] = {
	0, 0, 3, 6, 16, 27, 31, 36, 
	40, 44, 47, 51, 55, 59, 63, 67, 
	71, 75, 79, 83, 87, 91, 95, 99, 
	103, 107, 111, 115, 119, 123, 127, 131, 
	135, 139
};

static const char _ansi_bullshit_indicies[] = {
	1, 2, 0, 4, 5, 3, 4, 5, 
	6, 7, 8, 9, 10, 11, 12, 3, 
	4, 5, 13, 7, 8, 14, 9, 10, 
	11, 12, 3, 4, 5, 14, 3, 4, 
	5, 15, 16, 3, 4, 5, 17, 3, 
	4, 5, 18, 3, 4, 19, 3, 4, 
	5, 18, 3, 4, 5, 20, 3, 4, 
	5, 21, 3, 4, 5, 18, 3, 4, 
	5, 22, 3, 4, 5, 23, 3, 4, 
	5, 21, 3, 4, 5, 24, 3, 4, 
	5, 25, 3, 4, 5, 26, 3, 4, 
	5, 27, 3, 4, 5, 28, 3, 4, 
	5, 18, 3, 4, 5, 29, 3, 4, 
	5, 18, 3, 4, 5, 30, 3, 4, 
	5, 31, 3, 4, 5, 16, 3, 4, 
	5, 32, 3, 4, 5, 33, 3, 4, 
	5, 34, 3, 4, 5, 35, 3, 4, 
	5, 18, 3, 36, 0
};

static const char _ansi_bullshit_trans_targs[] = {
	2, 33, 3, 2, 33, 3, 4, 11, 
	14, 17, 23, 25, 28, 5, 6, 7, 
	10, 8, 9, 3, 12, 13, 15, 16, 
	18, 19, 20, 21, 22, 24, 26, 27, 
	29, 30, 31, 32, 0
};

static const char _ansi_bullshit_trans_actions[] = {
	0, 3, 0, 1, 5, 1, 11, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 8, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 0
};

static const int ansi_bullshit_start = 1;
static const int ansi_bullshit_first_final = 33;
static const int ansi_bullshit_error = 0;

static const int ansi_bullshit_en_main = 1;


#line 10 "src/ansi.rl"

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
  
#line 153 "src/ansi.c"
	{
	cs = ansi_bullshit_start;
	}

#line 158 "src/ansi.c"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _ansi_bullshit_trans_keys + _ansi_bullshit_key_offsets[cs];
	_trans = _ansi_bullshit_index_offsets[cs];

	_klen = _ansi_bullshit_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _ansi_bullshit_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _ansi_bullshit_indicies[_trans];
	cs = _ansi_bullshit_trans_targs[_trans];

	if ( _ansi_bullshit_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _ansi_bullshit_actions + _ansi_bullshit_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 53 "src/ansi.rl"
	{
    if(output_capacity==o-*output){
      //expand the array
      int offset = o-*output;
      output = realloc(output,output_capacity*2);
      o = *output+offset;
      output_capacity*=2;
      printf("array expanded");
    }
    *o = (*p);
    o++;
  }
	break;
	case 1:
#line 65 "src/ansi.rl"
	{
    basic_color = identify_basic_color(ts+1);
    printf("Found offset %i\n",basic_color);
    color_guess = basic;
  }
	break;
	case 2:
#line 73 "src/ansi.rl"
	{ background = *p == 'b';}
	break;
	case 3:
#line 74 "src/ansi.rl"
	{}
	break;
#line 261 "src/ansi.c"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	p += 1;
	goto _resume;
	_out: {}
	}

#line 77 "src/ansi.rl"

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
