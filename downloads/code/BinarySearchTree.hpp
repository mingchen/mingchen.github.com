#ifndef CM_BINARYSEARCHTREE_HPP
#define CM_BINARYSEARCHTREE_HPP

#include "BinaryTree.hpp"

namespace cm
{
  /// 算法导论 第12章 二叉查找树
  /// 
  /// 特性：
  /// 左节点 < 父节点 < 右节点
  /// 
  /// BinarySearchTree_T 是 BinarySearchTree 和 RBTree 的父类
  /// 
  template<typename NODE>
  class BinarySearchTree_T : public BinaryTree<NODE>
  {
public:
  typedef NODE Node;

public:
      Node* find(const typename Node::VALUE_TYPE& value)
      {
        Node* node = this->root();
        while (node != Node::NIL && node->value() != value) {
          if (value < node->value()) 
            node = node->left();
          else 
            node = node->right();
        }

        return node;
      }

      /// 返回节点x的后继
      Node* successor(Node* x) 
      {
        // 有右子树
        if (x->right() != Node::NIL) {
          return this->min(x->right());
        }

        // 没有右子树
        Node* y = x->parent();
        while ((y != Node::NIL) && (x == y->right())) {
          x = y;
          y = y->parent();
        }

        return y;
      }


      Node* min()
      {
        return this->min(this->root());
      }

      Node* min(Node* node)
      {
        if (node != Node::NIL) {
          while (node->left() != Node::NIL) {
            node = node->left();
          }
        }

        return node;
      }

      Node* max()
      {
        return this->max(this->root());
      }

      Node* max(Node* node)
      {
        if (node != Node::NIL) {
          while (node->right() != Node::NIL) {
            node = node->right();
          }
        }

        return node;
      }
  };


  /**
   *  最基本的二叉查找树
   */
  template<typename T>
    class BinarySearchTree : public BinarySearchTree_T< BinaryTreeNode<T> >
  {
    public:
      typedef BinaryTreeNode<T> Node;

    public:
      BinarySearchTree()
      {
      }

      ~BinarySearchTree()
      {
      }

      BinarySearchTree<T>& insert(const T& value)
      {
        Node* node = new Node(value);
        Node* y = 0;
        Node* x = this->root();
        while (x) {
          y = x;
          if (value < x->value()) {
            x = x->left();
          }
          else {
            x = x->right(); 
          }
        }

        node->parent(y);

        if (y == Node::NIL) {
          // Empty tree.
          this->root_ = node;
        }
        else {
          if (value < y->value()) {
            y->left(node);
          }
          else {
            y->right(node);
          }
        }

        ++this->size_;

        return *this;
      }

      /// 返回节点x的前驱
      Node* predecessor(Node* x)
      {
        // TODO:
        return 0;
      }

      /// 删除一个值
      size_t remove(const T& value)
      {
        Node* node = this->find(value);
        if (node == Node::NIL) {
          return 0;
        }

        this->remove(node);

        return 1;
      }


      /// 删除一个已经存在的节点
      virtual void remove(Node* node)
      {
        Node* y = 0;  // 后继，y是真正要删除的节点
        Node* x = 0;  // y 的子节点
        if (node->left() == Node::NIL || node->right() == Node::NIL) {
          y = node;
        }
        else {
          y = this->successor(node);
        }

        if (y->left() != Node::NIL) {
          x = y->left();
        }
        else {
          x = y->right();
        }

        // 把节点y移除
        if (x != Node::NIL) {
          x->parent(y->parent());
        }

        if (y->parent() == Node::NIL) {
          this->root_ = x;
        }
        else {
          if (y == y->parent()->left()) {
            y->parent()->left(x);
          }
          else {
            y->parent()->right(x);
          }
        }

        if (y != node) {
          // 拷贝y的值到node
          node->value(y->value());
        }

        delete y;
        --this->size_;
      }
  };

} // end namespace cm

#endif // CM_BINARYSEARCHTREE_HPP


