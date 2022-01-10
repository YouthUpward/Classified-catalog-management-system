
//目录管理系统：双亲表示法实现N叉树的顺序存储
#define _CRT_SECURE_NO_WARNINGS
#define MAX_TREE_SIZE 100			//最多结点个数
#include <stdio.h>
#include <iostream>
using namespace std;

typedef struct Catalog				//目录结构类型
{
	char key[20] = { 0 };			//编号
	char caption[80] = { 0 };		//类名
	char parent[20] = { 0 };		//父亲编号
}Catalog;

typedef struct CatalogTree
{
	Catalog Node[MAX_TREE_SIZE] = { 0 };	//数据域,Node的下标就表示了某个单元信息在顺序存储中的位置
	int root = 0;					//根节点位置，根节点默认在0号单元
	int num = 0;					//最后一个结点的数组编号
}CatalogTree;

CatalogTree tree, * ct;				//如果直接用tree接收，应该写作fwrite(&tree->Node[i],sizeof(Catalog),1,fp)
FILE* fp = fopen("catalog3.dat", "rb+");//文件指针
Catalog C = { "###","###","###" };	//初始化一个结构体
int t = 0;							//记录层数
int f = 0;							//存储下标

void CreateCatalogTree();			//0 将文件读入数组
void AddCatalog();					//1 增加分类信息，保存至文件
void AlterCatalog();				//2 修改分类信息，保存至文件
void DeleteCatalog();				//3 从文件中删除分类信息
void PrintCatalog();				//4 顺序输出文件中所有分类信息
void CreateCatalogTree();			//5.1 将分类信息用双亲表示法存储。调用PrintCatalogTree()
void PrintCatalogTree();			//5.2 调用PreOrder()函数

void PreOrder(CatalogTree* ct, int i);
void PostOrder(CatalogTree* ct, int i);
bool leaf(CatalogTree* ct, int i);

void menu();

int main()
{
	if (fp == NULL)
	{
		printf("文件打开失败\n");
		return 0;
	}
	printf("**116132019188 曾思雅 计四**\n");
	CreateCatalogTree();			//将.dat文件写入数组
	for (;;)
	{
		menu();
		int choice;
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:AddCatalog(); break;
		case 2:PrintCatalog(); break;		//顺序输出分类信息
		case 3:PrintCatalogTree(); break;	//输出树形分类树,所以能理解为什么有些用&了是吧
		case 4:AlterCatalog(); break;
		case 5:DeleteCatalog(); break;
		default:return 0;
		}
	}
	fclose(fp);								//关闭文件流
	return 0;
}


/*将文件读入数组*/
void CreateCatalogTree()
{
	tree.num = 0;								//初始化结点数量
	for (int i = 0; !feof(fp); i++)				//如果文件没有读完，一次读入一个Catalog
	{
		fread(&tree.Node[i], sizeof(Catalog), 1, fp);		//从文件中读入
	//	printf("%d\t%s\t%s\t%s\n",i,tree.Node[i].key, tree.Node[i].caption, tree.Node[i].parent);
		tree.num++;
	}//但是请注意，会多读入一个空数据
	tree.num--;
	//	printf("tree.num=%d",tree.num);
}



/*开始写增加分类，支持读入多条记录，保存至文件*/
void AddCatalog()
{
	char temp[20] = { 0 };
	printf("请依次输入key、caption、parent（以#结束输入）\n");	//将新增分类直接写入数组
	for (;;)
	{
		scanf("%s", temp);
		if (strcmp(temp, "#") != 0)
		{
			strcpy(tree.Node[tree.num].key, temp);
			scanf("%s %s", tree.Node[tree.num].caption, tree.Node[tree.num].parent);	//更新数组

			//将新增分类写入文件
			fwrite(&tree.Node[tree.num], sizeof(Catalog), 1, fp);
			tree.num++;
		}
		else break;
	}
	printf("添加成功！\n");
}//至此增加分类已经写好


/*顺序打印数组*/
void PrintCatalog()
{
	for (int i = 0; i < tree.num; i++)
	{
		if (strcmp(tree.Node[i].caption, "###") != 0)
			printf("%s\t%s\t%s\n", tree.Node[i].key, tree.Node[i].caption, tree.Node[i].parent);
	}
}


/*树形打印分类树，调用先序遍历实现*/
void PrintCatalogTree()
{	//调用先序遍历
	PreOrder(&tree, 0);
}


