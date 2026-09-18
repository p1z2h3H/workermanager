#include <iostream>
#include <cstdlib>
#include "workerManager.h"
using namespace std;

int main()
{
#ifdef _WIN32
	// 源码是 UTF-8，把控制台切到 UTF-8 才能正确显示中文
	system("chcp 65001 > nul");
#endif

	WorkerManager wm;
	int choice = 0;

	while (true)
	{
		// 展示菜单
		wm.Show_Menu();

		cout << "请输入您的选择:" << endl;

		// 【修正】教程未处理输入非数字的情况，会导致死循环
		if (!(cin >> choice))
		{
			cin.clear();
			cin.ignore(1024, '\n');
			system("cls");
			continue;
		}

		switch (choice)
		{
		case 0: // 退出系统
			wm.exitSystem();
			break;
		case 1: // 添加职工
			wm.Add_Emp();
			break;
		case 2: // 显示职工
			wm.Show_Emp();
			break;
		case 3: // 删除职工
			wm.Del_Emp();
			break;
		case 4: // 修改职工
			wm.Mod_Emp();
			break;
		case 5: // 查找职工
			wm.Find_Emp();
			break;
		case 6: // 排序职工
			wm.Sort_Emp();
			break;
		case 7: // 清空文件
			wm.Clean_File();
			break;
		default:
			system("cls");
			break;
		}
	}

	system("pause");
	return 0;
}
