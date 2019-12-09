#pragma once
#include "defs.h"
#include "Fmystack.h"



/**
 * scan: ɨ���ַ�������ȡ��һ��token
 * cal: ����������ʽ�Ľ��
 * cal_token: ��������token��Ԫ����Ľ����signΪ���������������result������
 * get_op_level:��ȡ�������������ȼ�
 */

int scan(const char* expr, int start, Token* current);
void cal(const char* expr,Token* aim,int errl);
void cal_token(Token* t1, Token* t2, int sign, Token* result,int errl,int*flag);//������divide by 0ʱflag=1��ֱ�ӽ������ʽ����
int get_op_level(int op,int errl);
void calce(string expr, Token* rst, int errl);

void calce(string expr,Token* rst,int errl) {


	//string to char*

	//�����Ĳο�������char*����׺���ʽ����ʱ��ô�ã���ʱ���һ��
	string tmp = expr;
	tmp = tmp.substr(0, tmp.length() - 1);
	//char* ccc = tmp.c_str(); //c_str()���ص���һ����ʱָ�룬���ܶ�����в�����
	char c[100];
	strcpy_s(c, tmp.c_str());
	cal(c, rst,errl);

}



int get_op_level(int op,int errl) {
	switch (op)
	{
	case '(':
		return 1;
	case '+':
	case '-':
		return 2;
	case '*':
	case '/':
		return 3;
	case ')':
		return 4;
	default:
		error("bad operator",errl);
		return 0;
	}
}

void cal(const char* expr,Token * aim,int errl) {
	int len = strlen(expr);
	int location = 0; // �±�
	Token current;
	current.flag = 0;
	current.key = "";
	Stack post;
	Stack ops;

	current.tag = START;

	init_stack(&post);
	init_stack(&ops);
	while (current.tag != END)
	{
		location = scan(expr, location, &current);
		switch (current.tag)
		{
		case START:
			//unexpected case
			error("scan error: find no token",errl);
			break;
		case END:
			break;
		case INT:
		case FLOAT:
			push(&post, current);
			break;
		case SIGN: {
			int sign = current.value.sign;
			switch (sign)
			{
			case LP:
				push(&ops,current);
				break;
			case RP:
				do
				{
					Token* op;
					if (ops.size == 0)
					{
						error("parentheses mismatch",errl);
					}
					op = pop(&ops);
					push(&post, *op);

				} while (top(&ops)->value.sign != LP);
				pop(&ops);
				break;
			case PLUS:
			case MINUS:
			case MUL:
			case DIV: {
				if (ops.size == 0)
				{
					push(&ops, current);
					break;
				}
				int top_op = top(&ops)->value.sign;
				if (get_op_level(sign,errl) > get_op_level(top_op,errl))
				{
					push(&ops, current);
					break;
				}
				else {
					while (ops.size != 0)
					{
						int top_op = top(&ops)->value.sign;
						Token* op;
						if (get_op_level(sign,errl) <= get_op_level(top_op,errl))
						{
							op = pop(&ops);
							push(&post, *op);
						}
						else {
							break;
						}
					}
					push(&ops, current);
					break;
				}
			}
			default:
				break;
			}
			break;
		}
		default:
			//unexpected case
			error("scan error: find unexpected token",errl);
			break;
		}
	}

	
	while (ops.size != 0)
	{
		Token* op = pop(&ops);
		push(&post, *op);
	}


	free_stack(&ops);
	// ת���ĺ�׺���Ԫ�����෴�ģ�������Ҫ��תջ������
	inverse(&post);
	// �����׺���ʽ
	Stack tmp;
	init_stack(&tmp);

	while (post.size != 0) {
		int tag = top(&post)->tag;
		switch (tag)
		{
		case INT:
		case FLOAT:
			push(&tmp, *pop(&post));
			break;
		case SIGN: {
			Token t;
			int sign = pop(&post)->value.sign;
			switch (sign) {
			case '+':
			case '-':
			case '*':
			case '/': {
				Token* value2 = pop(&tmp);
				Token* value1 = pop(&tmp);
				int flag = 0;
				cal_token(value1, value2, sign, &t,errl,&flag);
				if (flag == 1)
				{
					return; //ֱ�ӽ������ʽ����
				}
				push(&tmp, t);
				break;
			}
			}
			break;
		}
		default:
			error("wrong in postfix expression",errl);
			break;
		}
	}
	if (tmp.size != 1) {
		printf("%d\n", tmp.size);
		error("bad expression",errl);
	}

	// ����Ľ��
	Token* cal_result = pop(&tmp);
	if (cal_result->tag == INT)
	{
		if (aim->tag == FLOAT)
			error("cast error:int to float",errl);  //����ת������
		else
		{
			aim->value.i_val = cal_result->value.i_val;
			aim->flag = 1;						//flag=1,�Ѹ�ֵ
		}
	}
	else{
		if (aim->tag == FLOAT) {
			aim->value.f_val = cal_result->value.f_val;
			aim->flag = 1;
		}
		else
		{
			aim->value.i_val = cal_result->value.f_val;
			aim->flag = 1;						//flag=1,�Ѹ�ֵ
		}
	}

}