/*先序遍历*/
void PreOrder(CatalogTree* ct, int i)			//输出树形分类树,这里为什么得*ct呢？*ct是树的数组
{
	//t为对应层数，打印空格
	for (int i = 0; i < t; i++)
		printf("   ");

	printf("|--%s  %s\n", tree.Node[i].caption, tree.Node[i].key);

	for (int j = i + 1; j < tree.num; j++)		//对于结点i，需要和后面的结点比较
	{	//在数组中逐个比对找到结点i的所有子结点
		if (strcmp(ct->Node[j].parent, ct->Node[i].key) == 0)
		{
			t++;
			PreOrder(ct, j);
			t--;
		}
	}
}


/*删除分类：包括级联删除和叶子结点删除，级联删除调用后序遍历*/
void DeleteCatalog()
{
	char a[20] = { 0 };
	int c;
	printf("请输入您需要删除的key: ");
	scanf("%s", &a);						//用户输入的应当是一个key

	for (f = 0; f < tree.num; f++)
	{//对于每个结点来说
		if (strcmp(tree.Node[f].key, a) == 0)		//找到该结点，进一步删除隶属于它的结点
			break;
	}
	printf("待删除信息：%s   %s   %s\n", tree.Node[f].key, tree.Node[f].caption, tree.Node[f].parent);
	if (leaf(&tree, f))
	{
		printf("tip：该目录下有子目录，将一起删除。\n如果继续删除操作，请按1;否则按2：");
		scanf("%d", &c);
		if (c != 1)
		{
			printf("退出删除\n");
			return;
		}
	}

	PostOrder(&tree, f);
	printf("删除成功！\n");
}


/*后序遍历*/
void PostOrder(CatalogTree* ct, int i)			//输出树形分类树,这里为什么得*ct呢？*ct是树的数组
{
	for (int j = i + 1; j < tree.num; j++)		//对于结点i，需要和后面的结点比较
	{	//在数组中逐个比对找到结点i的所有子结点
		if (strcmp(ct->Node[j].parent, ct->Node[i].key) == 0)
		{
			t++;
			PostOrder(ct, j);
			t--;
		}
	}

	//覆盖文件
	fseek(fp, i * sizeof(Catalog), 0);
	fwrite(&C, sizeof(Catalog), 1, fp);

	//将对应tree.Node[f]置为空，这样文件和数组就相互对应了
	strcpy(tree.Node[i].key, "###");
	strcpy(tree.Node[i].caption, "###");
	strcpy(tree.Node[i].parent, "###");
}


/*叶子结点判断，返回布尔型*/
bool leaf(CatalogTree* ct, int i)
{
	for (int j = i + 1; j < tree.num; j++)		//对于结点i，需要和后面的结点比较
	{	//在数组中逐个比对找到结点i的所有子结点
		if (strcmp(ct->Node[j].parent, ct->Node[i].key) == 0)
			return true;
	}
	return false;
}


/*修改分类：如果是叶子结点，可修改整条记录；否则只能修改caption*/
void AlterCatalog()
{
	printf("请输入您需要修改的key: ");
	int f = 0;										//存储下标
	char a[80] = { 0 };
	scanf("%s", &a);								//用户输入的应当是一个key

	for (f = 0; f < tree.num; f++)					//对于每个结点来说
	{
		if (strcmp(tree.Node[f].key, a) == 0)		//找到该结点
			break;
	}

	//读入待修改信息，写入数组
	printf("待修改信息：%s   %s   %s\n", tree.Node[f].key, tree.Node[f].caption, tree.Node[f].parent);
	if (leaf(&tree, f) == false)						//是叶子结点
	{
		printf("请输入key，caption，parent：");
		scanf("%s%s%s", tree.Node[f].key, tree.Node[f].caption, tree.Node[f].parent);	//已经写入数组
	}
	else
	{
		printf("tip：由于该目录下还有子目录，所以只能修改caption。\n请输入caption：");
		scanf("%s", tree.Node[f].caption);
	}

	//更新文件
	fseek(fp, f * sizeof(Catalog), 0);
	fwrite(&tree.Node[f], sizeof(Catalog), 1, fp);
	printf("修改成功！\n");
}


/*打印菜单*/
void menu()
{
	printf("\n************菜单************\n");
	printf("1、增加分类\n2、输出分类\n3、输出树形分类树\n4、修改分类\n5、删除分类\n0、返回上级\n");
	printf("****************************\n");
	printf("请输入你的选择（0-5）：");
}

