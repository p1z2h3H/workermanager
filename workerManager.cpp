#include "workerManager.h"
#include "employee.h"
#include "manager.h"
#include "boss.h"

// ============================ 构造 / 析构 ============================

WorkerManager::WorkerManager()
{
	ifstream ifs;
	ifs.open(FILENAME, ios::in);

	// 情况1：文件不存在（第一次使用）
	if (!ifs.is_open())
	{
		this->m_EmpNum = 0;
		this->m_FileIsEmpty = true;
		this->m_EmpArray = NULL;
		ifs.close();
		return;
	}

	// 情况2：文件存在，但内容为空（被清空过）
	char ch;
	ifs >> ch;
	if (ifs.eof())
	{
		this->m_EmpNum = 0;
		this->m_FileIsEmpty = true;
		this->m_EmpArray = NULL;
		ifs.close();
		return;
	}

	// 情况3：文件存在且保存了数据
	ifs.close();

	int num = this->get_EmpNum();
	this->m_EmpNum = num;
	this->m_FileIsEmpty = false; // 【修正】教程漏写，会导致有数据却提示"记录为空"

	this->m_EmpArray = new Worker *[this->m_EmpNum];
	this->init_Emp();
}

WorkerManager::~WorkerManager()
{
	if (this->m_EmpArray != NULL)
	{
		// 【修正】教程只 delete[] 了数组本身，数组里 new 出来的职工对象全部泄漏
		for (int i = 0; i < this->m_EmpNum; i++)
		{
			if (this->m_EmpArray[i] != NULL)
			{
				delete this->m_EmpArray[i];
				this->m_EmpArray[i] = NULL;
			}
		}

		delete[] this->m_EmpArray;
		this->m_EmpArray = NULL;
	}
}

// ============================ 菜单 / 退出 ============================

void WorkerManager::Show_Menu()
{
	cout << "********************************************" << endl;
	cout << "*********  欢迎使用职工管理系统！ **********" << endl;
	cout << "*************  0.退出管理程序  *************" << endl;
	cout << "*************  1.增加职工信息  *************" << endl;
	cout << "*************  2.显示职工信息  *************" << endl;
	cout << "*************  3.删除离职职工  *************" << endl;
	cout << "*************  4.修改职工信息  *************" << endl;
	cout << "*************  5.查找职工信息  *************" << endl;
	cout << "*************  6.按照编号排序  *************" << endl;
	cout << "*************  7.清空所有文档  *************" << endl;
	cout << "********************************************" << endl;
	cout << endl;
}

void WorkerManager::exitSystem()
{
	cout << "欢迎下次使用" << endl;
	system("pause");
	exit(0);
}

// ============================ 增加职工 ============================

void WorkerManager::Add_Emp()
{
	cout << "请输入增加职工数量： " << endl;

	int addNum = 0;
	cin >> addNum;

	if (addNum > 0)
	{
		// 计算新空间大小
		int newSize = this->m_EmpNum + addNum;

		// 开辟新空间
		Worker **newSpace = new Worker *[newSize];

		// 将原空间下的内容拷贝到新空间
		if (this->m_EmpArray != NULL)
		{
			for (int i = 0; i < this->m_EmpNum; i++)
			{
				newSpace[i] = this->m_EmpArray[i];
			}
		}

		// 输入新数据
		for (int i = 0; i < addNum; i++)
		{
			int id;
			string name;
			int dSelect;

			cout << "请输入第 " << i + 1 << " 个新职工编号：" << endl;
			cin >> id;

			cout << "请输入第 " << i + 1 << " 个新职工姓名：" << endl;
			cin >> name;

			cout << "请选择该职工的岗位：" << endl;
			cout << "1、普通职工" << endl;
			cout << "2、经理" << endl;
			cout << "3、老板" << endl;

			// 【修正】教程未校验输入，选错会存入一个 NULL 指针，后续解引用直接崩溃
			do
			{
				cin >> dSelect;
				if (dSelect < 1 || dSelect > 3)
				{
					cout << "输入有误，请重新选择（1/2/3）：" << endl;
				}
			} while (dSelect < 1 || dSelect > 3);

			Worker *worker = NULL;
			switch (dSelect)
			{
			case 1: // 普通员工
				worker = new Employee(id, name, 1);
				break;
			case 2: // 经理
				worker = new Manager(id, name, 2);
				break;
			case 3: // 老板
				worker = new Boss(id, name, 3);
				break;
			}

			newSpace[this->m_EmpNum + i] = worker;
		}

		// 释放原有空间（注意：只释放数组，不释放里面的对象，对象所有权已转移）
		delete[] this->m_EmpArray;

		// 更改新空间的指向
		this->m_EmpArray = newSpace;

		// 更新新的个数
		this->m_EmpNum = newSize;

		// 更新文件不为空的标志
		this->m_FileIsEmpty = false;

		// 保存到文件
		this->save();

		cout << "成功添加" << addNum << "名新职工！" << endl;
	}
	else
	{
		cout << "输入有误" << endl;
	}

	system("pause");
	system("cls");
}

