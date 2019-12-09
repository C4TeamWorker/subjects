#pragma once

#include<string.h>
#include<string>
#include<stdio.h>
#include<stdlib.h>
 //���ӵĻ���string �ᱨ δ֪��д˵����
using namespace std;
/*
	SIGN: �����
*/
// 0-4
enum Tag {
	START, END, SIGN, INT, FLOAT   
};

/**
 * �Ӽ��˳�
 * LP: �����ţ�Left parenthesis��
 * RP: �����ţ�Right parenthesis��
 */
enum Sign {
	PLUS = '+', MINUS = '-', MUL = '*', DIV = '/', LP = '(', RP = ')'
};

/**
 * token������ݽṹ�ͺ���
 * tag��ʾtoken����
 * value��ʾ����ֵ
 */
typedef struct _Token
{
	int tag;
	string key;
	int flag;
	union Value
	{
		int sign;
		int i_val;
		float f_val;
	} value;

}Token;