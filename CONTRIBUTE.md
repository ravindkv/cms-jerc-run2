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
