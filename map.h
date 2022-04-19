#pragma once
#include <cassert>
#include <string>
#include "hash-library/sha256.h"

namespace dts
{
    template <typename key, typename value>
    class map
    {
        using node_value_type = std::pair<const std::string, value>;
        using value_type = std::pair<const key, value>;
        SHA256 hash;

    public:
        map();
        ~map();
        class node
        {
        public:
            node *left = nullptr;
            node *right = nullptr;
            node *parent = nullptr;
            char clr = ' ';
            node_value_type *pair_obj = nullptr;
            node();
            node(const node_value_type &obj);
            ~node();
        };

        void insert(value_type &&obj);

        node *begin() { return root; }

        value &operator[](const key KEY);
        // node *find_position(node *nod, key ky);
        // void delete_node(node *nod);
        // void delete_all_node(node *nod);

        void erase(key KEY);

    private:
        node *root;
        size_t Size;

        void rnext(node *temp, node *new_node);
        void lnext(node *temp, node *new_node);

        node *grandparent(node *nod);
        node *uncle(node *nod);

        void rotate_right(node *nod);
        void rotate_left(node *nod);

        void insert_case1(node *nod);
        void insert_case2(node *nod);
        void insert_case3(node *nod);
        void insert_case4(node *nod);
        void insert_case5(node *nod);

        node *sibling(node *nod);
        void replace_node(node *nod, node *child);
        void delete_one_child(node *nod);

        void delete_case1(node *nod);
        void delete_case2(node *nod);
        void delete_case3(node *nod);
        void delete_case4(node *nod);
        void delete_case5(node *nod);
        void delete_case6(node *nod);
    };

    template <typename key, typename value>
    map<key, value>::map()
    {
        root = nullptr;
        Size = 0;
    }

    template <typename key, typename value>
    map<key, value>::~map()
    {
        root = nullptr;
        Size = 0;
    }

    template <typename key, typename value>
    map<key, value>::node::node()
    {
    }

    template <typename key, typename value>
    map<key, value>::node::node(const node_value_type &obj) : pair_obj(new node_value_type(obj)) {}

    template <typename key, typename value>
    map<key, value>::node::~node()
    {
        left = nullptr;
        right = nullptr;
        parent = nullptr;
        clr = ' ';
        pair_obj = nullptr;
    }

    template <typename key, typename value>
    typename map<key, value>::node *map<key, value>::grandparent(node *nod)
    {
        if ((nod != nullptr) && (nod->parent != nullptr))
            return nod->parent->parent;
        else
            return nullptr;
    }

    template <typename key, typename value>
    typename map<key, value>::node *map<key, value>::uncle(node *nod)
    {
        node *g = grandparent(nod);
        if (g == nullptr)
            return nullptr;
        if (nod->parent == g->left)
            return g->right;
        else
            return g->left;
    }

    template <typename key, typename value>
    void map<key, value>::rotate_left(node *nod)
    {
        node *pivot = nod->right;

        pivot->parent = nod->parent;
        if (nod->parent != nullptr)
        {
            if (nod->parent->left == nod)
                nod->parent->left = pivot;
            else
                nod->parent->right = pivot;
        }

        nod->right = pivot->left;
        if (pivot->left != nullptr)
            pivot->left->parent = nod;

        nod->parent = pivot;
        pivot->left = nod;
    }

    template <typename key, typename value>
    void map<key, value>::rotate_right(node *nod)
    {
        node *pivot = nod->left;

        pivot->parent = nod->parent;
        if (nod->parent != nullptr)
        {
            if (nod->parent->left == nod)
                nod->parent->left = pivot;
            else
                nod->parent->right = pivot;
        }

        nod->left = pivot->right;
        if (pivot->right != nullptr)
            pivot->right->parent = nod;

        nod->parent = pivot;
        pivot->right = nod;
    }

    template <typename key, typename value>
    void map<key, value>::insert_case1(node *nod)
    {
        if (nod->parent == nullptr)
            nod->clr = 'b';
        else
            insert_case2(nod);
    }

    template <typename key, typename value>
    void map<key, value>::insert_case2(node *nod)
    {
        if (nod->parent->clr == 'b')
            return;
        else
            insert_case3(nod);
    }

    template <typename key, typename value>
    void map<key, value>::insert_case3(node *nod)
    {
        node *u = uncle(nod), *g;

        if ((u != nullptr) && (u->clr == 'r'))
        {
            nod->parent->clr = 'b';
            u->clr = 'b';
            g = grandparent(nod);
            g->clr = 'r';
            insert_case1(g);
        }
        else
        {
            insert_case4(nod);
        }
    }

