
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cctype>

extern "C" {
#include "../filesystem/vfs.h"
}

using namespace std;

bool is_valid_name(const string& name)
{
    if (name.empty() || name.length() > 64)
        return false;

    for (char ch : name)
    {
        if (!isalnum(static_cast<unsigned char>(ch)) && ch != '_')
            return false;
    }

    return true;
}

Node* find_child(Node* parent, const char* name)
{
    if (parent == NULL)
        return NULL;

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
    if (root == NULL)
        return false;

    if (root == target)
        return true;

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

    if (root == NULL)
    {
        cout << "Filesystem initialization failed\n";
        return 1;
    }

    Node* current = root;

    string line, command, name;

    cout << "\n===== MiniShell V-Drive =====\n";

    while (true)
    {
        cout << "\nVDrive> ";

        if (!getline(cin, line))
            break;

        stringstream ss(line);

        command.clear();
        name.clear();

        ss >> command;

        if (command.empty())
            continue;

        if (command == "ls")
        {
            Node* temp = current->child;

            if (temp == NULL)
                cout << "Directory is empty\n";

            while (temp != NULL)
            {
                cout << (temp->type ? "[DIR]  " : "[FILE] ")
                     << temp->name << "\n";

                temp = temp->next;
            }
        }

        else if (command == "mkdir")
        {
            ss >> name;

            if (!is_valid_name(name) ||
                find_child(current, name.c_str()))
            {
                cout << "Invalid name or already exists\n";
                continue;
            }

            Node* node = create_node(name.data(), 1, NULL);

            if (node != NULL)
            {
                add_child(current, node);
                cout << "Directory created\n";
            }
            else
            {
                cout << "Directory creation failed\n";
            }
        }

        else if (command == "touch")
        {
            ss >> name;

            string data;
            getline(ss, data);

            if (!data.empty() && data[0] == ' ')
                data.erase(0, 1);

            if (!is_valid_name(name) ||
                find_child(current, name.c_str()))
            {
                cout << "Invalid name or already exists\n";
                continue;
            }

            Node* node = create_node(
                name.data(), 0, const_cast<char*>(data.c_str())
            );

            if (node != NULL)
            {
                add_child(current, node);
                cout << "File created\n";
            }
            else
            {
                cout << "File creation failed\n";
            }
        }


else if (command == "cd")
{
    ss >> name;

    if (name.empty())
    {
        cout << "Please provide a path\n";
        continue;
    }

    Node* target = (name[0] == '/') ? root : current;
    string part;
    stringstream path_stream(name);
    bool valid_path = true;

    while (getline(path_stream, part, '/'))
    {
        if (part.empty() || part == ".")
            continue;

        if (part == "..")
        {
            if (target->parent != NULL)
                target = target->parent;

            continue;
        }

        Node* node = find_child(target, part.c_str());

        if (node == NULL || node->type != 1)
        {
            valid_path = false;
            break;
        }

        target = node;
    }

    if (valid_path)
{
        current = target;
}
    else
{
        cout << "Directory not found\n";
        }
     }
        else if (command == "rm")
        {
            ss >> name;

            if (name.empty())
            {
                cout << "Please provide a name\n";
                continue;
            }

            Node* node = find_child(current, name.c_str());

            if (node == NULL)
            {
                cout << "File or directory not found\n";
            }
            else if (contains(node, current))
            {
                cout << "Cannot delete current directory or its parent\n";
            }
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

            Node* loaded_root = load_filesystem(name.c_str());

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

