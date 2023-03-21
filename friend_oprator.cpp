//
// Created by tanpe on 2023/3/22.
//

#include <iostream>

using namespace std;

class Student {
public:
    int age_;
    string name_;

    // 不考虑友元函数的流操作符重载，需要换位置
    ostream &operator>>(ostream &os) const {
        os << "name = " << name_ << ", ";
        os << "age = " << age_ << endl;
        return os;
    }
};

// 全局函数，只要不涉及私有成员访问，也不需要友元函数
ostream& operator <<(ostream& os, const Student& s) {
    os << "student name = " << s.name_ << ", ";
    os << "student age = " << s.age_ << endl;
    return os;
}

class Teacher {
public:
    Teacher(int age, string name) {
        age_ = age;
        name_ = name;
    }

private:
    int age_;
    string name_;

    // 即便这个函数申明为私有，但是仍可以直接使用，因为这个函数的this指针并不是该类
    // （实则因为是全局函数，这个函数调用时没有this指针）
    friend ostream& operator << (ostream& os, const Teacher& t);

    friend ostream& operator >> (ostream& os, const Teacher& t) {
        os << "teacher name = " << t.name_ << ", ";
        os << "teacher age = " << t.age_ << endl;
        return os;
    }

};

ostream& operator << (ostream& os, const Teacher& t) {
    os << "teacher name = " << t.name_ << ", ";
    os << "teacher age = " << t.age_ << endl;
    return os;
}

int main() {
    Student s{11, "aloha"};
    s >> cout << "hi" << endl;
    cout << s;

    Teacher t{45, "look"};
    cout << t;

    cout >> t;

    return 0;
}