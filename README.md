# MiniShell with Virtual File System (VFS)

## Overview
A MiniShell application integrated with a Virtual File System
implemented using C and C++.

## Features
- Directory creation (mkdir)
- File creation (touch)
- Directory navigation (cd)
- File and directory listing (ls)
- File and directory deletion (rm)
- Filesystem backup
- Filesystem restoration (load)
- Nested directory support

## Project Structure

minishell/
├── filesystem/
├── shell/
├── backup/
├── backup.dat
└── README.md

## Technologies
- C
- C++
- GCC
- Linux

## Compilation

gcc -Wall -Wextra -std=c11 -c filesystem/vfs.c -o filesystem/vfs.o

g++ -Wall -Wextra -std=c++17 shell/main.cpp filesystem/vfs.o -o minishell

## Execution

./minishell
