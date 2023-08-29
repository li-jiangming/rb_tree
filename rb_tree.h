/*
 * rb_tree.h
 *
 *  Created on: 2023/8/28
 *      Author: ljm
 */

#ifndef RB_TREE_H_
#define RB_TREE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define T rb_tree_t
typedef struct T *T;

extern T        rb_tree_new();
extern void     rb_tree_free(T *t);

extern void     rb_tree_insert(T t, int32_t value);
extern void     rb_tree_delete(T t, int32_t value);

extern void     rb_tree_inorder_walk(T t);

extern int32_t  rb_tree_minimum(T t);
extern int32_t  rb_tree_maximum(T t);

#undef T

#ifdef __cplusplus
}
#endif

#endif
