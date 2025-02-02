**Naming conventions: ** consistent CamelCase (PascalCase for classes, lowerCamelCase for methods, data members, and free functions). We maintain your existing directory structure (i.e., `header/*.h`, `cpp/*.cpp`) and Makefile but apply style changes throughout:

1. **Classes:** `PascalCase` (e.g., `GlobalFit`, `DataContainer`).
2. **Methods/Functions:** `lowerCamelCase` (e.g., `run()`, `loadInputs()`, `replaceDefault()`).
3. **Member Variables:** `m_` prefix + `lowerCamelCase` (e.g., `m_runName`, `mMode`, `mEtaMin`).
4. **Constants:** `k` prefix for “constant” or `kCamelCase` (e.g., `kFuncRangeMin`).
5. **Global Variables (extern):** `g_` prefix (e.g., `gMyData`, `gShapes`, `gSources`).

