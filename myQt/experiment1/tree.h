#ifndef TREE_H
#define TREE_H
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
using namespace std;
template <class T>
struct TreeNode
{
    vector<T> element;          // 节点的数值
    int ComparingDimension;     // 记录比较的维度
    TreeNode* leftChild;
    TreeNode* rightChild;
    TreeNode() {for(auto ele: element) ele = 0; leftChild = NULL; rightChild = NULL;}  // 缺省构造函数
    TreeNode(vector<T> initial)
    {
        for(int i=0; i<initial.size(); i++)
            element.template emplace_back(initial[i]);
        leftChild = NULL; rightChild = NULL;
    }
};

template <class T>
class Tree
{
public:
    int getSize(){return size;}                     // 获取k-d树大小
    bool empty(){return size==0;}                   // 判断k-d树是否为空
    Tree(){size = 0; root = NULL; this->k=2;}       // 构造函数(默认2-d树)
    Tree(int k){size = 0; root = NULL; this->k=k;}  // 构造函数(默认2-d树)
    void reconstruct(int kk) {clear(); this->k = kk;} // 重构k-d树
    bool find(vector<T> info);                      // 查询元素是否存在
    void search(pair<vector<T>, vector<T>> scope, vector<vector<T>>& result);    // 范围查询, 所有满足查询范围的节点将被置于result中
    bool insert(vector<T> info);                    // 插入元素
    bool initialize(vector<vector<T>> dataSet);     // 初始化k-d树
    bool erase(vector<T> info);                     // 查找并删除元素
    void show();                                    // 通过层次遍历，对树进行简单展示
    void draw(int num, string label);               // 生成dot文件
    void specificDraw(vector<vector<int>> v, int num, string label);    // 生成带有标记的dot文件
    void clear(){size = 0; root = NULL;}            // 清空k-d树

private:
    void del(TreeNode<T>* pp, TreeNode<T>* p);  // 在给定父节点和子节点的情况下删除子节点, 供erase内部调用
    void recursionSearch(TreeNode<T>* ptr, pair<vector<T>, vector<T>> scope, vector<vector<T>>& result);    // 递归查询，供search调用
    void groupInsert(TreeNode<T>*& ptr, vector<vector<T>> data, int currentComparingDimension);             // 递归插入，供initialize调用
    TreeNode<T>* root;      // 根节点
    int k;                  // 维度
    int size;               // k-d树元素个数
};

template<class T>
// 找出最小值
void getLeft(TreeNode<T>* t, vector<pair<TreeNode<T>*, TreeNode<T>*>>& parentAndChild, int ComparingDimension)  // 借助中序遍历寻找
{
    if(t != NULL)
    {
        if(t->leftChild != NULL && (t->leftChild->ComparingDimension != ComparingDimension ||
                                    (t->leftChild->ComparingDimension == ComparingDimension && t->leftChild->leftChild == NULL)))
        // 左孩子比较维度不等于当前比较维度，则将其纳入备选方案; 左孩子比较维度等于当前维度同时左孩子没有左孩子, 则有可能是最小值, 纳入备选方案
        {
            parentAndChild.push_back({t, t->leftChild});
        }
        if(t->rightChild != NULL && (t->rightChild->ComparingDimension != ComparingDimension ||
                                     (t->rightChild->ComparingDimension == ComparingDimension && t->rightChild->leftChild == NULL)))
        // 右孩子比较维度不等于当前维度，则将其纳入备选方案; 右孩子比较维度等于当前维度同时右孩子没有左孩子, 则有可能是最小值, 纳入备选方案
        {
            parentAndChild.push_back({t, t->rightChild});
        }
        getLeft(t->leftChild, parentAndChild, ComparingDimension);
        getLeft(t->rightChild, parentAndChild, ComparingDimension);
    }
}

