#pragma once
#include<windows.h>
#include<iostream>
#include<thread>
using namespace std;

struct Thread {
	int id;
	thread * t;
	void (*entry);
	struct Thread* next;
}Thread;

struct Thread * threads;

void func() {
	thread::id this_id = this_thread::get_id();
	cout << "线程"<<this_id<<"的函数正在执行" << endl;
}

void create(void (*func)){
	struct Thread* cur = (struct Thread*)malloc(sizeof(struct Thread));
	cur->id = threads ? threads->id + 1 : 1;
	cur->entry = (*func);
	cur->next = threads;
	threads = cur;
	cur->t = new thread(func, cur->id);
}

//thread* t;
//void create(void (*func)(int),int a) {
//	t = new thread(func,a);
//}
