#include "interpreter.h"
#include<iostream>
#include<fstream>
#include<stdio.h>
#include <stdlib.h>
#include<string.h>
using namespace std;
void init(){
	/*设置指令名称*/
	strcpy(&(mnemonic[LIT][0]),"LIT");
	strcpy(&(mnemonic[LOD][0]),"LOD");
	strcpy(&(mnemonic[STO][0]),"STO");
	strcpy(&(mnemonic[CAL][0]),"CAL");
	strcpy(&(mnemonic[INT][0]),"INT");
	strcpy(&(mnemonic[JMP][0]),"JMP");
	strcpy(&(mnemonic[JPC][0]),"JPC");
	strcpy(&(mnemonic[ADD][0]),"ADD");
	strcpy(&(mnemonic[SUB][0]),"SUB");
	strcpy(&(mnemonic[MUL][0]),"MUL");
	strcpy(&(mnemonic[DIV][0]),"DIV");
	strcpy(&(mnemonic[RED][0]),"RED");
	strcpy(&(mnemonic[WRT][0]),"WRT");
	strcpy(&(mnemonic[RET][0]),"RET");
}

//解释程序部分
int base(int l,int b)
{
	int b1;
	b1=b;
	return b1;
}
//解释目标代码
void interpret()
{
	int p,b,t;
	struct instruction i;//指令
	int s[stacksize];//栈式数据区
	t=0;//栈顶寄存器
	b=0;//基址地址
	p=0;//指向下一条执行的目标程序的地址
	s[0]=s[1]=s[2]=0;//三个静态连、动态连，返回地址设置为零
	do
	{
		i=code[p];
		p++;
		switch(i.f)
		{
		case LIT://将常数值取到栈顶，a为常数值
			s[t]=i.a;
			t++;
			break;
		case LOD: //将变量值取到栈顶，a为相对地址，t为层数           
			s[t]=s[base(i.l,b)+i.a];
			t++;
			break;
		case STO:    //将栈顶内容送入某变量单元中，a为相对地址，t为层数   
			t--;
			s[base(i.l,b)+i.a]=s[t];
			break;
		case CAL:                  //调用函数，a为函数地址
			s[t]=base(i.l,b);        //当前基址入栈
			s[t+1]=b;                  //本过程基址入栈  DL    (和s[t]一样)
			s[t+2]=p;                  //当前指令地址入栈 RA(返回地址)  在判断程序是否结束时起作用
			b=t;            //新基址
			p=i.a;
			
			break;
		case INT: //在运行栈中为被调用的过程开辟a个单元的数据区
			t=t+i.a;
			break;
		case JMP://无条件跳转至a地址
			p=i.a;
			break;
		case JPC://条件跳转，当栈顶值为0，则跳转至a地址，否则顺序执行
			t--;
			if(s[t]==0)
			{
				p=i.a;
			}
			break;
		case ADD://次栈顶与栈顶相加，退两个栈元素，结果值进栈
			t--;
			s[t-1]=s[t-1]+s[t];
			break;
		case SUB://次栈顶减去栈顶，退两个栈元素，结果值进栈
			t--;
			s[t-1]=s[t-1]-s[t];
			break;
		case MUL://次栈顶乘以栈顶，退两个栈元素，结果值进栈
			t--;
			s[t-1]=s[t-1]*s[t];
			break;
		case DIV://次栈顶除以栈顶，退两个栈元素，结果值进栈
			t--;
			if(s[t-1]!=0)
				s[t-1]=s[t-1]/s[t];
			else
				cout<<"input error"<<endl;
			break;
		case RED://从命令行读入一个输入置于栈顶

			cout<<"input:";
			cin>>s[t];
			t++;
			break;
		case WRT://栈顶值输出至屏幕并换行
			cout<<"result:"<<s[t-1]<<endl;  
			fprintf(fa2,"%d\n",s[t-1]);
			t--;
			break;
		case RET://函数调用结束后,返回调用点并退栈
			t=b;     
			p=s[t+2];       //由t找到调用函数前的p值     要返回的地址为0时程序结束
			b=s[t+1];
			break;
		}
	}while(p!=0);
}
//目标代码解释执行，翻译
void translate(char *s2,int ll,int aa,int i)  //处理读取的文本内容
{
	if(strcmp(s2,"LIT")==0)
	{
		code[i].f=(fct)0;
	}
	else if(strcmp(s2,"LOD")==0)
	{
		code[i].f=(fct)1;
	}
	else if(strcmp(s2,"STO")==0)
	{
		code[i].f=(fct)2;
	}
	else if(strcmp(s2,"CAL")==0)
	{
		code[i].f=(fct)3;
	}
	else if(strcmp(s2,"INT")==0)
	{
		code[i].f=(fct)4;
	}
	else if(strcmp(s2,"JMP")==0)
	{
		code[i].f=(fct)5;
	}
	else if(strcmp(s2,"JPC")==0)
	{
		code[i].f=(fct)6;
	}
	else if(strcmp(s2,"ADD")==0)
	{
		code[i].f=(fct)7;
	}
	else if(strcmp(s2,"SUB")==0)
	{
		code[i].f=(fct)8;
	}
	else if(strcmp(s2,"MUL")==0)
	{
		code[i].f=(fct)9;
	}
	else if(strcmp(s2,"DIV")==0)
	{
		code[i].f=(fct)10;
	}
	else if(strcmp(s2,"RED")==0)
	{
		code[i].f=(fct)11;
	}
	else if(strcmp(s2,"WRT")==0)
	{
		code[i].f=(fct)12;
	}
	else if(strcmp(s2,"RET")==0)
	{
		code[i].f=(fct)13;
	}
	code[i].l=ll;
	code[i].a=aa;
}

//解释执行
void intercall()
{
	cout<<"c0目标代码解释执行："<<endl;
	ifstream in("fa.tmp",ios::in);//输入文件流
	if(!in)//如果文件不存在，则打印错误
	{
		cerr<<"file could not be open";
		return;
	}
	//S2存指令类型
	char s1[10],s2[10];
	//ll存的是层数
	int ll,aa;
	int i=0;     //当前指令执行的序号
	while(in>>s1>>s2>>ll>>aa)//将文本存入code数组
	{
		translate(s2,ll,aa,i);
		i++;
	}
	interpret();
}
void main(){
				fa2=fopen("fa2.tmp","w");	//先将fa2.tmp文件转为读状态
				intercall();    //调用解释程序
			    fclose(fa2);	//关闭fa2.tmp文件	
}