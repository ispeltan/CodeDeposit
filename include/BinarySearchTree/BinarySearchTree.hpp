//
// Created by tanpe on 2023/3/29.
//

#pragma

#include <iostream>
#include <sstream>
#include <queue>
#include <optional>

template<class K, class V>
class BinarySearchTree {
private:
    struct Node {
        Node *left;
        Node *right;
        K key;
        V val;
        int cnt; // 以该节点为根的树有多少个节点

        Node(K k, V v) {
            this->left = nullptr;
            this->right = nullptr;
            this->key = k;
            this->val = v;
            this->cnt = 0;
        }
        Node(Node* node) {
            this->left = node->left;
            this->right = node->right;
            this->key = node->key;
            this->val = node->val;
            this->cnt = node->cnt;
        }
    };

    Node *root_ = nullptr;
public:
    // 插入 - 如果存在相同键值，则返回false和k-v信息
    std::pair<bool, std::pair<K, V>> Insert(K key, V val) {
        auto ret = DoInsert(root_, key, val);
        root_ = ret.first;
        return std::make_pair(ret.second.first, std::make_pair(ret.second.second->key, ret.second.second->val));
    }

    // 查找
    bool Contain(K key) {
        return DoContain(root_, key);
    }

    // 搜索
    std::optional<std::pair<K, V>>Search(K key) {
        auto ret = DoSearch(root_, key);
        if (!ret) {
            return {};
        }
        return std::make_pair(ret->key, ret->val);
    }

    // 遍历
    // 1 前序遍历
    std::string PreOrder() {
        if (!root_)
            return {};
        return DoPreOrder(root_);
    }

    // 2 中序遍历
    std::string InOrder() {
        if (!root_)
            return {};
        return DoInOrder(root_);

    }

    // 3 后序遍历
    std::string PostOrder() {
        if (!root_)
            return {};
        return DoPostOrder(root_);

    }

    // 4 层序遍历
    std::string LevelOrder() {
        if (root_ == nullptr) {
            return {};
        }
        std::queue<Node *> q;
        q.push(root_);
        std::stringstream ss{};
        int level = 1;
        while (!q.empty()) {
            std::size_t size = q.size();
            ss << "level" << level << ": ";
            while (size-- > 0) {
                auto node = q.front();
                q.pop();
                ss << "k=" << node->key << "c=" << node->cnt << " ";
                if (node->left)
                    q.push(node->left);
                if (node->right)
                    q.push(node->right);
            }
            ss << std::endl;
            level++;
        }
        return ss.str();
    }

    // 析构函数
    ~BinarySearchTree() {
        Destroy(root_);
    }
    // 获取和删除最大值
    std::optional<std::pair<K, V>> Maximum() {
        if (!root_) {
            return {};
        }
        auto ret = DoMaximum(root_);
        if (!ret) {
            return {};
        }
        return std::make_pair(ret->key, ret->val);
    }
    void DelMax() {
        if (!root_) {
            return;
        }
        root_ = DoDelMax(root_);
    }

    // 获取和删除最小值
    std::optional<std::pair<K, V>> Minimum() {
        if (!root_) {
            return {};
        }
        auto ret = DoMinimum(root_);
        if (!ret) {
            return {};
        }
        return std::make_pair(ret->key, ret->val);
    }
    void DelMin() {
        if (!root_) {
            return;
        }
        root_ = DoDelMin(root_);
    }

    // 删除任意节点
    void Remove(K key) {
        root_ = DoRemove(root_, key);
    }

    // floor、ceil
    std::optional<std::pair<K, V>> Floor(K key) {
        auto ret = DoFloor(root_, key);
        if (!ret) {
            return {};
        }
        return std::make_pair(ret->key, ret->val);
    }
    std::optional<std::pair<K, V>> Ceil(K key) {
        auto ret = DoCeil(root_, key);
        if (!ret) {
            return {};
        }
        return std::make_pair(ret->key, ret->val);
    }

    void CompleteCnt() {
        if (!root_)
            return;
        root_->cnt = CompleteCnt(root_);
    }
    // rank(排名第几)、select（排名第几的元素是谁）
    int Rank(K key) {
        CompleteCnt();
        return DoRank(root_, key);
    }

    std::optional<std::pair<K, V>> Select(int rank) {
        CompleteCnt();
        auto ret = DoSelect(root_, rank);
        if (!ret) {
            return {};
        }
        return std::make_pair(ret->key, ret->val);
    }

    // 支持重复元素的二分搜索树
private:
    std::pair<Node *, std::pair<bool, Node *>> DoInsert(Node *node, K key, V val) {
        if (node == nullptr) {
            auto new_node = new Node(key, val);
            return std::make_pair(new_node, std::make_pair(true, new_node));
        }
        if (node->key == key) {
            return std::make_pair(node, std::make_pair(false, node));
        } else if (node->key > key) {
            auto ret = DoInsert(node->left, key, val);
            node->left = ret.first;
            return std::make_pair(node, ret.second);
        } else {
            auto ret = DoInsert(node->right, key, val);
            node->right = ret.first;
            return std::make_pair(node, ret.second);
        }
    }

