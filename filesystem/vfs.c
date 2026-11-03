
#include "vfs.h"
#include<stdint.h>

Node* create_node(char *name, int  is_directory,char*content)
{
Node *new_node;

new_node = malloc(sizeof(Node));
if(new_node == NULL)
{
printf("Memory allocation failed\n");
return NULL;
}
strcpy(new_node -> name,name);
new_node -> type = is_directory;
new_node -> parent = NULL;
new_node -> child = NULL;
new_node -> next = NULL;
new_node -> prev = NULL;

new_node -> content = NULL;
if(! is_directory && content != NULL)
{
new_node -> content = malloc(strlen(content)+1);
if(new_node -> content == NULL)
{
free(new_node);
return NULL;
}
strcpy(new_node -> content,content);
}
return new_node;
}
void add_child(Node*parent,Node*child)
{
if(parent == NULL || child == NULL)
return;
child -> parent = parent;
if(parent -> child == NULL)
{
parent -> child = child;
}
else
{
Node *temp = parent -> child;
while(temp -> next != NULL)
{
temp = temp -> next;
}
temp -> next = child;
child -> prev = temp;
}
}
void remove_node(Node *node)
{
if (node == NULL)
return;
if (node->prev != NULL)
{
node->prev->next = node->next;
}
else if (node->parent != NULL)
{
node->parent->child = node->next;
}
if (node->next != NULL)
{
node->next->prev = node->prev;
}
Node *temp = node->child;

while (temp != NULL)
{
Node *next_child = temp->next;
remove_node(temp);
temp = next_child;
}

free(node->content);
free(node);
}
void free_tree(Node *root)
{
Node*current;
Node*next_node;
if(root == NULL)
return;
current = root -> child;
while(current != NULL)
{
next_node = current -> next;
free_tree(current);
current = next_node;
}
free(root -> content);
free(root);
}
static int write_node(FILE *fp, Node *node)
{
uint32_t content_length = 0;
uint32_t child_count = 0;
uint8_t type;

if (node == NULL)
return 0;

type = node->type;

if (node->content != NULL)
content_length = strlen(node->content);

Node *temp = node->child;

while (temp != NULL)
{
child_count++;
temp = temp->next;
}
if (fwrite(node->name, sizeof(node->name), 1, fp) != 1)
return 0;

if (fwrite(&type, sizeof(type), 1, fp) != 1)
return 0;

if (fwrite(&content_length, sizeof(content_length), 1, fp) != 1)
return 0;

if (content_length > 0)
{
if (fwrite(node->content, 1, content_length, fp) != content_length)
return 0;
}

if (fwrite(&child_count, sizeof(child_count), 1, fp) != 1)
return 0;

temp = node->child;

while (temp != NULL)
{
if (!write_node(fp, temp))
return 0;

temp = temp->next;
}

return 1;
}
void save_filesystem(Node *root, const char *filename)
{
FILE *fp;

if (root == NULL || filename == NULL)
{
printf("Invalid filesystem or filename\n");
return;
}

fp = fopen(filename, "wb");

if (fp == NULL)
{
printf("Unable to create backup file\n");
return;
}

if (fwrite("VDR1", 1, 4, fp) != 4)
{
printf("Backup failed\n");
fclose(fp);
return;
}

if (!write_node(fp, root))
{
printf("Backup failed\n");
fclose(fp);
return;
}

fclose(fp);
printf("Filesystem backup completed successfully!\n");
}
static Node* read_node(FILE *fp)
{
char name[65];
char *content = NULL;

uint8_t type;
uint32_t content_length;
uint32_t child_count;

Node *new_node;
Node *child;

if (fread(name, sizeof(name), 1, fp) != 1)
return NULL;

name[64] = '\0';

if (fread(&type, sizeof(type), 1, fp) != 1)
return NULL;

if (type > 1)
return NULL;

if (fread(&content_length, sizeof(content_length), 1, fp) != 1)
return NULL;

if (content_length > 16 * 1024 * 1024)
return NULL;

if (content_length > 0)
{
if (type == 1)
return NULL;

content = malloc(content_length + 1);

if (content == NULL)
return NULL;

if (fread(content, 1, content_length, fp) != content_length)
{
free(content);
return NULL;
}

content[content_length] = '\0';
}

if (fread(&child_count, sizeof(child_count), 1, fp) != 1)
{
free(content);
return NULL;
}

new_node = create_node(name, type, content);

free(content);

if (new_node == NULL)
return NULL;

for (uint32_t i = 0; i < child_count; i++)
{
child = read_node(fp);

if (child == NULL)
{
free_tree(new_node);
return NULL;
}

add_child(new_node, child);
}

return new_node;
}
Node* load_filesystem(const char *filename)
{
FILE *fp;
char magic[4];
Node *root;

if (filename == NULL)
return NULL;

fp = fopen(filename, "rb");

if (fp == NULL)
{
printf("Unable to open backup file\n");
return NULL;
}

if (fread(magic, 1, 4, fp) != 4 ||
memcmp(magic, "VDR1", 4) != 0)
{
printf("Invalid backup file\n");
fclose(fp);
return NULL;
}

root = read_node(fp);

fclose(fp);

if (root == NULL)
{
printf("Failed to restore filesystem\n");
return NULL;
}

printf("Filesystem restored successfully!\n");

return root;
}
