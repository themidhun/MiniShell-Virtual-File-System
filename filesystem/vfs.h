#ifndef VFS_H
#define VFS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
char name[65];
unsigned int type : 1;

struct Node *parent;
struct Node *child;
struct Node *next;
struct Node *prev;

char *content;

} Node;

Node* create_node(char *name, int is_directory, char *content);

void add_child(Node *parent, Node *child);

void remove_node(Node *node);

void free_tree(Node *root);

void save_filesystem(Node *root, const char *filename);

Node* load_filesystem(const char *filename);

#endif
