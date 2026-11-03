
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

extern "C" {
#include "../filesystem/vfs.h"
}

using namespace std;

Node* find_child(Node* parent, const char* name)
{
    if (!parent) return NULL;

    Node* temp = parent->child;

    while (temp != NULL)
    {
        if (strcmp(temp->name, name) == 0)
            return temp;

        temp = temp->next;
    }

    return NULL;
}

bool contains(Node* root, Node* target)
{
    if (root == NULL) return false;
    if (root == target) return true;

    Node* temp = root->child;

    while (temp != NULL)
    {
        if (contains(temp, target))
            return true;

        temp = temp->next;
    }

    return false;
}

int main()
{
    char root_name[] = "/";
    Node* root = create_node(root_name, 1, NULL);
    Node* current = root;

    if (root == NULL)
    {
        cout << "Filesystem initialization failed\n";
        return 1;
    }

    string line, command, name;

    cout << "\n===== MiniShell V-Drive =====\n";

    while (true)
    {
        cout << "\nVDrive> ";

        if (!getline(cin, line))
            break;

        stringstream ss(line);
        ss >> command;

        if (command.empty())
            continue;

        if (command == "ls")
        {
            Node* temp = current->child;

            if (!temp)
                cout << "Directory is empty\n";

            while (temp)
            {
                cout << (temp->type ? "[DIR]  " : "[FILE] ")
                     << temp->name << "\n";

                temp = temp->next;
            }
        }

        else if (command == "mkdir")
        {
            ss >> name;

            if (name.empty() || find_child(current, name.c_str()))
            {
                cout << "Invalid name or already exists\n";
                continue;
            }

            Node* node = create_node(name.data(), 1, NULL);

            if (node)
            {
                add_child(current, node);
                cout << "Directory created\n";
            }
        }

        else if (command == "touch")
        {
            ss >> name;

            string data;
            getline(ss, data);

            if (!data.empty() && data[0] == ' ')
                data.erase(0, 1);

            if (name.empty() || find_child(current, name.c_str()))
            {
                cout << "Invalid name or already exists\n";
                continue;
            }

            Node* node = create_node(
                name.data(), 0, const_cast<char*>(data.c_str())
            );

            if (node)
            {
                add_child(current, node);
                cout << "File created\n";
            }
        }

        else if (command == "cd")
        {
            ss >> name;

            if (name == "/")
                current = root;

            else if (name == "..")
            {
                if (current->parent)
                    current = current->parent;
            }

            else
            {
                Node* node = find_child(current, name.c_str());

                if (node && node->type)
                    current = node;
                else
                    cout << "Directory not found\n";
            }
        }

        else if (command == "rm")
        {
            ss >> name;

            Node* node = find_child(current, name.c_str());

            if (!node)
                cout << "File or directory not found\n";

            else if (contains(node, current))
                cout << "Cannot delete current directory or its parent\n";

            else
            {
                remove_node(node);
                cout << "Deleted successfully\n";
            }
        }
       else if (command == "load")
{
    ss >> name;

    if (name.empty())
        name = "backup.dat";

    Node *loaded_root = load_filesystem(name.c_str());

    if (loaded_root != NULL)
    {
        free_tree(root);
        root = loaded_root;
        current = root;

        cout << "Filesystem restored successfully\n";
    }
    else
    {
        cout << "Unable to restore filesystem\n";
    }
}
        else if (command == "backup")
        {
            ss >> name;

            if (name.empty())
                name = "backup.dat";

            save_filesystem(root, name.c_str());
        }

        else if (command == "exit")
        {
            cout << "Exiting MiniShell...\n";
            break;
        }

        else
        {
            cout << "Unknown command\n";
        }
    }

    free_tree(root);

    return 0;
}