// ============================ 文件读写 ============================

void WorkerManager::save()
{
	ofstream ofs;
	ofs.open(FILENAME, ios::out);

	for (int i = 0; i < this->m_EmpNum; i++)
	{
		ofs << this->m_EmpArray[i]->m_Id << " "
			<< this->m_EmpArray[i]->m_Name << " "
			<< this->m_EmpArray[i]->m_DeptId << endl;
	}

	ofs.close();
}

int WorkerManager::get_EmpNum()
{
	ifstream ifs;
	ifs.open(FILENAME, ios::in);

	int id;
	string name;
	int dId;
	int num = 0;

	while (ifs >> id && ifs >> name && ifs >> dId)
	{
		num++;
	}

	ifs.close();
	return num;
}

void WorkerManager::init_Emp()
{
	ifstream ifs;
	ifs.open(FILENAME, ios::in);

	int id;
	string name;
	int dId;
	int index = 0;

	while (ifs >> id && ifs >> name && ifs >> dId)
	{
		Worker *worker = NULL;

		// 根据部门编号创建不同子类对象 —— 多态的关键
		if (dId == 1) // 普通员工
		{
			worker = new Employee(id, name, dId);
		}
		else if (dId == 2) // 经理
		{
			worker = new Manager(id, name, dId);
		}
		else // 老板
		{
			worker = new Boss(id, name, dId);
		}

		this->m_EmpArray[index] = worker;
		index++;
	}

	ifs.close();
}

// ============================ 显示职工 ============================

void WorkerManager::Show_Emp()
{
	if (this->m_FileIsEmpty)
	{
		cout << "文件不存在或记录为空！" << endl;
	}
	else
	{
		for (int i = 0; i < this->m_EmpNum; i++)
		{
			// 利用多态调用接口
			this->m_EmpArray[i]->showInfo();
		}
	}

	system("pause");
	system("cls");
}

// ============================ 删除职工 ============================

int WorkerManager::IsExist(int id)
{
	int index = -1;

	for (int i = 0; i < this->m_EmpNum; i++)
	{
		if (this->m_EmpArray[i]->m_Id == id)
		{
			index = i;
			break;
		}
	}

	return index;
}

void WorkerManager::Del_Emp()
{
	if (this->m_FileIsEmpty)
	{
		cout << "文件不存在或记录为空！" << endl;
	}
	else
	{
		cout << "请输入想要删除的职工号：" << endl;
		int id = 0;
		cin >> id;

		int index = this->IsExist(id);

		if (index != -1)
		{
			// 【修正】教程只把指针前移，被删掉的对象没有 delete，造成内存泄漏
			delete this->m_EmpArray[index];

			for (int i = index; i < this->m_EmpNum - 1; i++)
			{
				this->m_EmpArray[i] = this->m_EmpArray[i + 1];
			}

			this->m_EmpNum--;

			// 删除后同步到文件
			this->save();
			cout << "删除成功！" << endl;
		}
		else
		{
			cout << "删除失败，未找到该职工" << endl;
		}
	}

	system("pause");
	system("cls");
}

// ============================ 修改职工 ============================

void WorkerManager::Mod_Emp()
{
	if (this->m_FileIsEmpty)
	{
		cout << "文件不存在或记录为空！" << endl;
	}
	else
	{
		cout << "请输入修改职工的编号：" << endl;
		int id;
		cin >> id;

		int ret = this->IsExist(id);

		if (ret != -1)
		{
			// 释放旧对象
			delete this->m_EmpArray[ret];

			int newId = 0;
			string newName = "";
			int dSelect = 0;

			cout << "查到： " << id << "号职工，请输入新职工号： " << endl;
			cin >> newId;

			cout << "请输入新姓名： " << endl;
			cin >> newName;

			cout << "请输入岗位： " << endl;
			cout << "1、普通职工" << endl;
			cout << "2、经理" << endl;
			cout << "3、老板" << endl;

			// 【修正】教程未校验输入，选错会写入 NULL 指针
			do
			{
				cin >> dSelect;
				if (dSelect < 1 || dSelect > 3)
				{
					cout << "输入有误，请重新选择（1/2/3）：" << endl;
				}
			} while (dSelect < 1 || dSelect > 3);

			Worker *worker = NULL;
			switch (dSelect)
			{
			case 1: // 【修正】教程写成 case1: 会直接编译失败
				worker = new Employee(newId, newName, dSelect);
				break;
			case 2: // 【修正】同上
				worker = new Manager(newId, newName, dSelect);
				break;
			case 3:
				worker = new Boss(newId, newName, dSelect);
				break;
			}

			// 更改数据到数组中
			this->m_EmpArray[ret] = worker;

			cout << "修改成功！" << endl;

			// 保存到文件中
			this->save();
		}
		else
		{
			cout << "修改失败，查无此人" << endl;
		}
	}

	system("pause");
	system("cls");
}