template<class T>
// 找出最大值
void getRight(TreeNode<T>* t, vector<pair<TreeNode<T>*, TreeNode<T>*>>& parentAndChild, int ComparingDimension)  // 借助中序遍历寻找
{
    if(t != NULL)
    {
        if(t->leftChild != NULL && (t->leftChild->ComparingDimension != ComparingDimension ||
                                    (t->leftChild->ComparingDimension == ComparingDimension && t->leftChild->rightChild == NULL)))
        {
            parentAndChild.push_back({t, t->leftChild});
        }
        if(t->rightChild != NULL && (t->rightChild->ComparingDimension != ComparingDimension ||
                                     (t->rightChild->ComparingDimension == ComparingDimension && t->rightChild->rightChild == NULL)))
        {
            parentAndChild.push_back({t, t->rightChild});
        }
        getRight(t->leftChild, parentAndChild, ComparingDimension);
        getRight(t->rightChild, parentAndChild, ComparingDimension);
    }
}

template<class T>
bool Tree<T>::insert(vector<T> info)
{
    if(root == NULL)    // 若树为空
    {
        root = new TreeNode<T>(info);  // 直接将数据作为根节点
        root->ComparingDimension = 1;
        size++;
        return true;
    }
    // 树不空
    TreeNode<T>* pre = NULL;    // 正在遍历节点的前一个节点
    TreeNode<T>* cur = root;    // 正在遍历的节点
    while(cur != NULL)   // 向下遍历，直到到达叶节点的"数据层"
    {
        bool flag = true;
        for(int i=0; i<k; i++)
        {
            if(info[i] != cur->element[i])
                flag = false;
        }
        if(flag)      // 查询成功
        {
            return false;
        }
        pre = cur;
        int currentComparingDimension = cur->ComparingDimension;
        if(info[currentComparingDimension-1] <= cur->element[currentComparingDimension-1])
        {
            cur = cur->leftChild;
        }
        else
        {
            cur = cur->rightChild;
        }
    }
    // 此时cur为NULL，而pre指向叶节点
    int lastDimension = pre->ComparingDimension;        // 获取上一层比较时所参考的维度
    int currentDimension = (lastDimension + 1);         // 当前比较维度: 上一层比较维度的下一个
    if(currentDimension > k) currentDimension = 1;      // 超过k维则还原为1维
    TreeNode<T>* newNode = new TreeNode<T>(info);       // 创建要插入的节点
    newNode->ComparingDimension = currentDimension;     // 更新节点的要比较的值
    if(info[lastDimension-1] <= pre->element[lastDimension-1])
    {
        pre->leftChild = newNode;
    }
    else
    {
        pre->rightChild = newNode;
    }
    size ++;
    return true;
}

template<class T>
bool Tree<T>::initialize(vector<vector<T>> dataSet)   // 传入数据维度数以及所有数据集, 建立2-D树
{
//    for(auto ele: dataSet)
//        insert(ele);
    groupInsert(root, dataSet, 1);
    return true;
}


template <class T>
void Tree<T>::groupInsert(TreeNode<T>*& ptr, vector<vector<T>> data, int currentComparingDimension)      // Node是正在插入的位置
{
    if(data.empty()) return;
    int length = data.size();
    for(int i=0; i<length; i++)    // 首先按照比较维度排序
    {
        for(int j=0; j<length-1; j++)
        {
            if(data[j][currentComparingDimension-1] > data[j+1][currentComparingDimension-1])
            {
                vector<T> tmp = data[j];
                data[j] = data[j+1];
                data[j+1] = tmp;
            }
        }
    }
    vector<vector<T>> left, right;
    for(int i=0; i<length; i++)
    {
        if(i < length/2)
        {
            left.template emplace_back(data[i]);
        }
        else if(i > length/2)
        {
            right.template emplace_back(data[i]);
        }
    }
    ptr = new TreeNode<T>;
    ptr->ComparingDimension = currentComparingDimension;
    ptr->element = data[length/2];
    currentComparingDimension ++;
    if(currentComparingDimension > k)
    {
        currentComparingDimension = 1;
    }
    if(! left.empty())
    {
        groupInsert(ptr->leftChild, left, currentComparingDimension);
    }
    else
    {
        ptr->leftChild = NULL;
    }
    if(! right.empty())
    {
        groupInsert(ptr->rightChild, right, currentComparingDimension);
    }
    else
    {
        ptr->rightChild = NULL;
    }
    size++;
}

