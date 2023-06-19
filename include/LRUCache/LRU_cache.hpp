//
// Created by peltan on 2023/6/16.
//

#pragma once

#include <list>
#include <map>

template <typename K, typename V, int size>
class LRUCache {
private:
  class Node {
  public:
    Node() = default;
    Node(K key, V val) {
      key_ = key;
      val_ = val;
    }
    K key_;
    V val_;
    Node *pre_ = nullptr;
    Node *next_ = nullptr;
  };

public:
  LRUCache() { header_ = new Node(); }
  ~LRUCache() {
    auto node = header_;
    while (node) {
      auto next = node->next_;
      delete node;
      node = next;
    }
  }
  std::optional<V> Get(K key) {
    auto find_it = map_.find(key);
    if (find_it == map_.end()) {
      return {};
    }
    // 如果是非头结点，则替换头结点
    auto node = find_it->second;
    auto result = node->val_;
    if (header_->next_ != node) {
      RemoveNode(node, false);
      PutHead(node);
    }
    // 返回结果
    return result;
  }
  bool Put(K key, V val) {
    // 如果存在，则值替换+位置更新
    if (map_.find(key) != map_.end()) {
      Node *node = map_[key];
      node->val_ = val;
      if (header_->next_ != node) {
        RemoveNode(node, false);
        PutHead(node);
      }

      return true;
    }
    // 如果不存在则添加
    auto add_node = new Node(key, val);
    map_[key] = add_node;
    if (map_.size() > size) {
      // 删除队尾元素，添加队首元素
      RemoveNode(last_, true);
      PutHead(add_node);
    } else {
      PutHead(add_node);
    }
    return true;
  }

private:
  Node *header_ = nullptr;
  Node *last_ = nullptr;
  std::map<K, Node *> map_;

  void PutHead(Node *node) {
    if (!header_) {
      return;
    }
    auto first = header_->next_;
    node->next_ = first;
    node->pre_ = header_;
    header_->next_ = node;
    if (first != nullptr) {
      first->pre_ = node;
    } else {
      last_ = node;
    }
  }

  void RemoveNode(Node *node, bool del) {
    auto it = map_.find(node->key_);
    if (it == map_.end()) {
      return;
    }
    auto last = node->pre_;
    auto next = node->next_;
    last->next_ = next;
    if (next) {
      next->pre_ = last;
    } else {
      last_ = last;
    }
    if (del) {
      delete node;
      map_.erase(it);
    }
  }
};
