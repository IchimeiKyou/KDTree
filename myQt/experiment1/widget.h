#ifndef WIDGET_H
#define WIDGET_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <QWidget>      // 组件
#include <QLineEdit>    // 行编辑框
#include<QGridLayout>   // 网格布局
#include <QPushButton>  // 普通按钮
#include <QTextEdit>    // 文本编辑框
#include <QErrorMessage>// 错误信息
#include <QLabel>       // 标签
#include <qDebug>       // 输出
#include <QFile>        // 读入初始化文件
#include <QFileDialog>
#include "tree.h"
using namespace std;

class Widget : public QWidget
{
    Q_OBJECT
public slots:
    void renewPic(int num)     // 更新图片
    {
        string prePath = R"(C:\Users\86150\Desktop\myQt\experiment1\graph\)" + to_string(num) + ".png";
        const char* path = prePath.c_str();
        picLabel->setPixmap(QPixmap(path));
        picLabel->setFixedSize(900, 900);       // 标准大小(375, 905)
        picLabel->setAlignment(Qt::AlignCenter);
    }

    void clearLine()
    {
        chooseKLine->clear();                                              // 选定维度
        findLine->clear();                                                 // 查询find文本框
        insertLine->clear();                                               // 插入insert文本
        eraseLine->clear();                                                // 删除erase文本框
        searchLine1->clear();searchLine2->clear();                                // 搜索search文本框
    }

    void choose()
    {
        string first = chooseKLine->text().toStdString();
        if(first.empty())
            return;
        int chooseValue = stoi(first);
        myTree.reconstruct(chooseValue);
        k = chooseValue;
        clearLine();
        system("del C:\\Users\\86150\\Desktop\\myQt\\experiment1\\graph /q *.*");
    }

    void find()
    {
        QString first = findLine->text();
        if(first.toStdString().empty())
            return;

        /*需要对文本框输入的内容进行拆分处理, 取出在各个维度上的取值*/
        string findvalue = first.toStdString();
        vector<int> info;
        for(int i=0; i<(int)findvalue.size(); i++)
        {
            int loc = i;
            bool tmp = false;
            while(isdigit(findvalue[i]))
            {
                tmp = true;
                i++;
            }
            if(tmp)
                info.emplace_back(stoi(findvalue.substr(loc, i-loc)));
        }

        bool flag = myTree.find(info);
        if(flag == false || (int)info.size()!=k) return;
        vector<vector<int>> v;
        vector<int> tmp = {info};     // find的v只有一个vector, 因为最多有一个搜索结果
        v.emplace_back(tmp);
        string label = "\"検索 (";
        for(int i=0; i<(int)info.size()-1; i++)
        {
            label += to_string(info[i]) + ", ";
        }
        label += to_string(info[info.size()-1]) + ")\"";
        myTree.specificDraw(v, picNum, label); renewPic(picNum);
        picNum++;
        currentNum = picNum-1;
        clearLine();
    }

    void insert()   // 在文本框中读取要插入的数据, 并执行插入
    {
        if(insertLine->text().toStdString().empty())
            return;
        string insertvalue = insertLine->text().toStdString();
        /*需要对文本框输入的内容进行拆分处理, 取出在各个维度上的取值*/
        vector<int> info;
        for(int i=0; i<(int)insertvalue.size(); i++)
        {
            int loc = i;
            bool tmp = false;
            while(isdigit(insertvalue[i]))
            {
                tmp = true;
                i++;
            }
            if(tmp)
                info.emplace_back(stoi(insertvalue.substr(loc, i-loc)));
        }
        bool flag = myTree.insert(info);
        if(flag == false || (int)info.size()!=k) return;
        string label = "\"挿入 (";
        for(int i=0; i<(int)info.size()-1; i++)
        {
            label += to_string(info[i]) + ", ";
        }
        label += to_string(info[info.size()-1]) + ")\"";
        myTree.draw(picNum, label); renewPic(picNum);      // 绘图
        picNum++;
        currentNum = picNum-1;
        clearLine();
    }

    void erase()   //  在文本框中读取要删除的数据, 并执行删除
    {
        if(eraseLine->text().toStdString().empty())
            return;
        string erasevalue = eraseLine->text().toStdString();
        vector<int> info;
        for(int i=0; i<(int)erasevalue.size(); i++)
        {
            int loc = i;
            bool tmp = false;
            while(isdigit(erasevalue[i]))
            {
                tmp = true;
                i++;
            }
            if(tmp)
                info.emplace_back(stoi(erasevalue.substr(loc, i-loc)));
        }
        bool flag = myTree.erase(info);
        if(flag == false || (int)info.size()!=k) return;
        string label = "\"删除 (";
        for(int i=0; i<(int)info.size()-1; i++)
        {
            label += to_string(info[i]) + ", ";
        }
        label += to_string(info[info.size()-1]) + ")\"";
        myTree.draw(picNum, label); renewPic(picNum);      // 绘图
        picNum++;
        currentNum = picNum-1;
        clearLine();
    }

