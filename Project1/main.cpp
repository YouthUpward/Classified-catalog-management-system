
//Ŀ¼����ϵͳ��˫�ױ�ʾ��ʵ��N������˳��洢
#define _CRT_SECURE_NO_WARNINGS
#define MAX_TREE_SIZE 100			//��������
#include <stdio.h>
#include <iostream>
using namespace std;

typedef struct Catalog				//Ŀ¼�ṹ����
{
	char key[20] = { 0 };			//���
	char caption[80] = { 0 };		//����
	char parent[20] = { 0 };		//���ױ��
}Catalog;

typedef struct CatalogTree
{
	Catalog Node[MAX_TREE_SIZE] = { 0 };	//������,Node���±�ͱ�ʾ��ĳ����Ԫ��Ϣ��˳��洢�е�λ��
	int root = 0;					//���ڵ�λ�ã����ڵ�Ĭ����0�ŵ�Ԫ
	int num = 0;					//���һ������������
}CatalogTree;

CatalogTree tree, * ct;				//���ֱ����tree���գ�Ӧ��д��fwrite(&tree->Node[i],sizeof(Catalog),1,fp)
FILE* fp = fopen("catalog3.dat", "rb+");//�ļ�ָ��
Catalog C = { "###","###","###" };	//��ʼ��һ���ṹ��
int t = 0;							//��¼����
int f = 0;							//�洢�±�

void CreateCatalogTree();			//0 ���ļ���������
void AddCatalog();					//1 ���ӷ�����Ϣ���������ļ�
void AlterCatalog();				//2 �޸ķ�����Ϣ���������ļ�
void DeleteCatalog();				//3 ���ļ���ɾ��������Ϣ
void PrintCatalog();				//4 ˳������ļ������з�����Ϣ
void CreateCatalogTree();			//5.1 ��������Ϣ��˫�ױ�ʾ���洢������PrintCatalogTree()
void PrintCatalogTree();			//5.2 ����PreOrder()����

void PreOrder(CatalogTree* ct, int i);
void PostOrder(CatalogTree* ct, int i);
bool leaf(CatalogTree* ct, int i);

void menu();

int main()
{
	if (fp == NULL)
	{
		printf("�ļ���ʧ��\n");
		return 0;
	}
	printf("**116132019188 ��˼�� ����**\n");
	CreateCatalogTree();			//��.dat�ļ�д������
	for (;;)
	{
		menu();
		int choice;
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:AddCatalog(); break;
		case 2:PrintCatalog(); break;		//˳�����������Ϣ
		case 3:PrintCatalogTree(); break;	//������η�����,���������Ϊʲô��Щ��&���ǰ�
		case 4:AlterCatalog(); break;
		case 5:DeleteCatalog(); break;
		default:return 0;
		}
	}
	fclose(fp);								//�ر��ļ���
	return 0;
}


/*���ļ���������*/
void CreateCatalogTree()
{
	tree.num = 0;								//��ʼ���������
	for (int i = 0; !feof(fp); i++)				//����ļ�û�ж��꣬һ�ζ���һ��Catalog
	{
		fread(&tree.Node[i], sizeof(Catalog), 1, fp);		//���ļ��ж���
	//	printf("%d\t%s\t%s\t%s\n",i,tree.Node[i].key, tree.Node[i].caption, tree.Node[i].parent);
		tree.num++;
	}//������ע�⣬������һ��������
	tree.num--;
	//	printf("tree.num=%d",tree.num);
}



/*��ʼд���ӷ��֧࣬�ֶ��������¼���������ļ�*/
void AddCatalog()
{
	char temp[20] = { 0 };
	printf("����������key��caption��parent����#�������룩\n");	//����������ֱ��д������
	for (;;)
	{
		scanf("%s", temp);
		if (strcmp(temp, "#") != 0)
		{
			strcpy(tree.Node[tree.num].key, temp);
			scanf("%s %s", tree.Node[tree.num].caption, tree.Node[tree.num].parent);	//��������

			//����������д���ļ�
			fwrite(&tree.Node[tree.num], sizeof(Catalog), 1, fp);
			tree.num++;
		}
		else break;
	}
	printf("��ӳɹ���\n");
}//�������ӷ����Ѿ�д��


/*˳���ӡ����*/
void PrintCatalog()
{
	for (int i = 0; i < tree.num; i++)
	{
		if (strcmp(tree.Node[i].caption, "###") != 0)
			printf("%s\t%s\t%s\n", tree.Node[i].key, tree.Node[i].caption, tree.Node[i].parent);
	}
}


/*���δ�ӡ�������������������ʵ��*/
void PrintCatalogTree()
{	//�����������
	PreOrder(&tree, 0);
}