    // 1 前序遍历
    std::string DoPreOrder(Node *node) {
        std::stringstream ss{};
        ss << node->key << " ";
        if (node->left)
            ss << DoPreOrder(node->left);
        if (node->right)
            ss << DoPreOrder(node->right);

        return ss.str();
    }

    // 2 中序遍历
    std::string DoInOrder(Node *node) {
        std::stringstream ss{};
        if (node->left)
            ss << DoInOrder(node->left);
        ss << node->key << " ";
        if (node->right)
            ss << DoInOrder(node->right);

        return ss.str();
    }

    // 3 后序遍历
    std::string DoPostOrder(Node *node) {
        std::stringstream ss{};

        if (node->left)
            ss << DoPostOrder(node->left);
        if (node->right)
            ss << DoPostOrder(node->right);
        ss << node->key << " ";
        return ss.str();
    }

    // 4 层序遍历
    std::string DoLevelOrder(Node *node) {
        return {};
    }

    bool DoContain(Node* node, K key) {
        if (node == nullptr) {
            return false;
        }
        if (node->key == key) {
            return true;
        } else if (node->key > key) {
            return DoContain(node->left, key);
        } else {
            return DoContain(node->right, key);
        }
    }
    Node* DoSearch(Node* node, K key) {
        if (node == nullptr) {
            return nullptr;
        }
        if (node->key == key) {
            return node;
        } else if (node->key > key) {
            return DoSearch(node->left, key);
        } else {
            return DoSearch(node->right, key);
        }
    }

    void Destroy(Node* node) {
        if (!node) {
            return;
        }
        Destroy(node->left);
        Destroy(node->right);
        delete node;
    }
    Node* DoMaximum(Node* node) {
       if (node->right == nullptr) {
           return node;
       }
       return DoMaximum(node->right);
    }

    Node* DoMinimum(Node* node) {
        if (node->left == nullptr) {
            return node;
        }
        return DoMinimum(node->left);
    }

    // 删除以node为根的二叉树的最大节点
    // 返回新的二叉树的根
    Node* DoDelMax(Node* node) {
        if (node->right == nullptr) {
            auto left = node->left;
            delete node;
            return left;
        }
        node->right = DoDelMax(node->right);
        return node;
    }
    Node* DoDelMin(Node* node) {
        if (node->left == nullptr) {
            auto right = node->right;
            delete node;
            return right;
        }
        node->left = DoDelMin(node->left);
        return node;
    }

    // 删除以node为根的key值元素
    // 返回新的根节点
    Node* DoRemove(Node* node, K key) {
        if (node == nullptr) {
            return nullptr;
        }
        if (node->key > key) {
            node->left = DoRemove(node->left, key);
            return node;
        } else if (node->key < key) {
            node->right = DoRemove(node->right, key);
            return node;
        } else {
            if (node->left == nullptr) {
                Node* right = node->right;
                delete node;
                return right;
            }
            if (node->right == nullptr) {
                Node* left = node->left;
                delete node;
                return left;
            }

            auto s = new Node(DoMinimum(node->right));
            s->right = DoDelMin(node->right);
            s->left = node->left;
            delete node;
            return s;
        }

    }

    Node* DoFloor(Node* node, K key) {
        if (node == nullptr) {
            return nullptr;
        }
        if (node->key > key) {
            return DoFloor(node->left, key);
        } else if (node->key < key) {
            auto ret = DoFloor(node->right, key);
            if (!ret) {
                return node;
            }
            return ret;
        } else {
            return node;
        }
    }
    Node* DoCeil(Node* node, K key) {
        if (node == nullptr) {
            return nullptr;
        }
        if (node->key < key) {
            DoCeil(node->right, key);
        } else if (node->key > key) {
            auto ret = DoCeil(node->left, key);
            if (!ret) {
                return node;
            }
            return ret;
        } else {
            return node;
        }
    }

    int CompleteCnt(Node* node) {
        if (!node) {
            return 0;
        }
        if (node->left == nullptr && node->right == nullptr) {
            node->cnt = 1;
            return node->cnt;
        }
        node->cnt = CompleteCnt(node->left) + CompleteCnt(node->right) + 1;
        return node->cnt;
    }

    int DoRank(Node* node, K key) {
        if (!node) {
            return 0;
        }
        if (node->key <= key) {
            return (node->left? node->left->cnt + 1: 1) + DoRank(node->right, key);
        } else {
            return DoRank(node->left, key);
        }
    }

    Node* DoSelect(Node* node, int rank) {
        if (rank <= 0 || node == nullptr) {
            return nullptr;
        }
        if (!node->left) {
            return DoSelect(node->right, rank - 1);
        }
        if (node->left->cnt + 1 == rank) {
            return node;
        } else if (node->left->cnt + 1 > rank) {
            return DoSelect(node->left, rank);
        } else {
            return DoSelect(node->right, rank - node->left->cnt - 1);
        }

    }

};