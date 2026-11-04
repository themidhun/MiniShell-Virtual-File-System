
# MiniShell V-Drive - Test Case Document

## Test Environment
- Operating System: Kali Linux
- Programming Languages: C and C++
- Compiler: GCC and G++
- Project: MiniShell with Virtual File System

## Test Cases

| Sl. No. | Command | Expected Output | Your Output | Result |
|---|---|---|---|---|
| 1 | mkdir docs | Directory created | Directory created | PASS |
| 2 | mkdir project_01 | Directory created | Directory created | PASS |
| 3 | mkdir my-folder | Invalid name | Invalid name or already exists | PASS |
| 4 | mkdir hello.txt | Invalid name | Invalid name or already exists | PASS |
| 5 | mkdir docs (duplicate) | Duplicate name rejected | Invalid name or already exists | PASS |
| 6 | touch note_01 hello | File created | File created | PASS |
| 7 | touch bad@name hello | Invalid name | Invalid name or already exists | PASS |
| 8 | mkdir [64-character name] | Directory created | Directory created | PASS |
| 9 | mkdir [65-character name] | Invalid name | Invalid name or already exists | PASS |
| 10 | cd docs | Change directory | Directory navigation successful | PASS |
| 11 | cd .. | Move to parent directory | Directory navigation successful | PASS |
| 12 | cd nonexistent | Directory not found | Directory not found | PASS |
| 13 | rm nonexistent | File or directory not found | File or directory not found | PASS |
| 14 | ls | Display directory contents | Directory contents displayed | PASS |
| 15 | rm parent (with children) | Recursively delete directory | Deleted successfully | PASS |
| 16 | backup backup.dat | Backup completed | Filesystem backup completed successfully | PASS |
| 17 | load backup.dat after restart | Restore filesystem | Filesystem restored successfully | PASS |
| 18 | load backup/bad.dat | Reject invalid backup | Invalid backup file; Unable to restore filesystem | PASS |
| 19 | Load invalid backup with existing files | Preserve current filesystem | Existing files remained intact | PASS |
| 20 | Valgrind comprehensive memory test | No memory errors or leaks | 11 allocations, 11 frees, 0 bytes remaining, 0 errors | PASS |
| 21 | cd parent/child | Navigate nested relative path | Navigation successful | PASS |
| 22 | cd /docs/projects | Navigate nested absolute path | Navigation successful | PASS |
| 23 | cd /docs/wrong | Reject invalid path | Directory not found | PASS |
| 24 | cd .. | Navigate to parent directory | Navigation successful | PASS |
## Conclusion

The tested MiniShell V-Drive operations passed the recorded test cases. 
