/** 
 * Copyright (c) 2005-2006
 * Chen Ming <mockey.chen_AT_gmail.com>  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY Mockey Chen <mockey.chen_AT_gmail.com> 
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 * A PARTICULAR PURPOSE, OR NON-INFRINGEMENT, ARE DISCLAIMED.
 */

#ifndef CM_BINOMIALHEAP_HPP
#define CM_BINOMIALHEAP_HPP 1

#include "Util.hpp"
#include <ostream>

namespace cm
{
  /**
   * 二项堆 - 算法导论 第19章
   *
   * 二项堆的性质：
   *
   * 操作           时间复杂度
   * make-heap      O(1)
   * insert         O(lg(n)) 
   * minimum        O(lg(n)) 
   * extract-min    O(lg(n)) 
   * union          O(lg(n)) 
   * descrease-key  O(lg(n)) 
   * delete         O(lg(n)) 
   * 
   */
  template<typename T>
  class BinomialHeap;

  template<typename T>
  class BinomialNode
  {
    public:
      explicit BinomialNode(const T& v)
        : value_(v), parent_(NULL), child_(NULL), sibling_(NULL), degree_(0)
      {
      }

      const T& value() const
      {
        return value_;
      }

    private:
      friend class BinomialHeap<T>;

    private:
      T                value_;
      BinomialNode<T>* parent_;
      BinomialNode<T>* child_;  // 左孩子
      BinomialNode<T>* sibling_;   // 最近邻的右兄弟
      size_t           degree_;    // 子女的个数
  };

  /**
   * 二项最小堆：节点的关键字大于父节点的关键字
   */
  template<typename T>
  class BinomialHeap
  {
    public:
      typedef BinomialNode<T> Node;

    public:
      BinomialHeap()
        : head_(NULL), size_(0)
      {
      }

      BinomialHeap(const BinomialHeap<T>& rhs)
      {
        head_ = this->copy(rhs.head_);
        size_ = rhs.size_;
      }

      BinomialHeap<T>& operator=(const BinomialHeap<T>& rhs)
      {
        if (&rhs != this) {
          this->destroy();
          head_ = this->copy(rhs.head_);
          size_ = rhs.size_;
        }

        return *this;
      }

      ~BinomialHeap()
      {
        this->destroy();
      }

      // 释放堆分配的资源
      void destroy()
      {
        this->destroy(head_);
      }

      // 判断堆是否为空
      bool is_empty() const
      {
        return head_ == NULL;
      }

      // 返回堆中元素的个数
      size_t size() const
      {
        return size_;
      }

      // 返回最小关键字的节点
      // 如果二项堆为空，返回NULL
      Node* min()
      {
        Node* prev = NULL;
        Node* node = NULL;
        min(prev, node);
        return node;
      }

      // 删除最小的节点
      bool extract_min(T& x)
      {
        Node* prev = NULL;
        Node* node = NULL;
        min(prev, node);

        if (!node) {
          return false;
        }

        x = node->value_;

        // 从根链表中删除节点node
        if (prev) {
          prev->sibling_ = node->sibling_;
        }
        else {
          head_ = node->sibling_;
        }

        unify(head_, reverse(node->child_));

        delete node;
        --size_;
        return true;
      }

      // 插入元素到堆中
      BinomialHeap<T>& insert(const T& key)
      {
        Node* x = new Node(key);
        unify(head_, x);
        ++size_;

        return *this;
      }

      // 减小一个节点的关键字值，即降低该节点的高度
      // 前提条件: new_key < node->value_
      bool descrease(Node* node, const T& new_key)
      {
        if (new_key > node->value_) {
          return false;
        }

        node->value_ = new_key;

        Node* parent = node->parent_;
        while (parent && node->value_ < parent->value_) {
          swap(node->value_, parent->value_);
          node = parent;
          parent = node->parent_;
        }

        return true;
      }

      // 如果key存在，则删除并返回1，否则返回0
      size_t remove(const T& key)
      {
        size_t ret = 0;
        Node* x = this->find(key);
        if (x) {
          this->remove(x);
          ++ret;
        }

        return ret;
      }

      // 删除一个节点。
      // 删除方法：
      //   1. 先递降该节点，直到递降到根链表为止。
      //   2. 删除该节点
      void remove(Node* node)
      {
        Node* parent = node->parent_;
        while (parent) {
          // swap parent and node
          swap(parent->value_, node->value_);
          node = parent;
          parent = node->parent_;
        }

        Node* prev = NULL;
        Node* pos = head_;
        while (pos != node) {
          prev = pos;
          pos = pos->sibling_;
        }

        if (prev) {
          prev->sibling_ = node->sibling_;
        }
        else {
          head_ = node->sibling_;
        }

        this->unify(head_, this->reverse(node->child_));

        delete node;
        --size_;
      }

      // 返回key对应的节点，如果key不存在，返回NULL
      Node* find(const T& key)
      {
        return find(head_, key);
      }

