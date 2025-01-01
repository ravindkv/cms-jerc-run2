### General:
1. **Use of smart pointers:** Instead of raw pointers (e.g., `SkimTree* tree`), consider using `std::shared_ptr` or `std::unique_ptr` to improve memory safety.
2. **Encapsulation:** Keep class members private and expose only necessary public methods.
3. **Const-correctness:** Methods that do not modify the state of the class should be marked as `const`.
4. **Use of `override` keyword:** Mark virtual functions with `override` to ensure proper method overriding.
5. **Simplify repetitive code:** Refactor the repetitive `if (isDebug)` blocks using a helper function to reduce redundancy.
6. **Error handling:** Consider handling potential errors, e.g., null pointers or invalid data, in a robust way.
7. **Use Standard Library Containers:**
  - Prefer `std::vector`, `std::array`, etc., over raw arrays.
8. **Separate Interface and Implementation:**
  - Use header files (`.h` or `.hpp`) for declarations and source files (`.cpp`) for definitions.
9. ** Always use namespace when needed. Don't declare it
  - Use `std::cout` not just `cout`


## For Debugging (or catching the segmentation violation)
* Compile the code with flag -g in the Makefile (GCC=g++ -g -std=c++17)
* Run the code with gdp (
$ gdb --args ./runMain -o Data_2024Cv1_DiJet_JetMET1_Hist_1of100.root
* (gdb) run
* (gdb) backtrace
* (gdb) quit

Additionally
* valgrind --leak-check=full ./runMain -o Data_2024Cv1_DiJet_JetMET1_Hist_1of100.root
## Detect bugs
Use clang-tidy for that. 
* echo "Checks: 'modernize-*,performance-*,cppcoreguidelines-*'" > .clang-tidy
Uncomment corresponding lines in the Makefile and move them where we create .o files 


## Check which function takes how much time

* perf record ./runMain -o Data_2016PostF_ZeeJet_DoubleEG_Hist_1of100.root
* perf report

Alternatively use gprof
* Set -pg in the GCC flag in the Makefile
* ./runMain -o output_file.root
* gprof runMain gmon.out > gprof_analysis.txt

## Sanitize the program

AddressSanitizer (ASan) for memory errors:
* g++ -fsanitize=address -g

ThreadSanitizer (TSan) for race conditions:
* g++ -fsanitize=thread -g

UndefinedBehaviorSanitizer (UBSan) for undefined behavior:
* g++ -fsanitize=undefined -g

Once you run your program with these sanitizers, they will provide immediate feedback if they detect any issues. They are incredibly useful in ensuring your program is memory-safe, thread-safe, and free of undefined behavior.

