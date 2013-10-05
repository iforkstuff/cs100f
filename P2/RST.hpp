#ifndef RST_HPP
#define RST_HPP
#include "BST.hpp"
#include <cstdlib>

template <typename Data>
class RST : public BST<Data> {

/** 
 * @author Kevin Yang
 * Login name: kjy002
 * Description: If a BST is unbalanced after we inserts some node, then RST.hpp
 * has a method that performs rotations to ensure that a BST is balanced.
 */

public: 

  virtual std::pair<typename BST<Data>::iterator,bool> insert(const Data& item) {
    std::pair<typename BST<Data>::iterator,bool> ret = BST<Data>::insert(item);
    repair(ret.first.curr);
    return ret;
  }

protected:

  /** Restore any required invariants after inserting a new Data item
   *  in this BST.  The argument should be a pointer to the node containing
   *  the Data item that was inserted.
   */
  virtual void repair(BSTNode<Data>* n) {
    n->info = rand();
    if (n->parent != 0) {
      while ((n->parent != 0) && (n->info > (n->parent)->info)) {
        if ((n->parent)->left == n) rotateWithLeftChild(n->parent);
        else rotateWithRightChild(n->parent);
      }
    }
  }

private:

  /** Perform this operation if the newly inserted node has greater priority 
   *  than its parent, and is the left child.
   */ 
  virtual void rotateWithLeftChild(BSTNode<Data>* n) {
    BSTNode<Data> *pointer = n->left;
    n->left = pointer->right;
    if(n->left != 0) n->left->parent = n;
    pointer->right = n;
    if(n->parent != 0) {
      BSTNode<Data> *header = n->parent;
 /*     if(pointer->data < header->data)*/ header->left = pointer;
 /*
      else {
	std::cout << "right child" << std::endl;
	header->right = pointer;
      }
      */
      pointer->parent = header;
      n->parent = pointer;
    }
    else {
      BST<Data>::root = pointer;
      n->parent = pointer;
      pointer->parent = 0;
    }
    if(n->right != 0) n->right->parent = n;
  }

  /** Perform this operation if the newly inserted node has greater priority 
   *  than its parent, and is the right child.
   */
  virtual void rotateWithRightChild(BSTNode<Data>* n) {
    BSTNode<Data> *pointer = n->right;
    n->right = pointer->left;
    if(n->right != 0) n->right->parent = n;
    pointer->left = n;
    if(n->parent != 0) {
      BSTNode<Data> *header = n->parent;
 //     if (pointer->data < header->data) header->left = pointer;
  //    else {
//	std::cout << "right child again" << std::endl;
	header->right = pointer;
 //     }
      pointer->parent = header;
      n->parent = pointer;
    }
    else {
      BST<Data>::root = pointer;
      n->parent = pointer;
      pointer->parent = 0;
    }
    if(n->left != 0) n->left->parent = n;
  }

};
#endif // RST_HPP
