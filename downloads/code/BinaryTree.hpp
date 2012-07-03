#ifndef CM_BINARYTREE_HPP
#define CM_BINARYTREE_HPP 1

#include <ostream>
#include "Util.hpp"

namespace cm
{
  /// 二叉树节点
  template<typename T>
    class BinaryTreeNode
    {
      public:
        typedef T VALUE_TYPE;
        static BinaryTreeNode<T>* NIL;

      public:
        explicit BinaryTreeNode(const T& value)
          : value_(value)
            , left_(0)
            , right_(0)
            , parent_(0)
      {
      }

        BinaryTreeNode<T>* left()  {  return left_;   }
        BinaryTreeNode<T>* right() {  return right_;  }
        BinaryTreeNode<T>* parent(){  return parent_; }
        BinaryTreeNode<T>* left()   const {  return left_;   }
        BinaryTreeNode<T>* right()  const {  return right_;  }
        BinaryTreeNode<T>* parent() const {  return parent_; }
        const T&     value() const {  return value_;  }

        void left(BinaryTreeNode<T>* node)  {  left_ = node;   }
        void right(BinaryTreeNode<T>* node) {  right_ = node;  }
        void parent(BinaryTreeNode<T>* node){  parent_ = node; }
        void value(const T& value)          {  value_ = value; }

      private:
        T            value_;
        BinaryTreeNode<T>* left_;
        BinaryTreeNode<T>* right_;
        BinaryTreeNode<T>* parent_;
    };

  template<typename T>
    BinaryTreeNode<T>* BinaryTreeNode<T>::NIL = 0;


  /// 二叉树
  template<typename NODE>
    class BinaryTree
    {
      public:
        typedef NODE Node;

        BinaryTree()
          : root_(Node::NIL)
            , size_(0)
      {
      }

        BinaryTree(const BinaryTree<NODE>& rhs)
          : root_(Node::NIL)
            , size_(0)
        {
          this->root_ = this->copy(rhs.root());
          size_ = rhs.size();
        }

        BinaryTree& operator=(const BinaryTree<NODE>& rhs)
        {
          if (&rhs == this) {
            return *this;
          }

          this->destroy();
          this->root_ = this->copy(rhs.root());
          size_ = rhs.size();

          return *this;
        }

        virtual ~BinaryTree()
        {
          this->destroy();
        }

        size_t size() const
        {
          return this->size_;
        }

        bool is_empty() const
        {
          return this->root() == Node::NIL;
        }


        /// 返回树的高度
        /// 空树的高度为0
        /// 根节点的高度为1
        size_t height() const
        {
          return this->height(this->root());
        }

        void destroy()
        {
          this->destroy(root_);
          this->root_ = Node::NIL;
          this->size_ = 0;
        }

        /// 中序遍历二叉树。
        /// 如果是是二叉排序的数，则是按排序后的结果遍历
        ///
        /// FUNC 需要实现 int operator()(const NODE& node);
        /// 一个实现示例:
        ///
        /// class TreePrintWalker
        /// {
        ///  public:
        ///    template<typename NODE>
        ///      void operator()(NODE node) 
        ///      {
        ///        std::cout << node->value() << ' ';
        ///      }
        /// };
        ///
        /// 如果walker返回非0值，则停止遍历
        ///
        template<class FUNC>
          void inorder_traverse(FUNC walker)
          {
            this->inorder_traverse(walker, this->root());
          }

        template<class FUNC>
          void preorder_traverse(FUNC walker)
          {
            this->preorder_traverse(walker, this->root());
          }

        template<class FUNC>
          void postorder_traverse(FUNC walker)
          {
            this->postorder_traverse(walker, this->root());
          }

        bool operator==(const BinaryTree<NODE>& rhs) const
        {
          return this->equal(this->root(), rhs.root());
        }

        /**
         * 输出二叉树的结构, 以括号来分层。
         * 示例：
         *             7
         *            / \
         *           3   9
         *            \   
         *             5
         * 输出：
         * 7(3(nil,5(nil, nil)),9(nil, nil))
         *
         */
        std::ostream& dump(std::ostream& os) const
        {
          os << "Dump " << this << "(" << this->size() << "): ";
          this->dump(os, this->root());
          return os;
        }

      protected:
        const Node* root() const
        {
          return this->root_;
        }

        Node* root()
        {
          return this->root_;
        }

      protected:
        NODE*  root_;
        size_t size_;

      private:
        //  使用前序遍历来实现拷贝
        Node* copy(const Node* orig) const
        {
          if (orig == Node::NIL) {
            return Node::NIL;
          }

          Node* n = new Node(orig->value());
          n->left(this->copy(orig->left()));
          n->right(this->copy(orig->right()));

          return n;
        }

        // 如果 lhs 和 rhs 的子树不等同，则返回false
        // 否则返回true 
        bool equal(Node* lhs, Node* rhs) const
        {
          if (lhs == Node::NIL && rhs == Node::NIL) 
            return true;

          if (lhs != Node::NIL && rhs != Node::NIL
              && equal(lhs->left(), rhs->left())
              && equal(lhs->right(), rhs->right())) {
            return true;
          }

          return false;
        }

        size_t height(const Node* node) const
        {
          if (node == Node::NIL) {
            return 0;
          }

          size_t lh = this->height(node->left());
          size_t rh = this->height(node->right());

          return 1 + ((lh < rh) ? rh : lh);
        }

        void dump(std::ostream& os, const Node* node) const
        {
          if (node != Node::NIL) {
            os << node->value();
            if (node->left() != Node::NIL || node->right() != Node::NIL) {
              os << '(';
              this->dump(os, node->left());
              os << ',';
              this->dump(os, node->right());
              os << ')';
            }
          }
          else {
            os << "nil";
          }
        }

        /// 释放节点 current 及其子节点的内存
        void destroy(Node* current)
        {
          if (current != Node::NIL) {
            destroy(current->left());
            destroy(current->right());
            delete current;
          }
        }

        template<class FUNC>
          void preorder_traverse(FUNC walker, Node* node)
          {
            if (node != Node::NIL) {
              if (walker(node) != 0) {
                return;
              }
              this->preorder_traverse(walker, node->left());
              this->preorder_traverse(walker, node->right());
            }
          }

        template<class FUNC>
          void inorder_traverse(FUNC walker, Node* node)
          {
            if (node != Node::NIL) {
              this->inorder_traverse(walker, node->left());
              if (walker(node) != 0) {
                return;
              }
              this->inorder_traverse(walker, node->right());
            }
          }

        template<class FUNC>
          void postorder_traverse(FUNC walker, Node* node)
          {
            if (node != Node::NIL) {
              this->postorder_traverse(walker, node->left());
              this->postorder_traverse(walker, node->right());
              if (walker(node) != 0) {
                return;
              }
            }
          }
    };

}


#endif // CM_BINARYTREE_HPP


