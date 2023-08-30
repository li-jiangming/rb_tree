/*
 * main.c
 *
 *  Created on: 2023/8/28
 *      Author: ljm
 */


#include "rb_tree.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    rb_tree_t rb_tree = rb_tree_new();

    rb_tree_insert(rb_tree, 9);
    rb_tree_insert(rb_tree, 1);
    rb_tree_insert(rb_tree, 2);
    rb_tree_insert(rb_tree, 3);
    rb_tree_insert(rb_tree, 4);
    rb_tree_insert(rb_tree, 7);
    rb_tree_insert(rb_tree, 5);
    rb_tree_insert(rb_tree, 6);
    rb_tree_insert(rb_tree, 8);
    rb_tree_insert(rb_tree, 10);

    rb_tree_inorder_walk(rb_tree);
    printf("\n");

#if 0
    rb_tree_delete(rb_tree, 10);
    rb_tree_inorder_walk(rb_tree);
    printf("\n");

    rb_tree_delete(rb_tree, 9);
    rb_tree_inorder_walk(rb_tree);
    printf("\n");

    rb_tree_delete(rb_tree, 8);
    rb_tree_inorder_walk(rb_tree);
    printf("\n");

    rb_tree_delete(rb_tree, 7);
    rb_tree_inorder_walk(rb_tree);
    printf("\n");

    rb_tree_delete(rb_tree, 6);
    rb_tree_inorder_walk(rb_tree);
    printf("\n");

    rb_tree_delete(rb_tree, 5);
    rb_tree_inorder_walk(rb_tree);
    printf("\n");

    rb_tree_delete(rb_tree, 4);
    rb_tree_inorder_walk(rb_tree);
    printf("\n");

    rb_tree_delete(rb_tree, 3);
    rb_tree_inorder_walk(rb_tree);
    printf("\n");

    rb_tree_delete(rb_tree, 2);
    rb_tree_inorder_walk(rb_tree);
    printf("\n");

    rb_tree_delete(rb_tree, 1);
    rb_tree_inorder_walk(rb_tree);
    printf("\n");
#else
    rb_tree_delete(rb_tree, 10);
    rb_tree_inorder_walk(rb_tree);
    printf("\n");

    rb_tree_delete(rb_tree, 1);
    rb_tree_inorder_walk(rb_tree);
    printf("\n");

    rb_tree_delete(rb_tree, 2);
    rb_tree_inorder_walk(rb_tree);
    printf("\n");

    rb_tree_delete(rb_tree, 3);
    rb_tree_inorder_walk(rb_tree);
    printf("\n");

    rb_tree_delete(rb_tree, 4);
    rb_tree_inorder_walk(rb_tree);
    printf("\n");

    rb_tree_delete(rb_tree, 5);
    rb_tree_inorder_walk(rb_tree);
    printf("\n");

    rb_tree_delete(rb_tree, 6);
    rb_tree_inorder_walk(rb_tree);
    printf("\n");

    rb_tree_delete(rb_tree, 7);
    rb_tree_inorder_walk(rb_tree);
    printf("\n");

    rb_tree_delete(rb_tree, 8);
    rb_tree_inorder_walk(rb_tree);
    printf("\n");

    rb_tree_delete(rb_tree, 9);
    rb_tree_inorder_walk(rb_tree);
    printf("\n");
#endif

    rb_tree_free(&rb_tree);
    return EXIT_SUCCESS;
}
