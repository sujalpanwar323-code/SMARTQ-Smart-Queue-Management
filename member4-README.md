# Member-4 - Request and Queue Management

## Your responsibility
- Own the `Request` structure and `QueueManager` operations.
- Manage registration, token generation, queue joining, tracking, ETA, cancellation, staff actions, counter status, history and persistence methods implemented inside `QueueManager`.

## Files
- `Member-4.cpp` - your actual assigned C++ code extracted from the team's original `src/main.cpp`.

## Dependencies / integration notes
This code depends on `Status`, `statusName`, `nowText`, `clean` (shared helpers near the top of original `main.cpp`), plus Member-1 `Student`, Member-2 `LinkedQueue`, and Member-3 `Counter` / `CounterHeap`. Coordinate these interfaces before integration.

## What to do before uploading to GitHub
1. Open `Member-4.cpp` and understand the assigned code.
2. Make changes only to your assigned responsibility.
3. Do not create another `main()` function.
4. Do not overwrite the complete project `src/main.cpp` with this contribution file.
5. Coordinate function names and class declarations with the team leader before changing shared interfaces.
6. Upload this folder to your assigned branch (`Member-4`) and create a Pull Request targeting `main`.

## Expected deliverables
- Updated request/queue-management implementation.
- Test cases for token generation, join, track, cancel, call-next, start, complete and no-show flows.

## Important
The original project currently keeps most implementation in one `src/main.cpp`. The extracted file preserves the relevant existing code, but some classes/functions refer to code assigned to other members. It must be integrated with the other members' contributions and the original data files before compiling the complete application.