      /**
       * 打印一个二项堆的内部结构，表示方法：
       *   如果一个节点有子节点，使用(x)来表示子节点；
       *   使用 -> 来指向兄弟节点。
       *
       * 示例：
       *
       *  --> 33 --------> 7
       *      /           / \
       *     41         25   12
       *                /
       *              28
       *
       * 表示结果为：
       * 33(41) -> 7(25(28) -> 12)
       *
       */
      std::ostream& dump(std::ostream& os)
      {
        dump(os, head_);
        return os;
      }

    private:
      Node* copy(const Node* node)
      {
        if (!node) {
          return NULL;
        }

        Node* n = new Node(node->value_);
        n->degree_ = node->degree_;
        n->child_ = this->copy(node->child_);
        n->sibling_ = this->copy(node->sibling_);

        return n;
      }

      void destroy(Node*& node)
      {
        if (node) {
          destroy(node->child_);
          destroy(node->sibling_);
          delete node;
          node = NULL;
        }
      }

      // 查找方法：
      // 先比较本节点的值是否要找的值，如果是则返回
      // 否则和比较子节点的值，如果key大于child的值，则在以child的数种查找
      // 最后再在兄弟节点中查找
      Node* find(Node* head, const T& key)
      {
        if (!head) {
          return NULL;
        }

        if (head->value_ == key) {
          return head;
        }
        
        if (head->value_ < key) {
          return find(head->child_, key);
        }

        // head->value_ > key
        // seach all siblings
        Node* next = head->sibling_;
        while (next) {
          Node* x = find(next, key);
          if (x) {
            return x;
          }

          next = next->sibling_;
        }

        return NULL;
      }

      std::ostream& dump(std::ostream& os, Node* root)
      {
        if (root) {
          os << root->value_;
          if (root->child_) {
            os << '(';
            dump(os, root->child_);
            os << ')';
          }
          if (root->sibling_) {
            os << " -> ";
            dump(os, root->sibling_);
          }
          else {
          }
        }

        return os;
      }

      // 反转根链表, 返回反转后的跟链表指针
      static Node* reverse(Node* h)
      {
        Node* tail = NULL;
        Node* next;

        if (!h) {
          return h;
        }

        h->parent_ = NULL;
        while(h->sibling_) {
          next = h->sibling_;
          h->sibling_ = tail;
          tail = h;
          h = next;
          h->parent_ = NULL;
        }

        h->parent_ = NULL;
        h->sibling_ = tail;
        return h;
      }

      // 返回二项堆的最小节点node 及 根链表中它的前节点prev
      void min(Node*& prev, Node*& node)
      {
        prev = NULL;

        if (this->is_empty()) {
          node = NULL;
          return;
        }

        node = head_;
        Node* old_prev = node;
        Node* curr = node->sibling_;  // 遍历指针
        while(curr != NULL) {
          if (curr->value_ < node->value_) {
            node = curr;
            prev = old_prev;
          }
          old_prev = curr;
          curr = curr->sibling_;
        }
      }
      
      // 合并两个二项堆:  把 h2 合并到 target 
      static void unify(Node*& target, Node* h2)
      {
        if (!h2) {
          return;
        }

        Node* h1 = target;
        if (!h1) {
          target = h2;
          return;
        }

        h1 = merge(h1, h2);
        Node* prev = NULL;
        Node* x = h1;
        Node* next = x->sibling_;
        while (next) {
          if (x->degree_ != next->degree_ ||
              (next->sibling_ && next->sibling_->degree_ == x->degree_)) {
            // Nothing to do, advance
            prev = x;
            x = next;
          }
          else if (x->value_ <= next->value_) {
            // x becoomes the root of next
            x->sibling_ = next->sibling_;
            link(x, next);
          }
          else {
            // next becomes the root x
            if (prev) {
              prev->sibling_ = next;
            }
            else {
              h1 = next;
            }

            link(next, x);
            x = next;
          }

          next = x->sibling_;
        }

        target = h1;
      }

      // 把 other 合并到当前的二项堆
      static Node* merge(Node* h1, Node* h2)
      {
        Node* new_head = NULL;
        Node** pos = &new_head;

        while (h1 && h2) {
          if (h1->degree_ < h2->degree_) {
            *pos = h1;
            h1 = h1->sibling_;
          }
          else {
            *pos = h2;
            h2 = h2->sibling_;
          }
          pos = &(*pos)->sibling_;
        }

        *pos = h1 ? h1 : h2;

        return new_head;
      }

      static void link(Node* root, Node* child) 
      {
        child->parent_  = root;
        child->sibling_ = root->child_;
        root->child_    = child;
        root->degree_++;
      }

    private:
      Node* head_;
      size_t size_;

      // TODO: 缓冲最小节点，提高获取最小节点的速度
      //Node* min_;
  };
}

#endif // CM_BINOMIALHEAP_HPP


