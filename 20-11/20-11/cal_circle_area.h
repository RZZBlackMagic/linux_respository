#pragma once
#include<iostream>
#include<math.h>
using namespace std;

#define pi acos(-1.0)

class circle {
public:
	double r;//԰�뾶
	double x;//Բ�ĵĺ�����
	double y;//Բ�ĵ�������
	circle() {}
	circle(double r, double x, double y) {
		this->r = r;
		this->x = x;
		this->y = y;
	}
};

double cal_area(circle &c1, circle &c2) {
	//������Բ�ľ���
	double distance = sqrt((c1.x-c2.x)*(c1.x-c2.x)+(c1.y-c2.y)*(c1.y-c2.y));
	//�ж���Բ��λ�ù�ϵ
	if (distance >= (c1.r + c2.r)) {
		//���л�������,
		return 0;
	}
	else if(fabs(c1.r-c2.r>distance)){
		//����
		if (c1.r > c2.r) {
			return pi * c2.r*c2.r;
		}
		else {
			return pi * c1.r*c1.r;
		}
	}
	else {
		double ang1=acos((c1.r*c1.r+distance*distance-c2.r*c2.r)/(2*c1.r*distance));
	    double ang2=acos((c2.r*c2.r+distance*distance-c1.r*c1.r)/(2*c2.r*distance));
	    return ang1*c1.r*c1.r + ang2*c2.r*c2.r - c1.r*distance*sin(ang1);
	}
}