template<class T>
bool Tree<T>::find(vector<T> info)
{
    if(root == NULL) return false;
    TreeNode<T>* ptr = root;        // 从根节点出发遍历查询
    while(ptr != NULL)
    {
        bool flag = true;
        for(int i=0; i<k; i++)
        {
            if(ptr->element[i] != info[i])
                flag = false;
        }
        if(flag)      // 查询成功
        {
            return true;
        }
        int currentComparingDimension = ptr->ComparingDimension;        // 获取正在进行比较的维度
        if(info[currentComparingDimension-1] <= ptr->element[currentComparingDimension-1])
        {
            ptr = ptr->leftChild;
        }
        else
        {
            ptr = ptr->rightChild;
        }
    }
    return false;       // 完成查询, 但仍然没有找到
}

template <class T>
void Tree<T>::search( pair<vector<T>, vector<T>> scope, vector<vector<T>>& result)
{
    recursionSearch(root, scope, result);
}

template <class T>
void Tree<T>::recursionSearch(TreeNode<T>* ptr, pair<vector<T>, vector<T>> scope, vector<vector<T>>& result)
// scope: 查询范围(scope[0], scope[1]分别表示在第一和第二维度上的查询范围);  result: 满足查询范围的节点
{
    if(ptr == NULL) return;
    else
    {
        int currentComparingDimension = ptr->ComparingDimension;     // 获取要比较的维度
        if(ptr->element[currentComparingDimension-1] < scope.first[currentComparingDimension-1])     // 访问节点在搜索区间以右
        {
            recursionSearch(ptr->rightChild, scope, result);
        }
        else if(ptr->element[currentComparingDimension-1] > scope.second[currentComparingDimension-1])      // 访问节点在搜索区间以右
        {
            recursionSearch(ptr->leftChild, scope, result);
        }
        else    // 落在搜索区间以内
        {
            bool flag = true;
            for(int i=0; i<k; i++)      // 遍历各个维度
            {
                if(ptr->element[i] < scope.first[i] || ptr->element[i] > scope.second[i])
                {
                    flag = false;
                }
            }
            if(flag)     // 判断其他各个维度是否也满足条件
            {
                result.template emplace_back(ptr->element);
            }
            recursionSearch(ptr->leftChild, scope, result);
            recursionSearch(ptr->rightChild, scope, result);
        }
    }
}