int scan(const char* expr, int start, Token* current) {
	int len = strlen(expr);
	int offset = start;
	char curr_char = expr[offset];
	while (offset < len)
	{
		// �����ո�
		if (curr_char == ' ')
		{
			curr_char = expr[++offset];
			continue;
		}

		if (curr_char == '\0')
		{
			current->tag = END;
			return offset + 1;
		}

		// ���������
		if (curr_char == '+' || curr_char == '-' || curr_char == '*' || curr_char == '/'
			|| curr_char == '(' || curr_char == ')')
		{
			current->tag = SIGN;
			current->value.sign = curr_char;
			return offset + 1;
		}

		// ��������
		if (curr_char >= '0' && curr_char <= '9')
		{
			int is_float = 0;
			int int_val = 0;
			float float_val;

			do
			{
				int_val = int_val * 10 + (curr_char - '0');
				curr_char = expr[++offset];
			} while (curr_char >= '0' && curr_char <= '9');

			// �����С���㣬��Ϊfloat
			if (curr_char == '.')
			{
				float_val = int_val;
				curr_char = expr[++offset];
				float times = 10.0f;

				int overflow = 0;
				while (curr_char >= '0' && curr_char <= '9') {
					if (overflow)
					{
						curr_char = expr[++offset];
						continue;
					}

					float_val += (curr_char - '0') / times;
					curr_char = expr[++offset];
					times *= 10;
					// ��ֹ���
					if (times > 0xffffffffu / 10)
					{
						overflow = 1;
					}
				}

				current->tag = FLOAT;
				current->value.f_val = float_val;
				return offset;
			}

			current->tag = INT;
			current->value.i_val = int_val;
			return offset;
		}

	}
	if (curr_char == '\0')
	{
		current->tag = END;
		return offset;
	}
}


void cal_token(Token* t1, Token* t2, int sign, Token* result,int errl,int*flag) {
	if (t1->tag == FLOAT || t2->tag == FLOAT)
	{
		result->tag = FLOAT;
		float v1, v2;
		v1 = t1->tag == FLOAT ? t1->value.f_val : t1->value.i_val;
		v2 = t2->tag == FLOAT ? t2->value.f_val : t2->value.i_val;

		switch (sign)
		{
		case '+':
			result->value.f_val = v1 + v2;
			return;
		case '-':
			result->value.f_val = v1 - v2;
			return;
		case '*':
			result->value.f_val = v1 * v2;
			return;
		case '/':
			if (v2 == 0.0f)
			{
				error("divide by zero",errl);
				*flag = 1;    //ֱ�ӷ��أ��������ʽ����
				return;
			}
			result->value.f_val = v1 / v2;
			return;
		default:
			return;
		}
	}
	else {
		result->tag = INT;
		int v1, v2;
		v1 = t1->value.i_val;
		v2 = t2->value.i_val;
		switch (sign)
		{
		case '+':
			result->value.i_val = v1 + v2;
			return;
		case '-':
			result->value.i_val = v1 - v2;
			return;
		case '*':
			result->value.i_val = v1 * v2;
			return;
		case '/':
			if (v2 == 0)
			{
				error("divide by zero",errl);
				*flag = 1;
				return;
			}
			result->value.i_val = v1 / v2;
			return;
		default:
			return;
		}
	}
}

