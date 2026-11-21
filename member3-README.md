# Member-3 - Counter Heap and Counter Model

## Your responsibility
- Maintain `CounterHeap`, the indexed min-heap used for workload ordering.
- Maintain the `Counter` structure and its assigned queue/workload fields.

## Files
- `Member-3.cpp` - your actual assigned C++ code extracted from the team's original `src/main.cpp`.

## Dependencies / integration notes
`Counter` uses Member-2 `LinkedQueue`. `QueueManager` uses both `Counter` and `CounterHeap`. Preserve the existing member names and method signatures.

## What to do before uploading to GitHub
1. Open `Member-3.cpp` and understand the assigned code.
2. Make changes only to your assigned responsibility.
3. Do not create another `main()` function.
4. Do not overwrite the complete project `src/main.cpp` with this contribution file.
5. Coordinate function names and class declarations with the team leader before changing shared interfaces.
6. Upload this folder to your assigned branch (`Member-3`) and create a Pull Request targeting `main`.

## Expected deliverables
- Tested heap insert/update/erase behavior.
- Counter fields and queue integration checked.

## Important
The original project currently keeps most implementation in one `src/main.cpp`. The extracted file preserves the relevant existing code, but some classes/functions refer to code assigned to other members. It must be integrated with the other members' contributions and the original data files before compiling the complete application.
