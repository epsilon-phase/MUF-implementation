# MUF Instruction Support

## Stack Manipulation

* [x] dup `x -- x x` duplicate the topmost item in the stack.
* [x] dupn `x1 x2 … xi i -- x1 … xi x1 … xi`
* [x] pop `x -- `
* [x] popn `x1 … xi i -- `
* [x] swap `x y -- y x`
* [x] tuck `x y -- y x y`
* [x] nip `x y -- y`
* [x] rot `x y z -- y z x`
* [x] rotate `x1 … xi i -- x2 … xi x1`
* [x] depth `-- i`
* [x] reverse `x1 x2 … xi i -- xi … x1`
* [x] lreverse `x1 x2 … xi i -- xi … x2 x1 i`
* [ ] put `xi … x1 xi i -- x1 … xi .. x1`
* [x] pick ` xi … x2 x1 i -- xi … x1 xi`
* [ ] sort 
* [x] over `x y -- x y x`

## Math/logic prims

* [ ] Math commands
    * [x] Basic math operators
        * [x] `+`
        * [x] `-` 
        * [x] `*`
        * [x] `/`
        * [x] `^` or `pow`
        * [x] `++`
        * [x] `--`
        * [x] `%`
    * [x] Trigonometry
        * [x] `sin`
        * [x] `cos`
        * [x] `tan`
        * [x] `asin`
        * [x] `acos`
        * [x] `atan`
        * [x] `atan2`
    * [x] `sqrt`
    * [x] `log`
    * [x] `log10`
    * [ ] `ln`
    * [ ] `polar_to_xyz`
    * [ ] `xyz_to_polar`
    * [ ] `dist3d`
    * [x] `ceil`
    * [x] `floor`
    * [ ] `round`
    * [ ] `modf`
    * [ ] `exp`
    * [x] `abs` `fabs`
    * [ ] Bit manipulation
        * [ ] `bitxor`
        * [ ] `bitshift`
        * [ ] `bitor`
        * [ ] `bitand`
    * [ ] Random functions
      * [ ] `random`
      * [ ] `frand`

# String manipulation

* [x] `atoi`
* [x] `intostr`
* [x] `split`
* [x] `rsplit`
* [x] `explode`
* [x] `subst`
* [x] `strlen`
* [x] `strcmp`
* [x] `instring`
* [x] `instr`
* [x] `tolower`
* [x] `toupper`
* [x] `strcat`
* [x] `stringpfx`
* [x] `striptail`
* [x] `striplead`
* [x] `strip`
* [ ] `smatch`
* [ ] `rinstr`
* [ ] `rinstring`
* [x] `midstr`
* [ ] `textattr`
* [ ] `parse_ansi`
* [ ] `unparse_ansi`
* [ ] `ansi_strlen`
* [ ] `parse_neon`
* [ ] `regexp`
* [ ] `regsub`
* [ ] `regmatch`
* [ ] `fmtstring`
* [ ] `fmtstringex`
* [ ] hashing function
    * [ ] `md5hash`
    * [ ] `sha1hash`

## Array primitives

* [x] array_make
* [x] array_make_dict
* [x] array_setitem
* [x] array_getitem
* [ ] array_nested_set
* [ ] array_nested_get
* [x] array_sum
* [x] array_count
* [ ] foreach
* [x] array_delitem
* [ ] array_getrange
* [ ] array_setrange
* [ ] array_delrange
* [x] array_appenditem
* [ ] array_nintersect
* [ ] array_nunion
* [ ] array_ndiff
* [ ] array_insertrange
* [ ] array_cut
* [ ] array_sort
* [ ] array_join
* [ ] array_explode
* [ ] explode_array
* [ ] array_keys
* [ ] array_vals
* [ ] array_next
* [ ] array_prev
* [ ] array_compare
* [ ] array_fmtstring
* [x] array_string_fragment

## Type querying

* [ ] `int?`
* [ ] `float?`
* [ ] `number?`
* [ ] `string?`
* [ ] `array?`
* [ ] `dictionary?`