// ============================ 查找职工 ============================

void WorkerManager::Find_Emp()
{
	if (this->m_FileIsEmpty)
	{
		cout << "文件不存在或记录为空！" << endl;
	}
	else
	{
		cout << "请输入查找的方式：" << endl;
		cout << "1、按职工编号查找" << endl;
		cout << "2、按姓名查找" << endl;

		int select = 0;
		cin >> select;

		if (select == 1) // 按职工号查找
		{
			int id;
			cout << "请输入查找的职工编号：" << endl;
			cin >> id;

			int ret = IsExist(id);

			if (ret != -1)
			{
				cout << "查找成功！该职工信息如下：" << endl;
				this->m_EmpArray[ret]->showInfo();
			}
			else
			{
				cout << "查找失败，查无此人" << endl;
			}
		}
		else if (select == 2) // 按姓名查找
		{
			string name;
			cout << "请输入查找的姓名：" << endl;
			cin >> name;

			bool flag = false;

			for (int i = 0; i < m_EmpNum; i++)
			{
				if (m_EmpArray[i]->m_Name == name)
				{
					cout << "查找成功,职工编号为："
						<< m_EmpArray[i]->m_Id
						<< " 号的信息如下：" << endl;

					flag = true;
					this->m_EmpArray[i]->showInfo();
				}
			}

			if (flag == false)
			{
				cout << "查找失败，查无此人" << endl;
			}
		}
		else
		{
			cout << "输入选项有误" << endl;
		}
	}

	system("pause");
	system("cls");
}

// ============================ 排序职工 ============================

void WorkerManager::Sort_Emp()
{
	if (this->m_FileIsEmpty)
	{
		cout << "文件不存在或记录为空！" << endl;
		system("pause");
		system("cls");
	}
	else
	{
		cout << "请选择排序方式： " << endl;
		cout << "1、按职工号进行升序" << endl;
		cout << "2、按职工号进行降序" << endl;

		int select = 0;
		cin >> select;

		// 选择排序
		for (int i = 0; i < m_EmpNum; i++)
		{
			int minOrMax = i;

			for (int j = i + 1; j < m_EmpNum; j++)
			{
				if (select == 1) // 升序
				{
					if (m_EmpArray[minOrMax]->m_Id > m_EmpArray[j]->m_Id)
					{
						minOrMax = j;
					}
				}
				else // 降序
				{
					if (m_EmpArray[minOrMax]->m_Id < m_EmpArray[j]->m_Id)
					{
						minOrMax = j;
					}
				}
			}

			if (i != minOrMax)
			{
				// 这里只交换指针，不交换对象
				Worker *temp = m_EmpArray[i];
				m_EmpArray[i] = m_EmpArray[minOrMax];
				m_EmpArray[minOrMax] = temp;
			}
		}

		cout << "排序成功,排序后结果为：" << endl;
		this->save();
		this->Show_Emp();
	}
}

// ============================ 清空文件 ============================

void WorkerManager::Clean_File()
{
	cout << "确认清空？" << endl;
	cout << "1、确认" << endl;
	cout << "2、返回" << endl;

	int select = 0;
	cin >> select;

	if (select == 1)
	{
		// ios::trunc 表示存在则先删除再重建
		ofstream ofs(FILENAME, ios::trunc);
		ofs.close();

		if (this->m_EmpArray != NULL)
		{
			for (int i = 0; i < this->m_EmpNum; i++)
			{
				if (this->m_EmpArray[i] != NULL)
				{
					delete this->m_EmpArray[i];
				}
			}
			delete[] this->m_EmpArray;
			this->m_EmpArray = NULL;
		}

		// 【修正】教程把这两句写在 if 内部，数组本来就是 NULL 时标志不会更新
		this->m_EmpNum = 0;
		this->m_FileIsEmpty = true;

		cout << "清空成功！" << endl;
	}

	system("pause");
	system("cls");
}
