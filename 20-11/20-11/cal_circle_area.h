#pragma once
#include<iostream>
#include<math.h>
using namespace std;

#define pi acos(-1.0)

class circle {
public:
	double r;//园半径
	double x;//圆心的横坐标
	double y;//圆心的纵坐标
	circle() {}
	circle(double r, double x, double y) {
		this->r = r;
		this->x = x;
		this->y = y;
	}
};

double cal_area(circle &c1, circle &c2) {
	//计算两圆的距离
	double distance = sqrt((c1.x-c2.x)*(c1.x-c2.x)+(c1.y-c2.y)*(c1.y-c2.y));
	//判断两圆的位置关系
	if (distance >= (c1.r + c2.r)) {
		//相切或者相离,
		return 0;
	}
	else if(fabs(c1.r-c2.r>distance)){
		//包含
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