    template <typename key, typename value>
    void map<key, value>::insert_case4(node *nod)
    {
        node *g = grandparent(nod);

        if ((nod == nod->parent->right) && (nod->parent == g->left))
        {
            rotate_left(nod->parent);
            nod = nod->left;
        }
        else if ((nod == nod->parent->left) && (nod->parent == g->right))
        {
            rotate_right(nod->parent);
            nod = nod->right;
        }
        insert_case5(nod);
    }

    template <typename key, typename value>
    void map<key, value>::insert_case5(node *nod)
    {
        node *g = grandparent(nod);

        nod->parent->clr = 'b';
        g->clr = 'r';
        if ((nod == nod->parent->left) && (nod->parent == g->left))
        {
            rotate_right(g);
        }
        else
        {
            rotate_left(g);
        }
        while (root->parent != nullptr)
            root = root->parent;
    }

    template <typename key, typename value>
    void map<key, value>::insert(value_type &&obj)
    {
        node *temp = root;
        std::string hash_key =  hash(std::to_string(obj.first));
        node_value_type new_obj(hash_key, obj.second);
        node *new_node = new node;
        new_node->pair_obj = &new_obj;
        new_node->clr = 'r';

        if (!Size)
        {
            root = new_node;
            insert_case1(root);
        }
        else
        {
            while (true)
            {
                if (new_node->pair_obj->first < temp->pair_obj->first)
                {
                    if (temp->left == nullptr)
                    {
                        temp->left = new_node;
                        new_node->parent = temp;
                        insert_case1(new_node);
                        break;
                    }
                    else
                    {
                        temp = temp->left;
                        lnext(temp, new_node);
                        rnext(temp, new_node);
                        break;
                    }
                }
                else if (new_node->pair_obj->first > temp->pair_obj->first)
                {
                    if (temp->right == nullptr)
                    {
                        temp->right = new_node;
                        new_node->parent = temp;
                        insert_case1(new_node);
                        break;
                    }
                    else
                    {
                        temp = temp->right;
                        lnext(temp, new_node);
                        rnext(temp, new_node);
                        break;
                    }
                }
            }
        }
        ++Size;
    }

    template <typename key, typename value>
    value &map<key, value>::operator[](const key KEY)
    {

        node *temp = root;
        while (true)
        {
            assert(temp != nullptr);
            if (hash(KEY, 17) == temp->pair_obj->first)
            {
                return temp->pair_obj->second;
            }
            if (hash(KEY, 17) < temp->pair_obj->first)
            {
                temp = temp->left;
            }
            if (hash(KEY, 17) > temp->pair_obj->first)
            {
                temp = temp->right;
            }
        }
    }

    template <typename key, typename value>
    void map<key, value>::delete_case1(node *nod)
    {
        if (nod->parent != nullptr)
            delete_case2(nod);
    }

    template <typename key, typename value>
    void map<key, value>::delete_case2(node *nod)
    {
        node *s = sibling(nod);

        if (s->clr == 'r')
        {
            nod->parent->clr = 'r';
            s->clr = 'b';
            if (nod == nod->parent->left)
                rotate_left(nod->parent);
            else
                rotate_right(nod->parent);
        }
        delete_case3(nod);
    }

    template <typename key, typename value>
    void map<key, value>::delete_case3(node *nod)
    {
        node *s = sibling(nod);

        if ((nod->parent->clr == 'b') && (s->clr == 'b') && (s->left->clr == 'b') && (s->right->clr == 'b'))
        {
            s->clr = 'r';
            delete_case1(nod->parent);
        }
        else
            delete_case4(nod);
    }

    template <typename key, typename value>
    void map<key, value>::delete_case4(node *nod)
    {
        node *s = sibling(nod);

        if ((nod->parent->clr == 'r') && (s->clr == 'b') && (s->left->clr == 'b') && (s->right->clr == 'b'))
        {
            s->clr = 'r';
            nod->parent->clr = 'b';
        }
        else
            delete_case5(nod);
    }

    template <typename key, typename value>
    void map<key, value>::delete_case5(node *nod)
    {
        node *s = sibling(nod);

        if (s->clr == 'b')
        {
            if ((nod == nod->parent->left) && (s->right->clr == 'b') && (s->left->clr == 'r'))
            {
                s->clr = 'r';
                s->left->clr = 'b';
                rotate_right(s);
            }
            else if ((nod == nod->parent->right) && (s->left->clr == 'b') && (s->right->clr == 'r'))
            {
                s->clr = 'r';
                s->right->clr = 'b';
                rotate_left(s);
            }
        }
        delete_case6(nod);
    }

