#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "worker.h"
using namespace std;

#define FILENAME "empFile.txt"

// 管理类：负责菜单交互、职工增删改查、文件读写
class WorkerManager
{
public:
	WorkerManager();
	~WorkerManager();

	// 菜单界面
	void Show_Menu();

	// 退出系统
	void exitSystem();

	// 增加职工
	void Add_Emp();

	// 保存文件
	void save();

	// 统计文件中的人数
	int get_EmpNum();

	// 初始化职工数组
	void init_Emp();

	// 显示职工
	void Show_Emp();

	// 删除职工
	void Del_Emp();

	// 判断职工是否存在：存在返回数组下标，不存在返回 -1
	int IsExist(int id);

	// 修改职工
	void Mod_Emp();

	// 查找职工
	void Find_Emp();

	// 排序职工
	void Sort_Emp();

	// 清空文件
	void Clean_File();

	int m_EmpNum;          // 记录当前职工人数
	Worker **m_EmpArray;   // 指向堆区数组，数组元素是 Worker* （多态基类指针）
	bool m_FileIsEmpty;    // 标志文件是否为空
};
