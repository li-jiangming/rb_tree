/*
 * rb_tree.c
 *
 *  Created on: 2023/8/28
 *      Author: ljm
 */

#include "rb_tree.h"

#include <ab_utils/ab_assert.h>
#include <ab_utils/ab_mem.h>


#include <stdlib.h>
#include <stdio.h>

#define T rb_tree_t

enum rb_tree_node_color_e {
    RB_TREE_NODE_COLOR_RED,
    RB_TREE_NODE_COLOR_BLACK
};

struct rb_node_t {
    struct rb_node_t *right;
    struct rb_node_t *left;
    struct rb_node_t *parent;
    int key;
    enum rb_tree_node_color_e color;
};

struct T {
    struct rb_node_t *root;
    struct rb_node_t *nil;
};

static struct rb_node_t *binary_tree_node_init(T t);
static void binary_tree_node_destory(T t, struct rb_node_t *node);
static void binary_tree_inorder_walk(T t, struct rb_node_t *node);
static void rb_tree_insert_fixup(T t, struct rb_node_t *z);
static void rb_tree_delete_fixup(T t, struct rb_node_t *z);
static void left_rotate(T t, struct rb_node_t *x);
static struct rb_node_t *tree_minimum(struct rb_node_t *node);
static struct rb_node_t *tree_maximum(struct rb_node_t *node);
static struct rb_node_t *rbtree_search(struct rb_node_t *node, int key);
static void rb_transplant(T t, struct rb_node_t *u, struct rb_node_t *v);

T rb_tree_new(void)
{
    T result;
    NEW(result);
    if (result != NULL) {
        NEW(result->nil);
        if (result->nil != NULL) {
            result->nil->color = RB_TREE_NODE_COLOR_BLACK;
            result->nil->key = 0;
            result->nil->parent = result->nil->left = result->nil->right = result->nil;
        }

        result->root = result->nil;
    }

    return result;
}

void rb_tree_free(T *t)
{
    assert(t && *t);

    FREE((*t)->nil);
    FREE(*t);
}