/*�������*/
void PreOrder(CatalogTree* ct, int i)			//������η�����,����Ϊʲô��*ct�أ�*ct����������
{
	//tΪ��Ӧ��������ӡ�ո�
	for (int i = 0; i < t; i++)
		printf("   ");

	printf("|--%s  %s\n", tree.Node[i].caption, tree.Node[i].key);

	for (int j = i + 1; j < tree.num; j++)		//���ڽ��i����Ҫ�ͺ���Ľ��Ƚ�
	{	//������������ȶ��ҵ����i�������ӽ��
		if (strcmp(ct->Node[j].parent, ct->Node[i].key) == 0)
		{
			t++;
			PreOrder(ct, j);
			t--;
		}
	}
}


/*ɾ�����ࣺ��������ɾ����Ҷ�ӽ��ɾ��������ɾ�����ú������*/
void DeleteCatalog()
{
	char a[20] = { 0 };
	int c;
	printf("����������Ҫɾ����key: ");
	scanf("%s", &a);						//�û������Ӧ����һ��key

	for (f = 0; f < tree.num; f++)
	{//����ÿ�������˵
		if (strcmp(tree.Node[f].key, a) == 0)		//�ҵ��ý�㣬��һ��ɾ�����������Ľ��
			break;
	}
	printf("��ɾ����Ϣ��%s   %s   %s\n", tree.Node[f].key, tree.Node[f].caption, tree.Node[f].parent);
	if (leaf(&tree, f))
	{
		printf("tip����Ŀ¼������Ŀ¼����һ��ɾ����\n�������ɾ���������밴1;����2��");
		scanf("%d", &c);
		if (c != 1)
		{
			printf("�˳�ɾ��\n");
			return;
		}
	}

	PostOrder(&tree, f);
	printf("ɾ���ɹ���\n");
}


/*�������*/
void PostOrder(CatalogTree* ct, int i)			//������η�����,����Ϊʲô��*ct�أ�*ct����������
{
	for (int j = i + 1; j < tree.num; j++)		//���ڽ��i����Ҫ�ͺ���Ľ��Ƚ�
	{	//������������ȶ��ҵ����i�������ӽ��
		if (strcmp(ct->Node[j].parent, ct->Node[i].key) == 0)
		{
			t++;
			PostOrder(ct, j);
			t--;
		}
	}

	//�����ļ�
	fseek(fp, i * sizeof(Catalog), 0);
	fwrite(&C, sizeof(Catalog), 1, fp);

	//����Ӧtree.Node[f]��Ϊ�գ������ļ���������໥��Ӧ��
	strcpy(tree.Node[i].key, "###");
	strcpy(tree.Node[i].caption, "###");
	strcpy(tree.Node[i].parent, "###");
}


/*Ҷ�ӽ���жϣ����ز�����*/
bool leaf(CatalogTree* ct, int i)
{
	for (int j = i + 1; j < tree.num; j++)		//���ڽ��i����Ҫ�ͺ���Ľ��Ƚ�
	{	//������������ȶ��ҵ����i�������ӽ��
		if (strcmp(ct->Node[j].parent, ct->Node[i].key) == 0)
			return true;
	}
	return false;
}


/*�޸ķ��ࣺ�����Ҷ�ӽ�㣬���޸�������¼������ֻ���޸�caption*/
void AlterCatalog()
{
	printf("����������Ҫ�޸ĵ�key: ");
	int f = 0;										//�洢�±�
	char a[80] = { 0 };
	scanf("%s", &a);								//�û������Ӧ����һ��key

	for (f = 0; f < tree.num; f++)					//����ÿ�������˵
	{
		if (strcmp(tree.Node[f].key, a) == 0)		//�ҵ��ý��
			break;
	}

	//������޸���Ϣ��д������
	printf("���޸���Ϣ��%s   %s   %s\n", tree.Node[f].key, tree.Node[f].caption, tree.Node[f].parent);
	if (leaf(&tree, f) == false)						//��Ҷ�ӽ��
	{
		printf("������key��caption��parent��");
		scanf("%s%s%s", tree.Node[f].key, tree.Node[f].caption, tree.Node[f].parent);	//�Ѿ�д������
	}
	else
	{
		printf("tip�����ڸ�Ŀ¼�»�����Ŀ¼������ֻ���޸�caption��\n������caption��");
		scanf("%s", tree.Node[f].caption);
	}

	//�����ļ�
	fseek(fp, f * sizeof(Catalog), 0);
	fwrite(&tree.Node[f], sizeof(Catalog), 1, fp);
	printf("�޸ĳɹ���\n");
}


/*��ӡ�˵�*/
void menu()
{
	printf("\n************�˵�************\n");
	printf("1�����ӷ���\n2���������\n3��������η�����\n4���޸ķ���\n5��ɾ������\n0�������ϼ�\n");
	printf("****************************\n");
	printf("���������ѡ��0-5����");
}

