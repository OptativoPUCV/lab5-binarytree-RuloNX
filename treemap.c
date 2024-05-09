#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap* tree = (TreeMap*)malloc(sizeof(TreeMap));
    tree->root = NULL;
    tree->current = NULL;
    tree->lower_than = lower_than;
    return tree;
}

void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode* new_node = createTreeNode(key, value);
    if (tree->root == NULL) {
        tree->root = new_node;
        tree->current = new_node;
        return;
    }
    TreeNode* current = tree->root;
    TreeNode* parent = NULL;
    while (current != NULL) {
        parent = current;
        if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    new_node->parent = parent;
    if (tree->lower_than(key, parent->pair->key)) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }
    tree->current = new_node;
}
TreeNode * minimum(TreeNode * x) {
    if (x == NULL) {
        return NULL;
    }
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}
void removeNode(TreeMap * tree, TreeNode* node) {
    if (node->left == NULL && node->right == NULL) {
        if (node == node->parent->left) {
            node->parent->left = NULL;
        } else {
            node->parent->right = NULL;
        }
        free(node);
    } else if (node->left == NULL || node->right == NULL) {
        TreeNode* child = (node->left != NULL) ? node->left : node->right;
        if (node == tree->root) {
            tree->root = child;
            child->parent = NULL;
        } else {
            if (node == node->parent->left) {
                node->parent->left = child;
            } else {
                node->parent->right = child;
            }
            child->parent = node->parent;
        }
        free(node);
    } else {
        TreeNode* successor = minimum(node->right);
        node->pair = successor->pair;
        removeNode(tree, successor);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* current = tree->root;
    while (current != NULL) {
        if (is_equal(tree, current->pair->key, key)) {
            tree->current = current;
            return current->pair;
        } else if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    tree->current = NULL;
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode* current = tree->root;
    if (current == NULL) {
        return NULL;
    }
    while (current->left != NULL) {
        current = current->left;
    }
    tree->current = current;
    return current->pair;
}


Pair * nextTreeMap(TreeMap * tree) {
    TreeNode* current = tree->current;
    if (current == NULL) {
        return NULL;
    }
    if (current->right != NULL) {
        current = current->right;
        while (current->left != NULL) {
            current = current->left;
        }
    } else {
        TreeNode* parent = current->parent;
        while (parent != NULL && current == parent->right) {
            current = parent;
            parent = parent->parent;
        }
        current = parent;
    }
    tree->current = current;
    if (current != NULL) {
        return current->pair;
    }
    return NULL;
}