    void search()   // 更新搜索数据, 并执行搜索
    {
        QString first = searchLine1->text();
        QString second = searchLine2->text();
        if(first.toStdString().empty() || second.toStdString().empty())
            return;
        string searchvalue1 = first.toStdString();
        string searchvalue2 = second.toStdString();
        pair<vector<int>, vector<int>>scope;

        vector<int> low;
        for(int i=0; i<(int)searchvalue1.size(); i++)
        {
            int loc = i;
            bool tmp = false;
            while(isdigit(searchvalue1[i]))
            {
                tmp = true;
                i++;
            }
            if(tmp)
                low.emplace_back(stoi(searchvalue1.substr(loc, i-loc)));
        }

        vector<int> high = {};
        for(int i=0; i<(int)searchvalue2.size(); i++)
        {
            int loc = i;
            bool tmp = false;
            while(isdigit(searchvalue2[i]))
            {
                tmp = true;
                i++;
            }
            if(tmp)
                high.emplace_back(stoi(searchvalue2.substr(loc, i-loc)));
        }

        scope = {low, high};
        vector<vector<int>> v;
        myTree.search(scope, v);
        if(v.empty())
            return;
        string label = "\"検索 下限(";
        for(int i=0; i<(int)low.size()-1; i++)
        {
            label += to_string(low[i]) + ", ";
        }
        label += to_string(low[low.size()-1]) + ")";
        label += "\ 上限(";
        for(int i=0; i<(int)high.size()-1; i++)
        {
            label += to_string(high[i]) + ", ";
        }
        label += to_string(high[high.size()-1]) + ")\"";
        myTree.specificDraw(v, picNum, label); renewPic(picNum);      // 绘图
        picNum++;
        currentNum = picNum-1;
        clearLine();
    }

    void initialize()
    {
        system("del C:\\Users\\86150\\Desktop\\myQt\\experiment1\\graph /q *.*");
        QString path = QFileDialog::getOpenFileName(this, QStringLiteral("初期化ファイルを選んでください"), R"(C:\Users\86150\Desktop\myQt\experiment1)", QStringLiteral("文本文件(*txt);;"));
        if(path.toStdString().empty())
            return;
        vector<vector<int>> number;
        ifstream ifs(path.toStdString(), ifstream::in);
        int tmpValue;
        while(ifs >> tmpValue)  // 先输入第一个
        {
            vector<int> temp = {tmpValue};
            for(int i=2; i<=k; i++) // 输入其他k-1个数据
            {
                ifs >> tmpValue;
                temp.emplace_back(tmpValue);
            }
            number.emplace_back(temp);
        }
        ifs.close();
        myTree.initialize(number);
        string label;
        label = "\"初期化\"";
        myTree.draw(picNum, label); renewPic(picNum);
        picNum ++;
        clearLine();
    }

    void left()
    {
        clearLine();
        if(currentNum != 0)
        {
            currentNum --;
            renewPic(currentNum);
        }
    }

    void right()
    {
        clearLine();
        if(currentNum != picNum-1)
        {
            currentNum ++;
            renewPic(currentNum);
        }
    }

    void clear()    // 清空
    {
        system("del C:\\Users\\86150\\Desktop\\myQt\\experiment1\\graph /q *.*");
        myTree.clear();
        picNum = 0;
        currentNum = 0;
        renewPic(0);
        clearLine();
    }

    void quit()     // 退出
    {
        exit(0);
    }


public:
    int k = 2;
    Tree<int> myTree;  // 2-d树对象
    Widget(QWidget *parent = 0);
    ~Widget();
    QLabel *picLabel;                 // 在图形界面显示2-d树图像的标签

    QPushButton *chooseKButton;       // 维度选择按钮
    QPushButton *findButton;          // 查询按钮
    QPushButton *insertButton;        // 插入按钮
    QPushButton *initializeButton;    // 初始化按钮
    QPushButton *eraseButton;         // 删除按钮
    QPushButton *searchButton;        // 查询按钮
    QPushButton *rightButton;         // 向右按钮
    QPushButton *leftButton;          // 向左按钮
    QPushButton *quitButton;          // 退出按钮
    QPushButton *clearButton;         // 清空按钮

    QLineEdit *chooseKLine;                                              // 选定维度
    QLineEdit *findLine;                                                 // 查询find文本框
    QLineEdit *insertLine;                                               // 插入insert文本
    QLineEdit *eraseLine;                                                // 删除erase文本框
    QLineEdit *searchLine1, *searchLine2;                                // 搜索search文本框

    QLabel *chooseKLabel;                                                // 选定维度按钮
    QLabel *findLabel;
    QLabel *insertLabel;
    QLabel *initializeLabel;
    QLabel *eraseLabel;
    QLabel *searchLabel1, *searchLabel2;
    QLabel *select;
    int picNum, currentNum;     // 照片总数量和目前正在显示的图片
};

#endif // WIDGET_H
