# Member-1 - User and Student Management

## Your responsibility
- Maintain the `User` base class and `Student` derived class.
- Handle student identity, name and contact information.

## Files
- `Member-1.cpp` - your actual assigned C++ code extracted from the team's original `src/main.cpp`.

## Dependencies / integration notes
The `Student` class is stored and accessed by `QueueManager` in Member-4 code. Keep the constructor and getter names (`getId`, `getName`, `getContact`) compatible.

## What to do before uploading to GitHub
1. Open `Member-1.cpp` and understand the assigned code.
2. Make changes only to your assigned responsibility.
3. Do not create another `main()` function.
4. Do not overwrite the complete project `src/main.cpp` with this contribution file.
5. Coordinate function names and class declarations with the team leader before changing shared interfaces.
6. Upload this folder to your assigned branch (`Member-1`) and create a Pull Request targeting `main`.

## Expected deliverables
- Updated `User` / `Student` implementation.
- Any agreed validation for student details.

## Important
The original project currently keeps most implementation in one `src/main.cpp`. The extracted file preserves the relevant existing code, but some classes/functions refer to code assigned to other members. It must be integrated with the other members' contributions and the original data files before compiling the complete application.
