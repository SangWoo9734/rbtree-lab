#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  
  // NIL Node 메모리 동적 할당
  node_t *nil = (node_t*)malloc(sizeof(node_t));
  

  // NIL Node 초기화
  nil->color = RBTREE_BLACK;
  nil->key = 0;
  nil->left = nil;
  nil->right = nil;
  nil->parent = nil;

  // 트리 속성 초기화
  p -> nil = nil;
  p -> root = nil;

  return p;
}

// delete_rbtree_node : 트리 내 노드를 순회하며 노드의 메모리를 반환
void delete_rbtree_node(node_t *node, node_t *nil) {
  if ( node != nil) {
    delete_rbtree_node(node -> left, nil);
    delete_rbtree_node(node -> right, nil);
    free(node);
  }
}

// delete_tree(tree): RB tree 구조체가 차지했던 메모리 반환
// 해당 tree가 사용했던 메모리를 전부 반환해야 합니다. (valgrind로 나타나지 않아야 함)
void delete_rbtree(rbtree *t) {
  // root 부터 노드를 순회
  delete_rbtree_node(t -> root, t -> nil);
  
  // rbtree 메모리 반환
  free(t -> nil);
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
