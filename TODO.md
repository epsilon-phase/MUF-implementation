## TODO

* [ ] Finish writing the primitives
* [x] Figure out how to correctly bind the for_vars values when a jump might take it out of the loop
* [x] Add IO primitives
* [ ] valgrind until it is perfect
* [ ] eliminate warnings from compilation with all warnings turned on.
* [x] Add entry points to the program(last declared function)
* [x] Add function calling.
* [ ] Add wider 'execution context' for the variables, and instance specific lvars.
* [ ] Add threading and message passing, keeping in line with the 'events' prims in protomuck
* [ ] Add PCRE primitives, regexp cache, and other such wonderful things.
* [ ] File primitives
* [ ] Optimization stuff
  * [ ] Peephole optimizer
    * Not all that sophisticated, can bring about excellent gains in performance if it is well done
  * [ ] Tail recursion detection
    * Extremely important for those who detest looping
  * [ ] String interning?
    * Faster string comparisons for larger programs with lots of stuff.
    * What is this? Java?
  * [ ] Tracing garbage collection?
    * Wikipedia claims it's faster than RC, but I'm not sure it makes sense for this language. (Or that the claim is accurate).
* [ ] Preprocessor
  * Not my favorite part of MUF, but certainly important for sanity in some cases.
    * [ ] \$def
    * [ ] \$define
    * [ ] \$include
    * [ ] \$undef
    * [ ] \$pubdef
    * [ ] \$libdef
