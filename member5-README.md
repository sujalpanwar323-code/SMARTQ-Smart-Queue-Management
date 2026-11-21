# Member-5 - Portals, Input Helpers and Main Integration

## Your responsibility
- Own console input helpers and the `main()` menu flow.
- Connect Student Portal, Staff Portal and Admin Portal to the `QueueManager` public interface.
- Run end-to-end tests after other members submit their parts.

## Files
- `Member-5.cpp` - your actual assigned C++ code extracted from the team's original `src/main.cpp`.

## Dependencies / integration notes
`main()` constructs `QueueManager` and calls its public methods. It cannot compile until Member-4 and all supporting classes are integrated. There must be exactly one `main()` in the final project.

## What to do before uploading to GitHub
1. Open `Member-5.cpp` and understand the assigned code.
2. Make changes only to your assigned responsibility.
3. Do not create another `main()` function.
4. Do not overwrite the complete project `src/main.cpp` with this contribution file.
5. Coordinate function names and class declarations with the team leader before changing shared interfaces.
6. Upload this folder to your assigned branch (`Member-5`) and create a Pull Request targeting `main`.

## Expected deliverables
- Integrated menu flow.
- Input validation and end-to-end test notes for all three portals.

## Important
The original project currently keeps most implementation in one `src/main.cpp`. The extracted file preserves the relevant existing code, but some classes/functions refer to code assigned to other members. It must be integrated with the other members' contributions and the original data files before compiling the complete application.