    template <typename key, typename value>
    void map<key, value>::delete_case6(node *nod)
    {
        node *s = sibling(nod);

        s->clr = nod->parent->clr;
        nod->parent->clr = 'b';

        if (nod == nod->parent->left)
        {
            s->right->clr = 'b';
            rotate_left(nod->parent);
        }
        else
        {
            s->left->clr = 'b';
            rotate_right(nod->parent);
        }
    }

    template <typename key, typename value>
    typename map<key, value>::node *map<key, value>::sibling(node *nod)
    {
        if (nod == nod->parent->left)
            return nod->parent->right;
        else
            return nod->parent->left;
    }

    template <typename key, typename value>
    void map<key, value>::replace_node(node *nod, node *child)
    {
        child->parent = nod->parent;
        if (nod == nod->parent->left)
        {
            nod->parent->left = child;
        }
        else
        {
            nod->parent->right = child;
        }
    }

    template <typename key, typename value>
    void map<key, value>::delete_one_child(node *nod)
    {
        node *child = (nod->right == nullptr) ? nod->left : nod->right;

        replace_node(nod, child);
        if (nod->clr == 'b')
        {
            if (child->clr == 'r')
                child->clr = 'b';
            else
                delete_case1(child);
        }
        delete nod;
    }

    template <typename key, typename value>
    void map<key, value>::erase(key KEY)
    {
        node *temp = root, *to_delete, *prnt;
        if (KEY != temp->pair_obj->first)
        {
            while (true)
            {
                if (KEY < temp->pair_obj->first)
                {
                    temp = temp->left;
                    if (temp->pair_obj->first == KEY)
                        break;
                }
                if (KEY > temp->pair_obj->first)
                {
                    temp = temp->right;
                    if (temp->pair_obj->first == KEY)
                        break;
                }
            }
        }
        if ((temp->left == nullptr && temp->right != nullptr) &&
                (temp->right->left == nullptr && temp->right->right == nullptr) ||
            (temp->right == nullptr && temp->right != nullptr) &&
                (temp->left->left == nullptr && temp->left->right == nullptr))
        {
            delete_one_child(temp);
            Size--;
            return;
        }
        else
        {
            if ((temp->left == nullptr) && (temp->right == nullptr))
            {
                if (temp = temp->parent->left)
                    temp->parent->left = nullptr;
                else
                    temp->parent->right = nullptr;
                delete temp;
            }
            else
            {
                to_delete = temp;
                temp = temp->left;

                while (temp->right != nullptr)
                    temp = temp->right;

                // prnt = temp->parent;

                // if (temp->left != nullptr)
                //     temp->left->parent = temp->parent;

                // child = temp->left;

                // if (temp == temp->parent->left)
                //     temp->parent->left = temp->left;
                // else
                //     temp->parent->right = temp->left;

                // // to_delete->pair_obj->first = temp->pair_obj->first;
                // // to_delete->pair_obj->second = temp->pair_obj->second;

                // temp->left = to_delete->left;
                // temp->right = to_delete->right;
                // temp->parent = to_delete->parent;

                // delete to_delete;

                // if (prnt->left == nullptr && prnt->right != nullptr)
                // {

                // }
                // else if (prnt->right == nullptr && prnt->left != nullptr))
                // {

                // }

                // delete_case1(temp);

                // if (temp = temp->parent->left)
                //     temp->parent->left = nullptr;
                // else
                //     temp->parent->right = nullptr;

                // temp->parent = nullptr;
            }
            temp = nullptr;
        }
    }

    template <typename key, typename value>
    void map<key, value>::rnext(node *temp, node *new_node)
    {

        if (new_node->pair_obj->first > temp->pair_obj->first)
        {
            if (temp->right == nullptr)
            {
                temp->right = new_node;
                new_node->parent = temp;
                insert_case1(new_node);
                return;
            }
            else
            {
                temp = temp->right;
                lnext(temp, new_node);
                rnext(temp, new_node);
                return;
            }
        }
    }

    template <typename key, typename value>
    void map<key, value>::lnext(node *temp, node *new_node)
    {
        if (new_node->pair_obj->first < temp->pair_obj->first)
        {
            if (temp->left == nullptr)
            {
                temp->left = new_node;
                new_node->parent = temp;
                insert_case1(new_node);
                return;
            }
            else
            {
                temp = temp->left;
                lnext(temp, new_node);
                rnext(temp, new_node);
                return;
            }
        }
    }
}
