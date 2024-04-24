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
  // RB 트리의 왼쪽으로 이동하며 node->left의 키 값이 null인지 확인. null이라면 해당 노드 return
  node_t *node = t ->root;

  while (node -> left != t -> nil ) {
    node = node -> left;
  }

  return node;
}

// ptr = tree_max(tree): 최대값을 가진 node pointer 반환
node_t *rbtree_max(const rbtree *t) {
  // RB 트리의 오른쪽으로 이동하며 node->right의 키 값이 null인지 확인. null이라면 해당 노드 return
  node_t *node = t ->root;

  while (node -> right != t -> nil ) {
    node = node -> right;
  }

  return node;
}

// rbtree_transplant(tree, u, v) 
// 삭제할 노드와 삭제 노드에 대체될 노드(prodecessor)의 위치를 교체
void rbtree_transplant(rbtree* t, node_t *u, node_t *v) {
  if ( u -> parent == t -> nil ) {
    t -> root = v;
  }
  else if ( u == u -> parent -> right ) {
    u -> parent -> right = v;
  } else {
    u -> parent -> left = v;
  }
  v -> parent = u -> parent;
}


// rbtree_erase_fixup(t, x)
void rbtree_erase_fixup(rbtree* t, node_t* x) {
  node_t* uncle;

  while ( x != t -> root && x -> color == RBTREE_BLACK ) {
    if ( x == x -> parent -> right ) { // 교체할 노드가 오른쪽 노드인 경우
      uncle = x -> parent -> left;

      if ( uncle -> color == RBTREE_RED ) { // 부모의 형제 노드가 RED인 경우 -> CASE 1
        uncle -> color = RBTREE_BLACK;
        x -> parent -> color = RBTREE_RED;
        right_rotate(t, x -> parent);

        uncle = x -> parent -> left;
      }
      
      if ( uncle -> left -> color == RBTREE_BLACK && uncle -> right -> color == RBTREE_BLACK ) { // 부모의 형제 노드, 왼/오른쪽 노드 모두 BLACK인 경우 -> CASE 2
        uncle -> color = RBTREE_RED;
        x = x -> parent;
      } else {
        if ( uncle -> right -> color == RBTREE_RED ) { // 형제 노드의 오른쪽 노드가 RED인 경우 -> CASE 3
          uncle -> right -> color = RBTREE_BLACK;
          uncle -> color = RBTREE_RED;
          left_rotate(t, uncle);
          uncle = x -> parent -> left;
        }
uncle -> color = x -> parent -> color; // 형제 노드의 왼쪽 노드가 RED인 경우 -> CASE 4
        x -> parent -> color = RBTREE_BLACK;
        uncle -> left -> color = RBTREE_BLACK;
        right_rotate(t, x -> parent);
        x = t -> root;
      }

    } else { // x == x -> parent -> left // 교체할 노드가 오른쪽 노드인 경우
      uncle = x -> parent -> right;

      if ( uncle -> color == RBTREE_RED ) { // 부모의 형제 노드가 RED인 경우 -> CASE 1
        uncle -> color = RBTREE_BLACK;
        x -> parent -> color = RBTREE_RED;
        left_rotate(t, x -> parent);

        uncle = x -> parent -> right;
      } 

      if ( uncle -> left -> color == RBTREE_BLACK && uncle -> right -> color == RBTREE_BLACK) { // 부모의 형제 노드, 왼/오른쪽 노드 모두 BLACK인 경우 -> CASE 2
        uncle -> color = RBTREE_RED;
        x = x -> parent;
      } else {
        if ( uncle -> left -> color == RBTREE_RED ) { // 형제 노드의 오른쪽 노드가 RED인 경우 -> CASE 3
          uncle -> left -> color = RBTREE_BLACK;
          uncle -> color = RBTREE_RED;
          right_rotate(t, uncle);
          uncle = x -> parent -> right;
        }

        uncle -> color = x -> parent -> color; // 형제 노드의 왼쪽 노드가 RED인 경우 -> CASE 4
        x -> parent -> color = RBTREE_BLACK;
        uncle -> right -> color = RBTREE_BLACK;
        left_rotate(t, x -> parent);
        x = t -> root;
      }
    }
  }

  x -> color = RBTREE_BLACK; // root node's color is always BLACK.
}

// tree_erase(tree, ptr): RB tree 내부의 ptr로 지정된 node를 삭제하고 메모리 반환
int rbtree_erase(rbtree *t, node_t *p) {
  node_t* changing_node = p; // y
  node_t* delete_node_next; // x
  key_t delete_origin_color = changing_node -> color;

  if ( p -> left == t -> nil ) { // 삭제 노드의 자식이 1개인 경우 -> 자식이 오른쪽에 있는 경우
    delete_node_next = p -> right;
    rbtree_transplant(t, p, p -> right);
  } else if (p -> right == t -> nil ) { // 삭제 노드의 자식이 1개인 경우 -> 자식이 왼쪽에 있는 경우
    delete_node_next = p -> left;
    rbtree_transplant(t, p, p -> left);
  } else { // 삭제 노드의 자식이 2개인 경우
    rbtree right_child_tree = { p -> right, t -> nil };
    changing_node = rbtree_min(&right_child_tree); // 하위 노드의 successor 확인
    delete_origin_color = changing_node -> color;
    delete_node_next = changing_node -> right;

    if ( changing_node -> parent == p) {
      delete_node_next -> parent = changing_node;
    } else {
      rbtree_transplant(t, changing_node, changing_node -> right);
      changing_node -> right = p -> right;
      changing_node -> right -> parent = changing_node;
    }

    rbtree_transplant(t, p, changing_node);
    changing_node -> left = p -> left;
    changing_node -> left -> parent = changing_node;

    changing_node -> color = p -> color;
  } 

  // 삭제한 노드의 메모리 해제
  free(p);

  // 삭제한 노드가 BLACK인 경우 트리 재조정 확인
  if ( delete_origin_color == RBTREE_BLACK ) {
    rbtree_erase_fixup(t, delete_node_next);
  }

  return 0;
}

// tree_to_array(tree, array, n)

// RB tree의 내용을 key 순서대로 주어진 array로 변환
// array의 크기는 n으로 주어지며 tree의 크기가 n 보다 큰 경우에는 순서대로 n개 까지만 변환
// array의 메모리 공간은 이 함수를 부르는 쪽에서 준비하고 그 크기를 n으로 알려줍니다.
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  int index = 0;

  // 트리의 왼쪽 끝까지 탐색
  if ( t -> root -> left != t -> nil ) {
    rbtree lt = { t-> root -> left, t -> nil };
    rbtree_to_array( &lt, arr, n );
  }
  
  // 현재 배열내 어디까지 원소가 채워져 있는지 index 확인
  index = 0;
  while (arr[index] != 0) {
      index++;
  }


  // 받은 자리수와 같아지면 index를 리턴하고, 함수 종료
  if ( index == n ) {
    return index;
  }

  arr[index] = t -> root -> key;

 // 트리의 오른쪽 순회
  if ( t -> root -> right != t -> nil ) {
    rbtree rt = { t-> root -> right, t -> nil };
    rbtree_to_array( &rt, arr, n );
  }
  return index;
}
