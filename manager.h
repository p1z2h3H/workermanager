#pragma once
#include <iostream>
#include <string>
#include "worker.h"
using namespace std;

// 经理类
class Manager : public Worker
{
public:
	Manager(int id, string name, int dId);

	// 显示个人信息
	virtual void showInfo() override;

	// 获取职工岗位名称
	virtual string getDeptName() override;
};