template<class T>
void Tree<T>::del(TreeNode<T> *pp, TreeNode<T> *p)
{
    if(p->leftChild == NULL && p->rightChild == NULL)   // 如果要删除的节点无左右子节点
    {
        if(pp->leftChild == p)  pp->leftChild = NULL;
        if(pp->rightChild == p) pp->rightChild = NULL;
    }
    else if(p->leftChild == NULL && p->rightChild != NULL) // 左空右不空
    {
        int ComparingDimension = p->ComparingDimension;
        vector<pair<TreeNode<T>*, TreeNode<T>*>> parentAndChild;
        parentAndChild.template emplace_back(p, p->rightChild);
        getLeft(p->rightChild, parentAndChild, ComparingDimension);     // 从右子树的左侧寻找可能解
        T minValue = 99999999;
        TreeNode<T>* pp2;    // 用来替换的节点的父节点
        TreeNode<T>* p2;     // 用来替换的节点
        for(auto i: parentAndChild)    // 找到在比较的维度上最小的节点及其父节点
        {
            if(i.second->element[ComparingDimension-1] < minValue)  // i.second指的是"候选"节点, 如果该候选节点在要比较的维度上的值小于minValue，则更新minValue和pp2、p2
            {
                minValue = i.second->element[ComparingDimension-1];
                pp2 = i.first;
                p2 = i.second;
            }
        }
        p->element = p2->element;   // 将得到的点替换要删除的点
        del(pp2, p2);       // 递归向下删除
    }
    else if(p->leftChild != NULL && p->rightChild == NULL) // 左子树不空、右子树为空
    {
        int ComparingDimension = p->ComparingDimension;
        vector<pair<TreeNode<T>*, TreeNode<T>*>> parentAndChild;
        parentAndChild.template emplace_back(p, p->leftChild);
        getRight(p->leftChild, parentAndChild, ComparingDimension);     // 从左子树的右侧找最大
        T maxValue = -99999999;
        TreeNode<T>* pp2;    // 用来替换的节点的父节点
        TreeNode<T>* p2;     // 用来替换的节点
        for(auto i: parentAndChild)    // 找到在比较的维度上最大的节点及其父节点
        {
            if(i.second->element[ComparingDimension-1] > maxValue)  // i.second指的是"候选"节点, 如果该候选节点在要比较的维度上的值大于maxValue，则更新maxValue和pp2、p2
            {
                maxValue = i.second->element[ComparingDimension-1];
                pp2 = i.first;
                p2 = i.second;
            }
        }
        p->element = p2->element;   // 将得到的点替换要删除的点
        del(pp2, p2);       // 递归向下删除
    }
    else if(p->leftChild != NULL && p->rightChild != NULL)  // 左子树不空且右子树不空, 这里选择从左子树找最大值
    {
        int ComparingDimension = p->ComparingDimension;
        vector<pair<TreeNode<T>*, TreeNode<T>*>> parentAndChild;
        parentAndChild.template emplace_back(p, p->leftChild);
        /* 根据先前对parentAndChild的定义, 在调用时会有一个尴尬点: 无论是getLeft还是getRight, 都是传入一个根节点，对该(子)树进行中序遍历
         而这个根节点, 由于要保存正在访问的节点的父节点，所以第一次调用del时, 如果传入的是要删除节点的左孩子或者右孩子，那么这个孩子就不能再被访问，
         只能作为父节点的配角角色。为解决这一问题，可以先将这个可怜的孩子结点存入数组，再做树的遍历*/
        getRight(p->leftChild, parentAndChild, ComparingDimension);     // 在其左子树向右找节点
        T maxValue = -99999999;
        TreeNode<T>* pp2;    // 用来替换的节点的父节点
        TreeNode<T>* p2;     // 用来替换的节点
        for(auto i: parentAndChild)    // 找到在比较的维度上最大的节点及其父节点
        {
            if(i.second->element[ComparingDimension-1] > maxValue)  // i.second指的是"候选"节点, 如果该候选节点在要比较的维度上的值大于maxValue，则更新maxValue和pp2、p2
            {
                maxValue = i.second->element[ComparingDimension-1];
                pp2 = i.first;
                p2 = i.second;
            }
        }
        p->element = p2->element;   // 将得到的点替换要删除的点
        del(pp2, p2);       // 递归向下删除
    }
}

template <class T>
bool Tree<T>::erase(vector<T> info)
{
    if(root == NULL) return false;
    TreeNode<T>* pre = NULL;
    TreeNode<T>* cur = root;
    while(cur != NULL)  // 查找树中是否有此节点
    {
        bool flag = true;
        for(int i=0; i<k; i++)
        {
            if(cur->element[i] != info[i])
            {
                flag = false;
            }
        }
        if(flag)
        {
            break;
        }
        pre = cur;
        int currentComparingDimension = cur->ComparingDimension;
        if(info[currentComparingDimension-1] <= cur->element[currentComparingDimension-1])
        {
            cur = cur->leftChild;
        }
        else
        {
            cur = cur->rightChild;
        }
    }
    if(cur == NULL) return false; // 最终没有找到该数据
    del(pre, cur);  // 调用内部删除函数
    return true;
}

