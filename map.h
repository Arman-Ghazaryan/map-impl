#pragma once
#include <cassert>

namespace dts
{
    template <typename key, typename value>
    class map
    {
        using value_type = std::pair<const key, value>;

    public:
        map();
        ~map();
        class node
        {
        public:
            node* left = nullptr;
            node* right = nullptr;
            node* parent = nullptr;
            char clr = ' ';
            value_type* pair_obj = nullptr;
            node();
            node(const value_type& obj);
            ~node();
        };

        

        void insert(value_type&& obj);

        node* begin() { return root; }

        value& operator[](const key KEY);
        // node *find_position(node *nod, key ky);
        // void delete_node(node *nod);
        // void delete_all_node(node *nod);

       

        void erase(key KEY);

    private:
        node* root;
        size_t Size;

        void rnext(node* temp, node* new_node);
        void lnext(node* temp, node* new_node);

        node* grandparent(node* nod);
        node* uncle(node* nod);

        void rotate_right(node* nod);
        void rotate_left(node* nod);

        void insert_case1(node* nod);
        void insert_case2(node* nod);
        void insert_case3(node* nod);
        void insert_case4(node* nod);
        void insert_case5(node* nod);

        node* sibling(node* nod);
        void replace_node(node* nod, node* child);
        void delete_one_child(node* nod);

        void delete_case1(node* nod);
        void delete_case2(node* nod);
        void delete_case3(node* nod);
        void delete_case4(node* nod);
        void delete_case5(node* nod);
        void delete_case6(node* nod);

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

    template<typename key, typename value>
    map<key, value>::node::node()
    {
    }

    template <typename key, typename value>
    map<key, value>::node::node(const value_type& obj) : pair_obj(new value_type(obj)) {}

    template<typename key, typename value>
    map<key, value>::node::~node()
    {
    }

    template <typename key, typename value>
    typename map<key, value>::node* map<key, value>::grandparent(node* nod)
    {
        if ((nod != nullptr) && (nod->parent != nullptr))
            return nod->parent->parent;
        else
            return nullptr;
    }

    template <typename key, typename value>
    typename map<key, value>::node* map<key, value>::uncle(node* nod)
    {
        node* g = grandparent(nod);
        if (g == nullptr)
            return nullptr;
        if (nod->parent == g->left)
            return g->right;
        else
            return g->left;
    }

    template <typename key, typename value>
    void map<key, value>::rotate_left(node* nod)
    {
        node* pivot = nod->right;

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
    void map<key, value>::rotate_right(node* nod)
    {
        node* pivot = nod->left;

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
    void map<key, value>::insert_case1(node* nod)
    {
        if (nod->parent == nullptr)
            nod->clr = 'b';
        else
            insert_case2(nod);
    }

    template <typename key, typename value>
    void map<key, value>::insert_case2(node* nod)
    {
        if (nod->parent->clr == 'b')
            return;
        else
            insert_case3(nod);
    }

    template <typename key, typename value>
    void map<key, value>::insert_case3(node* nod)
    {
        node* u = uncle(nod), * g;

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
    void map<key, value>::insert_case4(node* nod)
    {
        node* g = grandparent(nod);

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
    void map<key, value>::insert_case5(node* nod)
    {
        node* g = grandparent(nod);

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
    void map<key, value>::insert(value_type&& obj)
    {
        node* temp = root;
        node* new_node = new node(obj);
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
    value& map<key, value>::operator[](const key KEY)
    {
        node* temp = root;
        while (true)
        {
            assert(temp != nullptr);
            if (KEY == temp->pair_obj->first)
            {
                return temp->pair_obj->second;
            }
            if (KEY < temp->pair_obj->first)
            {
                temp = temp->left;
            }
            if (KEY > temp->pair_obj->first)
            {
                temp = temp->right;
            }
        }
    }

    template<typename key, typename value>
    void map<key, value>::delete_case1(node* nod)
    {
        if (nod->parent != nullptr)
            delete_case2(nod);
    }

    template<typename key, typename value>
    void map<key, value>::delete_case2(node* nod)
    {
        node* s = sibling(nod);

        if (s->clr == 'r') {
            nod->parent->clr = 'r';
            s->clr = 'b';
            if (nod == nod->parent->left)
                rotate_left(nod->parent);
            else
                rotate_right(nod->parent);
        }
        delete_case3(nod);
    }

    template<typename key, typename value>
    void map<key, value>::delete_case3(node* nod)
    {
        node* s = sibling(nod);

        if ((nod->parent->clr == 'b') && (s->clr == 'b') && (s->left->clr == 'b') && (s->right->clr == 'b'))
        {
            s->clr = 'r';
            delete_case1(nod->parent);
        }
        else
            delete_case4(nod);
    }

    template<typename key, typename value>
    void map<key, value>::delete_case4(node* nod)
    {
        node* s = sibling(nod);

        if ((nod->parent->clr == 'r') && (s->clr == 'b') && (s->left->clr == 'b') && (s->right->clr == 'b'))
        {
            s->clr = 'r';
            nod->parent->clr = 'b';
        }
        else
            delete_case5(nod);
    }

    template<typename key, typename value>
    void map<key, value>::delete_case5(node* nod)
    {
        node* s = sibling(nod);

        if (s->clr == 'b') {
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

    template<typename key, typename value>
    void map<key, value>::delete_case6(node* nod)
    {
        node* s = sibling(nod);

        s->clr = nod->parent->clr;
        nod->parent->clr = 'b';

        if (nod == nod->parent->left) {
            s->right->clr = 'b';
            rotate_left(nod->parent);
        }
        else {
            s->left->clr = 'b';
            rotate_right(nod->parent);
        }
    }

    template<typename key, typename value>
    map<key, value>::node* map<key, value>::sibling(node* nod)
    {
        if (nod == nod->parent->left)
            return nod->parent->right;
        else
            return nod->parent->left;
    }

    template<typename key, typename value>
    void map<key, value>::replace_node(node* nod, node* child)
    {
        child->parent = nod->parent;
        if (nod == nod->parent->left) {
            nod->parent->left = child;
        }
        else {
            nod->parent->right = child;
        }
    }

    template<typename key, typename value>
    void map<key, value>::delete_one_child(node* nod)
    {
        node* child = (nod->right == nullptr) ? nod->left : nod->right;

        replace_node(nod, child);
        if (nod->clr == 'b') {
            if (child->clr == 'r')
                child->clr = 'b';
            else
                delete_case1(child);
        }
        free(nod);
    }

    template<typename key, typename value>
    void map<key, value>::erase(key KEY)
    {
        node* temp = root;
        if (KEY == temp->pair_obj->first)
        {

        }
        while (true)
        {
            if (KEY < temp->pair_obj->first)
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
            else if (KEY > temp->pair_obj->first)
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

    template<typename key, typename value>
    void map<key, value>::rnext(node* temp, node* new_node)
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

    template<typename key, typename value>
    void map<key, value>::lnext(node* temp, node* new_node)
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
