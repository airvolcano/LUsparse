#ifndef _BIN_TREE_H
#define _BIN_TRRE_H
#include <iostream>
using namespace std;
//enum NodeType
//{
//	leaf, internal
//};
template <typename T0,typename T1>
class BinNode
{
public:
	T0 val0;
	T1 val1;
	BinNode *left;
	BinNode *right;
	BinNode()
	{
		left = right = NULL;
	}
	BinNode(T0 Val0,T1 Val1, BinNode *l, BinNode *r)
	{
		val0 = Val0;
		val1 = Val1;
		left = l;
		right = r;
	}
	~BinNode()
	{}
	bool isLeaf()
	{
		return (left == NULL) && (right == NULL);
	}
	void setVal(T0 Val0,T1 Val1)
	{
		val0 = Val0;
		val1 = Val1;
	}

};

template <typename T0,typename T1>
class BinTree
{
public:
	int num;
	T0* data0;
	T1* data1;
	BinNode<typename T0, typename T1> *root;
	BinTree()
	{
		num = 0;
		root = NULL;
	}
	~BinTree()
	{
	}
private:
	void insert(BinNode<typename T0, typename T1> *&rt, T0 Val0, T1 Val1)
	{
		if (rt == NULL)
		{
			rt = new BinNode<typename T0, typename T1>(Val0, Val1, NULL, NULL);
			num++;
		}
		else if (Val0 < rt->val0)
		{
			insert(rt->left, Val0, Val1);
		}
		else if (Val0 > rt->val0)
		{
			insert(rt->right, Val0, Val1);
		}
		else
		{
			rt->val1 += Val1;
		}
	}
	BinNode<typename T0, typename T1>* deletemin(BinNode<typename T0, typename T1> *&rt)
	{
		if (rt == NULL)
		{
			cout << "err" << endl;
			return NULL;
		}
		else if (rt->left != NULL)
		{
			return deletemin(rt->left);
		}
		else
		{
			BinNode<typename T0, typename T1>* oldrt = rt;
			rt = rt->right;
			num--;
			return oldrt;
		}
	}
	BinNode<typename T0, typename T1>* minVal(BinNode<typename T0, typename T1> *&rt)
	{
		if (rt == NULL)
		{
			cout << "err" << endl;
			return NULL;
		}
		else if (rt->left != NULL)
		{
			return minVal(rt->left);
		}
	}
	void remove(BinNode<typename T0, typename T1> *&rt, T0 val0)
	{
		if (rt == NULL)
		{
			//cout << val << " Not Found" << endl;
		}
		else if (val0 < rt->val0)
		{
			remove(rt->left, val0);
		}
		else if (val0 > rt->val0)
		{
			remove(rt->right, val0);
		}
		else
		{
			if (rt->left == NULL)
			{
				rt = rt->right;
				num--;
			}
			else if (rt->right == NULL)
			{
				rt = rt->left;
				num--;
			}
			else
			{
				//rt->setVal(deletemin(rt->right)->val0, deletemin(rt->right)->val1);
				rt = deletemin(rt->right);
			}
		}
	}
	void print(BinNode<typename T0, typename T1> *&rt, const int level)
	{
		if (rt != NULL)
		{
			print(rt->left, level + 1);
			//for (int i = 0; i < level; i++)
			//{
			//	cout << "\t";
			//}
			cout << level << "," << rt->val0 << "," << rt->val1 << "\t";
			print(rt->right, level + 1);
		}
	}
	void traverse(BinNode<typename T0, typename T1> *&rt)//////////////////////////
	{
		if (rt == NULL)
		{
			return;
		}
		else if (rt->left != NULL)
		{
			traverse(rt->left);
		}
		data0[num] = rt->val0;
		data1[num] = rt->val1;
		num++;
		if (rt->right != NULL)
		{
			traverse(rt->right);
		}
	}
	void traversedata0(BinNode<typename T0, typename T1> *&rt)//////////////////////////
	{
		if (rt == NULL)
		{
			return;
		}
		else if (rt->left != NULL)
		{
			traversedata0(rt->left);
		}
		data0[num] = rt->val0;
		num++;
		if (rt->right != NULL)
		{
			traversedata0(rt->right);
		}
	}
	void traversedata1(BinNode<typename T0, typename T1> *&rt)//////////////////////////
	{
		if (rt == NULL)
		{
			return;
		}
		else if (rt->left != NULL)
		{
			traversedata1(rt->left);
		}
		data1[num] = rt->val1;
		num++;
		if (rt->right != NULL)
		{
			traversedata1(rt->right);
		}
	}
	void divide(BinNode<typename T0, typename T1> *&rt, T1 d)
	{
		if (rt == NULL)
		{
			return;
		}
		else if (rt->left != NULL)
		{
			divide(rt->left, d);
		}
		rt->val1 = rt->val1 / d;
		if (rt->right != NULL)
		{
			divide(rt->right, d);
		}

	}
public:
	void insert(T0 val0, T1 val1)
	{
		insert(root, val0, val1);
	}

	BinNode<typename T0, typename T1>* deletemin()
	{
		return deletemin(root);
	}
	BinNode<typename T0, typename T1>* minVal()
	{
		return minVal(root);
	}
	void remove(T0 Val0)
	{
		remove(root, Val0);
	}
	void print()
	{
		print(root, 0);
	}
	T0* data0toArray()
	{
		data0 = (T0*)malloc(sizeof(T0)*num);
		num = 0;
		traversedata0(root);
		return data0;
	}
	T1* data1toArray()
	{
		data1 = (T1*)malloc(sizeof(T1)*num);
		num = 0;
		traversedata1(root);
		return data1;
	}
	void divide(T1 d)
	{
		divide(root, d);
	}
	void del()
	{
		num = 0;
		data0 = NULL;
		data1 = NULL;
		root = NULL;
	}
};



#endif