template<class T>
void Tree<T>::show()
{
    queue<TreeNode<T>*> q;
    queue<int> layer;   // 记录层数, 便于展示图像
    q.push(root);
    layer.push(1);
    int preNo = 1;
    int currentNo = 1;
    while(!q.empty())
    {
        currentNo = layer.front();
        TreeNode<T>* ele = q.front();
        if(currentNo > preNo) cout << "\n";
        cout << "(";
        for(auto e: ele->element)
        {
            cout << e << ",";
        }
        cout << ")" << " ";
//        cout << "(" << ele->element.first << "," << ele->element.second << ")" << " ";
        if(ele->leftChild != NULL)
        {
            q.push(ele->leftChild);
            layer.push(currentNo + 1);
        }
        if(ele->rightChild != NULL)
        {
            q.push(ele->rightChild);
            layer.push(currentNo + 1);
        }
        q.pop();
        layer.pop();
        preNo = currentNo;
    }
}

template <class T>
void Tree<T>::draw(int num, string label)
{
    string path = R"(C:\Users\<username>\Desktop\myQt\experiment1\graph\)" + to_string(num) + ".dot";
    const char* paths = path.c_str();
    ofstream out(paths, ios::out);
    out << "digraph mygraph {" << "\n";                             // 输入头内容
    out << "label = " + label << ";\n";
    out << "graph [autosize=false, size=\"8.5, 8.5\"];\n";
    out << "fontname=\"Helvetica,Arial,sans-serif\"" << "\n";       // 设置字体
    out << "node [shape=circle,fixedsize=true,width=1.2];" << "\n";
    out << "node [fontname=\"Helvetica,Arial,sans-serif\"]" << "\n";
    out << "edge [fontname=\"Helvetica,Arial,sans-serif\"]" << "\n";
    out << "node [shape = circle];\n";        // 普通节点

    if(root == NULL)    // 空树
    {
        out << "}";
        out.close();
        string comd = R"(dot -Tpng C:\Users\<username>\Desktop\myQt\experiment1\graph\)" + to_string(num) + R"(.dot -o C:\Users\<username>\Desktop\myQt\experiment1\graph\)" +
                      to_string(num) + ".png";
        const char* comds = comd.c_str();
        system(comds);
        return;
    }
    else if(root->leftChild == NULL && root->rightChild == NULL)    // 仅一个点
    {
        out << "\"(";
        for(int i=0; i<root->element.size()-1; i++)
        {
            out << root ->element[i] << ",";
        }
        out << root->element[root->element.size()-1] << ")\"" << ";\n";
//        out << "\"(" << root->element.first << "," << root->element.second << ")\"" << ";\n";
        out << "}";
        out.close();
        string comd = R"(dot -Tpng C:\Users\<username>\Desktop\myQt\experiment1\graph\)" + to_string(num) + R"(.dot -o C:\Users\<username>\Desktop\myQt\experiment1\graph\)" +
                      to_string(num) + ".png";
        const char* comds = comd.c_str();
        system(comds);
        return;
    }

    // 定位dot文件
    queue<TreeNode<T>*> q;
    q.push(root);
    while(! q.empty())
    {
        TreeNode<T>* ptr = q.front();
        q.pop();
        if(ptr->leftChild != NULL)
        {
            out << "edge [arrowhead=vee style=dashed] ";
            out << "\"(";
            for(int i=0; i<ptr->element.size()-1; i++)
            {
                out << ptr ->element[i] << ",";
            }
            out << ptr->element[ptr->element.size()-1] << ")\"";
            out  << " -> ";
            out << "\"(";
            for(int i=0; i<ptr->leftChild->element.size()-1; i++)
            {
                out << ptr->leftChild->element[i] << ",";
            }
            out << ptr->leftChild->element[ptr->leftChild->element.size()-1] << ")\"";
            q.push(ptr->leftChild);
        }
        if(ptr->rightChild != NULL)
        {
            out << "edge [arrowhead=vee style=solid] ";
            out << "\"(";
            for(int i=0; i<ptr->element.size()-1; i++)
            {
                out << ptr ->element[i] << ",";
            }
            out << ptr->element[ptr->element.size()-1] << ")\"";
            out  << " -> ";
            out << "\"(";
            for(int i=0; i<ptr->rightChild->element.size()-1; i++)
            {
                out << ptr->rightChild->element[i] << ",";
            }
            out << ptr->rightChild->element[ptr->rightChild->element.size()-1] << ")\";\n";
            q.push(ptr->rightChild);
        }
    }

    out << "}";     // 文件结尾
    out.close();
    string comd = R"(dot -Tpng C:\Users\<username>\Desktop\myQt\experiment1\graph\)" + to_string(num) + R"(.dot -o C:\Users\<username>\Desktop\myQt\experiment1\graph\)" +
                  to_string(num) + ".png";
    const char* comds = comd.c_str();
    system(comds);
}

