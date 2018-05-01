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
* [ ] put


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
    * [ ] `log`
    * [ ] `log10`
    * [ ] `ln`
    * [ ] `polar_to_xyz`
    * [ ] `xyz_to_polar`
    * [ ] `dist3d`
    * [ ] `ceil`
    * [ ] `floor`
    * [ ] `round`
    * [ ] `modf`
    * [ ] `exp`
    * [ ] `abs` `fabs`
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
