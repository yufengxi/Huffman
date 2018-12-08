//
// Created by fingal on 12/5/18.
#ifndef HUFFMAN_HUFFMAN_H
#define HUFFMAN_HUFFMAN_H

#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <string.h>
using namespace std;

struct element
{
    char data=' ';
    //保存的字母符号
    int weight;
    //结点权重
    string code = "";
    //定义字母对应的编码
    bool visit = false;
    //是否被遍历
    element *left, *right, *parent;
    //定义左右结点以及母结点
};

class HuffmanTree
{
private:
    element root;
    //定义根结点
    int num;
    //结点总个数
    int w[120];
    //结点权重
    string S;
    //读入文件内容保存为字符串S
    element huffTree[120];
    //存储结点
    void countLetter();
    //存储结点频率以及其他信息的函数
    void readFile();
    //读取文件的函数

public:
    HuffmanTree();
    //构造函数
    void Encode();
    //编码函数
    void Decode();
    //译码函数
    void Search (int &i1, int &i2);
};

HuffmanTree::HuffmanTree()
{
    int i;
    //用于循环的变量
    readFile ();
    //将对应数据读入S
    countLetter();
    //计算S中信息
    for (i = 0; i < 2* num -1; i++)
    {
        huffTree[i].left = NULL;
        huffTree[i].right = NULL;
        huffTree[i].parent = NULL;
    }
    //默认结点和母结点均不存在
    for ( i = 0; i < 2* num -1; i++)
    {
        if(i < num)
        {
            huffTree[i].weight = w[i];
        }
        else
        {
            huffTree[i].weight = 100;
        }
        //构造2*n-1个只有根结点的二叉树
    }
    for ( int k = num; k < 2* num -1; k++)
    {
        int i1,i2;
        //定义i1，i2为最小的两个根结点
        Search(i1 , i2);
        //循环查找权重最小的两个根结点为i1，i2
        huffTree[i1].parent = &huffTree[k];
        huffTree[i2].parent = &huffTree[k];
        //令i1i2双亲均为k，并生成k
        huffTree[k].weight = huffTree[i1].weight + huffTree[i2].weight;
        //k权重为二者相加之和
        huffTree[k].left= &huffTree[i1];
        huffTree[k].right = &huffTree[i2];
        //左右孩子定义为i1，i2
    }
    for(i = num; i < 2*num -1; i++)
    {
        if(huffTree[i].parent == NULL && huffTree[i].left!=NULL && huffTree[i].right != NULL)
        {
            //寻找根结点
            root = huffTree[i];
        }
    }
}

void HuffmanTree::Search(int &i1, int &i2)
{
    int i;
    //循环变量i
    bool flag = true;

    for(i = 0;i < 2*num-1; i++)
    {
        if(flag) {
            if (huffTree[i].parent == NULL) {
                //寻找根结点
                i1 = i;
                flag = false;
            }
        }
        else
        {
            if(huffTree[i].parent==NULL)
            {
                //寻找根结点
                i2 = i;
                break;
            }

        }

    }

    for (i = 0; i < 2*num -1; i++)
    {
        if(i2 !=i && huffTree[i].parent == NULL && huffTree[i].weight > 0) {
            if (huffTree[i1].weight > huffTree[i].weight) {
                i1 = i;
            }
        }
    }
    for (i = 0; i < 2*num -1; i++)
    {
        if(i1 !=i && huffTree[i].parent == NULL && huffTree[i].weight > 0)
        {
            if (huffTree[i2].weight > huffTree[i].weight)
            {
                i2 = i;
            }

        }
    }
    //i1,i2分别循环寻找不为彼此权重最小的根结点
}

void HuffmanTree::readFile()
{
    string inName;
    cout << "请输入要编码的文件名:" <<endl;
    cin >>inName ;
    //存入输入的文件名
    ifstream in;
    in.open(inName, ios::in);
    //打开对应文件
    if(!in.is_open())
    {
        cout <<"文件打开失败！！！"<<endl;
        exit(0);
    }
    in >> S;
    //将文件内容保存至S
    in.close();
}

void HuffmanTree::countLetter()
{
    int size = 52;
    //共52个字母
    char ch[size];
    //字符数组
    int Weight[size];
    //权重保存；
    num = 0;
    //结点总数清零
    int i = 0;
    //用于循环的变量i清零
    ch[0] = 'A';
    //预置第零个字母为A
    ch[26] = 'a';
    //预置第26个字母为a
    for (i = 1; i < size / 2; i++) {
        ch[i] = ch[i - 1] + 1;
        ch[i + 26] = ch[i + 25] + 1;
    }
    //通过循环将26个大小写字母置入字符数组
    for (i = 0; i < size; i++)
        Weight[i] = 0;
    //将权重数组清零
    for (i = 0; i < S.size(); i++) {
        if (S.at(i) > 64 && S.at(i) < 91)
            Weight[S.at(i) - 'A']++;
        if (S.at(i) > 96 && S.at(i) < 123)
            Weight[S.at(i) - 'a']++;
        //每识别到一个字母，对应字母的权重+1
    }
    for (int i = 0; i < size; i++) {
        if (Weight[i] != 0) {
            huffTree[num].data = ch[i];
            //循环保存权重不为0的字母
            huffTree[num].weight = Weight[i];
            //循环保存权重
            w[num] = Weight[i];
            //将结点权重直接保存在哈夫曼数组中
            num++;
            //计算不同的字母个数
        }
    }

}

void HuffmanTree::Encode()
{
    int i = 0;
    string str = "";
    element *p = &root;
    char c;
    while(p !=NULL)
    {
        if(root.left->visit == true&&root.right->visit ==true) break;
        //如果根结点已被遍历则代表全部被遍历，退出循环
        if(!p ->left->visit )
        {
            p = p->left;
            str += '0';

        }
        else if(!p ->right->visit)
        {
            p = p->right;
            str += '1';
        }
        //遍历查找所有未被遍历的点,并根据二叉树结构编码
        if(p ->left == NULL || (p->left->visit && p->right->visit))
        {
            //如果根结点左右孩子均被遍历或者为叶结点，则置为已遍历
            p -> visit = true;
        }
        if(p ->data != ' ')
        {
            p ->code = str;
            //如果为叶结点，则将编码赋给它并清空编码循环字符串str
            str = "";
            cout<< p->data  <<" : "<< p->code << endl;
            //最终将该数据对应编码打印出来
            p = &root;
        }
        if(p->visit == true)
        {
            p = p->parent;
            str.pop_back();
        }
    }
    while (S.length()!=0)
    {
        c = S.at(0);
        //c存储S第一个字符，据此进行下一步判断
        i = 0;
        while(i <= num)
        {
          if(c == huffTree[i].data)
          {
              cout <<huffTree[i].code;
              break;
          }
          //将字符串中的字母替换成编码
          i++;
        }
        S.erase(0,1);
    }
}



//

#endif //HUFFMAN_HUFFMAN_H
