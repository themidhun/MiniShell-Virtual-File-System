
# MiniShell with Virtual File System (V-Drive)

## 1. Overview

MiniShell with Virtual File System is a command-line application that simulates a hierarchical UNIX-like file system entirely in RAM.

The project is developed using C and C++, with a modular architecture separating the shell interface from the core Virtual File System engine.

It demonstrates:
- Dynamic memory management
- Tree data structures
- Doubly linked lists
- Command parsing
- Binary file persistence
- Recursive deletion

## 2. System Architecture

The project consists of two main modules.

### MiniShell Layer (C++)
- Accepts user commands
- Parses command input
- Maintains the current working directory
- Handles user interaction and errors

### Virtual File System Engine (C)
- Creates and deletes nodes
- Maintains hierarchical relationships
- Manages dynamic memory
- Saves and restores filesystem state

## 3. Features

| Command | Description |
|---|---|
| `ls` | Lists files and directories |
| `mkdir <name>` | Creates a directory |
| `touch <name> <data>` | Creates a file with content |
| `cd <path>` | Changes the current directory |
| `rm <name>` | Deletes a file or directory |
| `backup <filename>` | Saves the filesystem in binary format |
| `load <filename>` | Restores a saved filesystem |
| `exit` | Exits the shell and releases memory |

### Name Validation
- Maximum name length: 64 characters
- Supports alphabets and numbers
- Underscore (`_`) is allowed
- Duplicate names are rejected within a directory

## 4. Data Structure

Each filesystem node maintains:

- Name
- File or directory type
- Parent pointer
- Child pointer
- Next sibling pointer
- Previous sibling pointer
- Dynamically allocated file content

The filesystem uses a hierarchical tree structure with doubly linked sibling relationships.

## 5. Memory Management

- Dynamic allocation using `malloc()`
- Memory release using `free()`
- Recursive directory deletion
- Recursive filesystem cleanup
- Allocation failure checks

## 6. Persistence

The filesystem supports binary backup and restoration.

- `backup backup.dat` saves the complete filesystem.
- `load backup.dat` restores the saved hierarchy and file contents.
- Persistence can be tested across program restarts.

## 7. Project Structure

```text
minishell/
├── filesystem/
│   ├── vfs.c
│   ├── vfs.h
│   └── vfs.o
├── shell/
│   └── main.cpp
├── backup/
├── README.md
└── minishell
```

## 8. Technologies Used

- C
- C++
- GCC / G++
- Linux
- Valgrind

## 9. Compilation

Compile the C VFS engine:

```bash
gcc -Wall -Wextra -Wpedantic -std=c11 \
-c filesystem/vfs.c -o filesystem/vfs.o
```

Compile the C++ shell:

```bash
g++ -Wall -Wextra -Wpedantic -std=c++17 \
shell/main.cpp filesystem/vfs.o -o minishell
```

## 10. Execution

```bash
./minishell
```

## 11. Memory Testing

Run Valgrind:

```bash
valgrind --leak-check=full --track-origins=yes ./minishell
```

The tested operations completed with zero reported memory leaks and zero errors in the recorded Valgrind test.

## 12. Testing

Tested operations include:

- Directory and file creation
- Invalid filename rejection
- Duplicate-name prevention
- Doubly linked list deletion
- Recursive directory deletion
- Directory navigation
- Binary backup and restoration after restart
- Corrupted backup rejection
- Preservation of existing filesystem after failed restoration
- Memory cleanup using Valgrind
