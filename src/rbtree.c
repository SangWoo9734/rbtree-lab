#include "rbtree.h"

#include <stdlib.h>

// Rotate Logics
// L-ROTATE ()
void left_rotate(rbtree* t, node_t* node) {
  node_t* r_node = node -> right;

  node -> right = r_node -> left;

  if (r_node -> left != t -> nil) {
    r_node -> left -> parent = node;
  }

  r_node -> parent = node -> parent;

  if (node -> parent == t -> nil) {
    t -> root = r_node;
  }
  else if ( node -> key == node -> parent -> left -> key) {
    node -> parent -> left = r_node;
  }
  else {
    node -> parent -> right = r_node;
  }
  
  r_node -> left = node;
  node -> parent = r_node;
}

// R-ROTATE ( )
void right_rotate(rbtree* t, node_t* node) {
  node_t* l_node = node -> left;

  node -> left = l_node -> right;

  if (l_node -> right != t -> nil) {
    l_node -> right -> parent = node;
  }

  l_node -> parent = node -> parent;

  if ( node -> parent == t -> nil ) {
    t -> root = l_node;
  }
  else if ( node -> key == node -> parent -> left -> key ) {
    node -> parent -> left = l_node;
  }
  else {
    node -> parent -> right = l_node;
  }

  l_node -> right = node;
  node -> parent = l_node;
}

// tree = new_tree(): RB tree 구조체 생성
// 여러 개의 tree를 생성할 수 있어야 하며 각각 다른 내용들을 저장할 수 있어야 합니다.
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

// rbtree_insert_fixup(t, node)
void rbtree_insert_fixup(rbtree* t, node_t * z) {
  node_t *uncle; 
  while ( z -> parent -> color == RBTREE_RED ) {
    if ( z -> parent == z -> parent -> parent -> right) { // 부모가 조상 노드의 오른쪽인 경우
      uncle = z -> parent -> parent -> left;
      if ( uncle -> color == RBTREE_RED ) { // 부모의 형제가 RED인 경우
        uncle -> color = RBTREE_BLACK;
        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        z = z -> parent -> parent;
      } else {
        if ( z == z -> parent -> left ) { // 삽입 노드가 부모의 왼쪽인 경우
          z = z -> parent;
          right_rotate(t, z);
        }
        z -> parent -> color = RBTREE_BLACK; // 삽입 노드가 부모의 오른쪽인 경우
        z -> parent -> parent -> color = RBTREE_RED;
        left_rotate(t, z -> parent -> parent);
      }
    } else { // z -> parent == z -> parent -> parent -> left // 부모가 조상 노드의 왼쪽인 경우
      uncle = z -> parent -> parent -> right;
      if ( uncle -> color == RBTREE_RED ) { // 부모의 형제가 RED인 경우
        uncle -> color = RBTREE_BLACK;
        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        z = z -> parent -> parent;
      } else {
        if ( z == z -> parent -> right ) { // 삽입 노드가 부모의 오른쪽인 경우
          z = z -> parent;
          left_rotate(t, z);
        }
        z -> parent -> color = RBTREE_BLACK; // 삽입 노드가 부모의 왼쪽인 경우
        z -> parent -> parent -> color = RBTREE_RED;
        right_rotate(t, z -> parent -> parent);
      }
    }
  }
  t -> root -> color = RBTREE_BLACK; // root node must BLACK.
}

// tree_insert(tree, key): key 추가
// 구현하는 ADT가 multiset이므로 이미 같은 key의 값이 존재해도 하나 더 추가 합니다.
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // create new node
  node_t *new_node = (node_t*)calloc(1, sizeof(node_t));
  new_node -> key = key;

  node_t * y = t -> nil;
  node_t * x = t -> root;
  
  while ( x != t -> nil ) { // 삽입할 위치의 리프 노드 탐색
    y = x;
    if ( x -> key < new_node -> key ) {
      x = x -> right;
    } else {
      x = x -> left;
    }
  }
  new_node -> parent = y;

  if ( y == t -> nil ) { // 삽입한 노드와 부모노드를 연결
    t -> root = new_node;
  } else if ( y -> key < new_node -> key ) {
    y -> right = new_node;
  } else {
    y -> left = new_node;
  }

  new_node -> color = RBTREE_RED;
  new_node -> left = t -> nil;
  new_node -> right = t -> nil;
  rbtree_insert_fixup(t, new_node); // RB Tree의 규칙에 위배되는지 확인

  return t->root;
}

// ptr = tree_find(tree, key)
// RB tree내에 해당 key가 있는지 탐색하여 있으면 해당 node pointer 반환
// 해당하는 node가 없으면 NULL 반환
node_t *rbtree_find(const rbtree *t, const key_t key) {
  
  node_t *node = t->root;

  while ( 1 ){
    if (node == t -> nil) { // NIL노드를 만나는 경우 해당 노드가 없다고 판단, NULL 반환
        return NULL;
    }
    else if (node -> key > key) {
      node = node -> left;
    }
    else if (node -> key < key) {
      node = node -> right;
    }
    else { // 해당 노드를 찾은 경우
      return node;
    }
  }
}

// ptr = tree_min(tree): RB tree 중 최소 값을 가진 node pointer 반환
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
