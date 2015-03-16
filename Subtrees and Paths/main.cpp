//
//  main.cpp
//  Subtrees and Paths
//
//  Created by Yunfan Jin on 3/12/15.
//  Copyright (c) 2015 yunfan.jin. All rights reserved.
//

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <cassert>
#include <map>
#include <utility>
using namespace std;

struct TreeNode{
    long value;
    long parent;
    set<long> child;
    TreeNode() : value(0), parent(-1){}
};

class Tree{
    vector<TreeNode> tree;
    
public:
    Tree(long N){
        tree.resize(N, TreeNode());
        tree[0].parent = 0;
    }
    
    void quickAdd(long parent, long child){
        tree[parent].child.insert(child);
        tree[child].parent = parent;
    }
    
    bool buildTree(long x, long y){
        if (tree[x].parent != -1){
            assert(tree[y].parent == -1);
            tree[x].child.insert(y);
            tree[y].parent = x;
            return true;
        }
        if (tree[y].parent != -1){
            tree[y].child.insert(x);
            tree[x].parent = y;
            return true;
        }
        return false;
    }
    void addValue(long x, long value){
        tree[x].value += value;
    }
    long findLowestCommonParent(long x, long y){
        if (x == y){
            return x;
        }
        long x_curr = x, y_curr = y;
        set<long> xparent, yparent;
        xparent.insert(x_curr);
        yparent.insert(y_curr);
        while (x_curr != 0 || y_curr != 0){
            if (x_curr != 0){
                x_curr = tree[x_curr].parent;
                if (yparent.find(x_curr) != yparent.end()){
                    return x_curr;
                }
                xparent.insert(x_curr);
            }
            if (y_curr != 0){
                y_curr = tree[y_curr].parent;
                if (xparent.find(y_curr) != xparent.end()){
                    return y_curr;
                }
                yparent.insert(y_curr);
            }
        }
        return 0;
    }
    long getSubMax(long child, long parent){
        if (child == parent)
            return 0;
        long curr = child;
        vector<long> stack;
        while (curr != parent){
            stack.push_back(curr);
            curr = tree[curr].parent;
        }
        long sum = 0, max = 0;
        while (!stack.empty()){
            curr = stack.back();
            stack.pop_back();
            sum += tree[curr].value;
            if (max < sum) max = sum;
        }
        return max;
    }
    long getBaseValue(long x){
        vector<long> stack;
        long sum = tree[x].value;
        long curr = x;
        while (curr != 0){
            curr = tree[curr].parent;
            sum += tree[curr].value;
        }
        return sum;
    }
    vector<long> goToTop(long x){
        long curr = x;
        vector<long> list;
        while (curr != 0) {
            list.push_back(curr);
            curr = tree[curr].parent;
        }
        list.push_back(curr);
        return list;
    }
    long getMaxValue(long x, long y){
        vector<long> path1 = goToTop(x);
        vector<long> path2 = goToTop(y);
        long v1 = path1.back();
        long v2 = path2.back();
        path1.pop_back();
        path2.pop_back();
        long sum = 0;
        bool tag1 = true, tag2 = true;
        while (v1 == v2) {
            sum += tree[v1].value;
            if (!path1.empty()) {
                v1 = path1.back();
                path1.pop_back();
            }
            else{
                tag1 = false;
            }
            if (!path2.empty()) {
                v2 = path2.back();
                path2.pop_back();
            }
            else{
                tag2 = false;
            }
            if (!tag1 || !tag2) {
                break;
            }
        }
        long max1 = 0;
        if (tag1) {
            long sum1 = tree[v1].value;
            max1 = sum1 > 0 ? sum1 : 0;
            while (!path1.empty()) {
                v1 = path1.back();
                sum1 += tree[v1].value;
                path1.pop_back();
                if (max1 < sum1) {
                    max1 = sum1;
                }
            }
        }
        long max2 = 0;
        if (tag2) {
            long sum2 = tree[v2].value;
            max2 = sum2 > 0 ? sum2 : 0;
            while (!path2.empty()) {
                v2 = path2.back();
                sum2 += tree[v2].value;
                path2.pop_back();
                if (max2 < sum2) {
                    max2 = sum2;
                }
            }
        }
        long tmp = max1 > max2 ? max1 : max2;
        return tmp + sum;
    }
};

class Graph{
    vector<set<long>> graph;
    
public:
    Graph(long N){
        graph.resize(N, set<long>());
    }
    void addLine(long x, long y){
        graph[x].insert(y);
        graph[y].insert(x);
    }
    
    void buildTree(Tree &tree, long x, long from){
        for (long next : graph[x]){
            if (next != from){
                tree.quickAdd(x, next);
                buildTree(tree, next, x);
            }
        }
    }
    
};

inline void remove(multimap<long, long> &repository, long x, long y){
    auto bound = repository.equal_range(x);
    for (auto iter = bound.first; iter != bound.second; iter++){
        if (iter->second == y){
            repository.erase(iter);
            return;
        }
    }
}

void execute(Tree &tree, multimap<long, long> &repository, long x, long y){
    bool tag = tree.buildTree(x - 1, y - 1);
    if (!tag){
        repository.insert(make_pair(x, y));
        repository.insert(make_pair(y, x));
        return;
    }
    auto find = repository.find(x);
    if (find != repository.end()){
        long tmp = find->second;
        remove(repository, x, tmp);
        remove(repository, tmp, x);
        execute(tree, repository, x, tmp);
    }
    find = repository.find(y);
    if (find != repository.end()){
        long tmp = find->second;
        remove(repository, y, tmp);
        remove(repository, tmp, y);
        execute(tree, repository, y, tmp);
    }
}

int main(){
    long N, Q, x, y;
    string command;
    cin >> N;
    Tree tree(N);
    Graph graph(N);
    //multimap <long, long> repository;
    for (long i = 0; i < N - 1; i++){
        cin >> x >> y;
        graph.addLine(x - 1, y - 1);
        //execute(tree, repository, x, y);
    }
    graph.buildTree(tree, 0, 0);
    //assert(repository.empty());
    cin >> Q;
    for (long i = 0; i < Q; i++){
        cin >> command >> x >> y;
        if (command == "find"){
            cout << tree.findLowestCommonParent(x - 1, y - 1) + 1 << endl;
        }
        else if (command == "add"){
            tree.addValue(x - 1, y);
        }
        else if (command == "sub"){
            cout << tree.getSubMax(x - 1, y - 1) << endl;
        }
        else if (command == "base"){
            cout << tree.getBaseValue(x - 1) << endl;
        }
        else if (command == "max"){
            //            long base = tree.findLowestCommonParent(x - 1, y - 1);
            //            long left = tree.getSubMax(x - 1, base);
            //            long right = tree.getSubMax(y - 1, base);
            //            long tmp = left > right ? left : right;
            //            long bvalue = tree.getBaseValue(base);
            //            cout << tmp + bvalue << endl;
            cout << tree.getMaxValue(x - 1, y - 1) << endl;
        }
    }
}