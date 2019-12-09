#pragma once
#include "defs.h"
#include "errorout.h"
#include "Fmystack.h"
#include "clac.h"

using namespace std;

// iden array of defined identifiers; idennum: number of defined identifiers; errl: line number of prog
void scanner(string prog,Token* iden,int* idennum,int errl)// prog: line being processed
{
	int p = 0;
	char ch = prog[p];
	Token token;
	token.key = "";

	int n = prog.find('=');

	//û��'=',��ôֻ����������int a; ����� write(a)  �ٶ����������������a�� int����ĿҪ����������ֵ
	if (n == -1) {
		if (prog[0] == 'w')   //write
		{
			int i = prog.find('(');
			i += 1;
			char c = prog[i];
			token.key = "";
			if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
				while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
					token.key += c;
					i++;
					c = prog[i];
				}
				int j = 0;
				for (j = 0; j < *idennum; j++)
				{
					if (token.key == iden[j].key)
					{
						break;
					}


				}
				if (j == *idennum)
				{
					error("undefined identifier in write",errl);   //write��һ�����﷨�������ڵı�ʶ������write�Ķ��󲻺��﷨���� write(a*b)
					//system("pause");                               //�ݲ�����write(a b)��a������������ʼ�������
					//exit(1);
				}

				/*
				else if ((iden[j].tag == INT)&& iden[j].flag==1)//flag==1���Ѹ�ֵ
				{
					printf("%s = %d\n",iden[j].key, iden[j].value.i_val);
				}
				else if ((iden[j].tag == FLOAT )&& iden[j].flag == 1) {
					printf("%s = %.2f\n", iden[j].key, iden[j].value.f_val);
				}
				*/
				else if ((iden[j].tag == FLOAT) && iden[j].flag == 1)
				{
					cout << iden[j].key << " = " << iden[j].value.f_val << endl;
				}
				else if ((iden[j].tag == INT) && iden[j].flag == 1)
				{
					cout << iden[j].key << " = " << iden[j].value.i_val << endl;
				}
				else {
					error("Uninitialized identifier in write",errl);  //��ʶ����������δ��ʼ��
				}

			}

			else
			{
				error("undefined and illegal identifier in write",errl);   //write��һ�����Ϸ��ı�ʶ��
			}

		}

		else if (prog[0] == 'i') { //initial int
			//int i = prog.find(' ');
			int i = 4;            // �ٶ��ļ�������int ����ʱ��0�п�ʼ����ô����������4�п�ʼ
			char c = prog[i];
			token.key = "";
			if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
				while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
					token.key += c;
					i++;
					c = prog[i];
				}
				if (c != ';')
				{
					error("illegally defined int identifier",errl); //��������������м��������ַ�
				}

			}
			else
			{
				error("illegally defined int identifier",errl);  //���������������ʼ������ĸ
			}
			token.tag = INT;  //ע���ʱ�ñ�ʶ����δ��ֵ��ֻ������
			token.flag = 0; //undefined
			iden[*idennum] = token;
			*idennum = *idennum + 1;
		}
		//initial float  suppose like: int a;
		else if (prog[0] == 'f') 
		{
			//int i = prog.find(' ');
			int i = 6;
			char c = prog[i];
			token.key = "";
			if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
				while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
					token.key += c;
					i++;
					c = prog[i];
				}
				if (c != ';')
				{
					error("illegally defined float identifier", errl); //��������������м��������ַ�  
				}

			}
			else
			{
				error("illegally defined float identifier", errl);  //���������������ʼ������ĸ
			}
			token.tag = FLOAT;  //ע���ʱ�ñ�ʶ����δ��ֵ��ֻ������
			token.flag = 0; //undefined
			iden[*idennum] = token;
			*idennum = *idennum + 1;
		}

	}

	//��'='����ֵ����a= expr;����ʽ
	else {
		//ȷ������ֵ�ı�ʶ��
		int i = 0;
		char c = prog[i];
		token.key = "";
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
			while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
				token.key += c;
				i++;
				c = prog[i];
			}
			if (c != ' ')  //��Ϊ��׼��ʽΪ a = expr,=��ǰ��' '
			{
				error("assign to illegal identifier", errl); //����ֵ�ı�ʶ�������⡣����a+b = expr; δ���� a b = expr ������
				return;
			}

			int j = 0;
			for (j = 0; j < *idennum; j++)
			{
				if (token.key == iden[j].key)
				{
					break;
				}


			}
			if (j == *idennum)
			{
				error("assign to undefined identifier",errl);   //������������ʶ���﷨�Ϸ�����undefined
				return;
			}

			//��ȷ�ҵ��˱���ֵ�ı�ʶ��
			 //����calc����expr��ֵ������iden[j];
			//˼·�����滻��expr�еı�ʶ��;����b=a*2��a=2���Ȱ�a����2�����ַ����к���������������
			//����ʱ����aδ��ֵ��������
			
			//�滻��ʶ������ ���������Ѿ��ٶ�'='���expr��;��.���� ���������ʽΪ a = exp����=�ź���һ���ո�

			string raw = prog.substr(n+2);
			
			//���㲢��ֵ
				string expr = "";  //expr�洢ת����ı��ʽ
				int k = 0;
				int start, end; //start-end ������Ҫ���滻
				start = 0;
				int p = 0;
				//while ((raw[k] != ':') && (raw[k] != '.')&&(k<raw.length())  // out of range
				while (k < raw.length()) 
				{
					char c = raw[k];
					string cp = "";          //cp ���滻�ı�ʶ��
					if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
						end = k;
						while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
							cp += c;
							k++;
							c = raw[k];
						}
						expr = expr + raw.substr(start, end - start);
						start = k;
						for (p = 0; p < *idennum; p++)
						{
							if (cp == iden[p].key)
							{
								break;
							}
						}
						if (p == *idennum)
						{
							error("undefined identifier in expression",errl);   //expr�д��ں��﷨��δ����ı�ʶ�� 
												//��������ֻ����expr��ֻ����������⣬�����ı��ʽ���淶����1++2�ݲ�����
												 //�� b_d+3 ���������b�ǺϷ��ַ������岻�Ϸ������Ҳ��ʱ������
							return;
						}

						if (iden[p].flag == 0)
						{
							error("uninitialized identifier in expression", errl);
							return;
						}
						if(iden[p].tag==FLOAT)
							expr = expr + to_string(iden[p].value.f_val);
						else
							expr = expr + to_string(iden[p].value.i_val);
						/*if ((iden[p].tag == INT) && (iden[j].tag == FLOAT))
						{
							error("cast error: int to float",errl);  //����ת��int to float ��Ҫ���в�����
						}*/
					}
					else
					{
						k++;
					}

				}
				expr = expr + raw.substr(start, raw.length() - start); //ȥ������;
				//cout <<"final" <<expr<< endl;
				Token* rst = &iden[j]; //Ŀ���ʶ����token
				calce(expr,rst,errl);      //���㲢��ֵ
		}
		else
		{
			error("assign to illegal identifier", errl);//����ֵ�ı�ʶ����ͷ������ĸ��ͬʱundefined
		}
	}

}