template<class T>
void Tree<T>::specificDraw(vector<vector<int>> v, int num, string label)
{
    string path = R"(C:\Users\<username>\Desktop\myQt\experiment1\graph\)" + to_string(num) + ".dot";
    const char* paths = path.c_str();
    ofstream out(paths, ios::out);
    out << "digraph mygraph {" << "\n";                             // 输入头内容
    out << "label = " + label << ";\n";
    out << "graph [autosize=false, size=\"8.5, 8.5\"];\n";
    out << "fontname=\"Helvetica,Arial,sans-serif\"" << "\n";       // 设置字体
    out << "node [shape=circle,fixedsize=true,width=1.2];" << "\n";
    out << "node [fontname=\"Helvetica,Arial,sans-serif\"]" << "\n";
    out << "edge [fontname=\"Helvetica,Arial,sans-serif\"]" << "\n";
    out << "node [shape = doublecircle]; ";
    for(const auto& ele: v)
    {
        out << "\"(";
        for(int i=0; i<ele.size()-1; i++)
        {
            out << ele[i] << ",";
        }
        out << ele[ele.size()-1];
        out << ")\" ";
//        out << "\"(" << ele.first << "," << ele.second << ")\" ";
    }
    out << ";\n";
    out << "node [shape = circle];\n";        // 普通节点

    // 定位dot文件
    queue<TreeNode<T>*> q;
    q.push(root);
    while(! q.empty())
    {
        TreeNode<T>* ptr = q.front();
        q.pop();
        if(ptr->leftChild != NULL)
        {
            out << "edge [arrowhead=vee style=dashed] ";
            out << "\"(";
            for(int i=0; i<ptr->element.size()-1; i++)
            {
                out << ptr ->element[i] << ",";
            }
            out << ptr->element[ptr->element.size()-1] << ")\"";
            out  << " -> ";
            out << "\"(";
            for(int i=0; i<ptr->leftChild->element.size()-1; i++)
            {
                out << ptr->leftChild->element[i] << ",";
            }
            out << ptr->leftChild->element[ptr->leftChild->element.size()-1] << ")\";\n";
            q.push(ptr->leftChild);
        }
        if(ptr->rightChild != NULL)
        {
            out << "edge [arrowhead=vee style=solid] ";
            out << "\"(";
            for(int i=0; i<ptr->element.size()-1; i++)
            {
                out << ptr ->element[i] << ",";
            }
            out << ptr->element[ptr->element.size()-1] << ")\"";
            out  << " -> ";
            out << "\"(";
            for(int i=0; i<ptr->rightChild->element.size()-1; i++)
            {
                out << ptr->rightChild->element[i] << ",";
            }
            out << ptr->rightChild->element[ptr->rightChild->element.size()-1] << ")\";\n";
            q.push(ptr->rightChild);
        }
    }

    out << "}";     // 文件结尾
    out.close();
    string comd = R"(dot -Tpng C:\Users\<username>\Desktop\myQt\experiment1\graph\)" + to_string(num) + R"(.dot -o C:\Users\<username>\Desktop\myQt\experiment1\graph\)" +
                  to_string(num) + ".png";
    const char* comds = comd.c_str();
    system(comds);
}
#endif // TREE_H