void rb_tree_insert(T t, int key)
{
    struct rb_node_t *z = binary_tree_node_init(t);
    z->key = key;

    struct rb_node_t *x = t->root, *y = t->nil;
    while (x != t->nil) {
        y = x;
        if (z->key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    z->parent = y;
    if (y == t->nil) {
        t->root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }

    z->left = t->nil;
    z->right = t->nil;
    z->color = RB_TREE_NODE_COLOR_RED;
    rb_tree_insert_fixup(t, z);
}

void rb_tree_delete(T t, int key)
{
    struct rb_node_t *z = t->nil;
    if ((z = rbtree_search(t->root, key)) != t->nil) {
        struct rb_node_t *x = t->nil;
        struct rb_node_t *y = z;
        enum rb_tree_node_color_e y_orginal_color;
        y_orginal_color = y->color;
        if (z->left == t->nil) {
            x = z->right;
            rb_transplant(t, z, z->right);
        } else if(z->right == t->nil) {
            x = z->left;
            rb_transplant(t, z, z->left);
        } else {
            y = tree_minimum(z->right);
            y_orginal_color = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            } else {
                rb_transplant(t, y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            rb_transplant(t, z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        if (y_orginal_color == RB_TREE_NODE_COLOR_BLACK) {
            rb_tree_delete_fixup(t, x);
        }

        FREE(z);
    } else {
        fprintf(stderr, "can't find %d\n", key);
    }
}

void rb_tree_inorder_walk(T t)
{
    assert(t);
    binary_tree_inorder_walk(t, t->root);
}

int32_t rb_tree_minimum(T t)
{
    int32_t result = INT32_MAX;

    struct rb_node_t *node = tree_minimum(t->root);
    if (node != t->nil) {
        result = node->key;
    }

    return result;
}

int32_t rb_tree_maximum(T t)
{
    int32_t result = INT32_MIN;

    struct rb_node_t *node = tree_maximum(t->root);
    if (node != t->nil) {
        result = node->key;
    }

    return result;
}

struct rb_node_t *tree_minimum(struct rb_node_t *node)
{
    struct rb_node_t *result = node;

    while (result->left->left != result->left) {
        result = result->left;
    }

    return result;
}

struct rb_node_t *tree_maximum(struct rb_node_t *node)
{
    struct rb_node_t *result = node;

    while (result->right->right != result->right) {
        result = result->right;
    }

    return result;
}

struct rb_node_t *rbtree_search(struct rb_node_t *node, int key)
{
    struct rb_node_t *result = node;

    if (result->left == result || result->right == result || key == result->key) {
        return result;
    }

    if (key < result->key) {
        return(rbtree_search(result->left, key));
    } else {
        return(rbtree_search(result->right, key));
    }
}

void left_rotate(T t, struct rb_node_t *x)
{
    struct rb_node_t *y = x->right;
    x->right = y->left;
    if (y->left != t->nil) {
        y->left->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == t->nil) {
        t->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void right_rotate(T t, struct rb_node_t *y)
{
    struct rb_node_t *x = y->left;
    y->left = x->right;
    if (x->right != t->nil) {
        x->right->parent = y;
    }

    x->parent = y->parent;
    if (y->parent == t->nil) {
        t->root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }

    x->right = y;
    y->parent = x;
}

void rb_tree_insert_fixup(T t, struct rb_node_t *z)
{
    struct rb_node_t *y;

    while (z->parent->color == RB_TREE_NODE_COLOR_RED) {
        if (z->parent->parent == t->nil) {
            break;
        }
        
        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;
            if (y->color == RB_TREE_NODE_COLOR_RED) {
                z->parent->color = RB_TREE_NODE_COLOR_BLACK; 
                y->color = RB_TREE_NODE_COLOR_BLACK;
                z->parent->parent->color = RB_TREE_NODE_COLOR_RED;
                z = z->parent->parent;
            } else if (z == z->parent->right) {
                z = z->parent;
                left_rotate(t, z);
            } else {
                z->parent->color = RB_TREE_NODE_COLOR_BLACK;
                z->parent->parent->color = RB_TREE_NODE_COLOR_RED;
                right_rotate(t, z->parent->parent);
            }
        } else {
            y = z->parent->parent->left;
            if (y->color == RB_TREE_NODE_COLOR_RED) {
                z->parent->color = RB_TREE_NODE_COLOR_BLACK;
                y->color = RB_TREE_NODE_COLOR_BLACK;
                z->parent->parent->color = RB_TREE_NODE_COLOR_RED;
                z = z->parent->parent;
            } else if (z == z->parent->left) {
                z = z->parent;
                right_rotate(t, z);
            } else {
                z->parent->color = RB_TREE_NODE_COLOR_BLACK;
                z->parent->parent->color = RB_TREE_NODE_COLOR_RED;
                left_rotate(t, z->parent->parent);
            }
        }

        t->root->color = RB_TREE_NODE_COLOR_BLACK;
    }
}

void rb_transplant(T t, struct rb_node_t *u, struct rb_node_t *v)
{
    if (u->parent == t->nil) {
        t->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }

    v->parent = u->parent;
}

void rb_tree_delete_fixup(T t, struct rb_node_t *z)
{
    struct rb_node_t *w = t->nil;
    while (z != t->root && z->color==RB_TREE_NODE_COLOR_BLACK) {
        if(z == z->parent->left) {
            w = z->parent->right;
            if(w->color == RB_TREE_NODE_COLOR_RED) {
                w->color = RB_TREE_NODE_COLOR_BLACK;
                w->parent->color = RB_TREE_NODE_COLOR_RED;
                left_rotate(t, z->parent);
                w = z->parent->right;
            }
            
            if (w->left->color == RB_TREE_NODE_COLOR_BLACK && w->right->color == RB_TREE_NODE_COLOR_BLACK) {
                w->color = RB_TREE_NODE_COLOR_RED;
                z = z->parent;
            } else {
                if(w->right->color == RB_TREE_NODE_COLOR_BLACK) {
                    w->left->color = RB_TREE_NODE_COLOR_BLACK;
                    w->color = RB_TREE_NODE_COLOR_RED;
                    right_rotate(t, w);
                    w = z->parent->right;
                }

                w->color = z->parent->color;
                z->parent->color = RB_TREE_NODE_COLOR_BLACK;
                w->right->color = RB_TREE_NODE_COLOR_BLACK;
                left_rotate(t, z->parent);
                z = t->root;
            }
        } else {
            w = z->parent->left;
            if(w->color == RB_TREE_NODE_COLOR_RED) {
                w->color = RB_TREE_NODE_COLOR_BLACK;
                z->parent->color = RB_TREE_NODE_COLOR_RED;
                right_rotate(t, z->parent);
                w = z->parent->left;
            }
        }

        if(w->left->color == RB_TREE_NODE_COLOR_BLACK && w->right->color == RB_TREE_NODE_COLOR_BLACK) {
            w->color = RB_TREE_NODE_COLOR_RED;
            z = z->parent;
        } else {
            if(w->left->color == RB_TREE_NODE_COLOR_BLACK) {
                w->right->color = RB_TREE_NODE_COLOR_BLACK;
                w->color = RB_TREE_NODE_COLOR_RED;
                left_rotate(t, w);
                w = z->parent->left;
            }

            w->color = z->parent->color;
            z->parent->color = RB_TREE_NODE_COLOR_BLACK;
            w->left->color = RB_TREE_NODE_COLOR_BLACK;
            right_rotate(t, z->parent);
            z = t->root;
        }
    }
 
    z->color = RB_TREE_NODE_COLOR_BLACK;
}

struct rb_node_t *binary_tree_node_init(T t)
{
    struct rb_node_t *result;
    NEW(result);
    if (result != NULL) {
        result->color = RB_TREE_NODE_COLOR_BLACK;
        result->key = 0;
        result->right = result->left = result->parent = t->nil;
    }

    return result;
}

void binary_tree_node_destory(T t, struct rb_node_t *node)
{
    if (node != t->nil) {
        binary_tree_node_destory(t, node->left);
        binary_tree_node_destory(t, node->right);
        FREE(node);
    }
}

void binary_tree_inorder_walk(T t, struct rb_node_t *node)
{
    if (node != t->nil) {
        binary_tree_inorder_walk(t, node->left);
        printf("%d ", node->key);
        binary_tree_inorder_walk(t, node->right);
    }
}
