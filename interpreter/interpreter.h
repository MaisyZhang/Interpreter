#include<iostream>
#include<fstream>
#include<stdio.h>

struct instruction
{
	enum fct f;
	int l;//层数
	int a;//地址
};
#define norw 8      //关键字的个数  word长度
#define al 10
#define txmax 100    //名字表容量
#define nmax 14
#define cxmax 200
#define levmax 3
#define amax 2047   //地址上界
#define stacksize 500

//枚举类型，枚举表，罗列出来所有可能出现的值
enum fct
{
	LIT,  LOD,  STO,
	CAL,  INT,  JMP,
	JPC,  ADD,  SUB,
	MUL,  DIV,  RED,
	WRT,  RET,
};
//定义存放指令名称的第一位
#define fctnum 14
//指令数组
instruction code[100];
char mnemonic[fctnum][5];   //虚拟机代码指令名称
//存放指令代码的tmp文件
FILE *fa;
//将结果写入fa2.tmp文件中
FILE *fa2;

//解释目标代码
void interpret();   
//解释程序部分
int base(int l,int b);
//目标代码解释执行，翻译
void translate(char *s2,int ll,int aa,int i); 
//解释执行
void intercall();