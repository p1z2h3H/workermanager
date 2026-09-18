# 职工管理系统（C++ 多态 + 文件读写）

一个基于**多态**和**文件持久化**的控制台职工管理系统，实现了教程中的全部 8 个功能。

## 一、项目结构

```
E:\code\workermanager\
├── worker.h              抽象基类 Worker：定义纯虚函数 showInfo() / getDeptName()
├── employee.h/.cpp       普通员工类（部门编号 1）
├── manager.h/.cpp        经理类（部门编号 2）
├── boss.h/.cpp           老板类（部门编号 3）
├── workerManager.h/.cpp  管理类：菜单交互 + 增删改查 + 文件读写
├── main.cpp              主函数：菜单循环 + switch 分发
├── empFile.txt           运行时自动生成的数据文件（不在仓库中预置）
└── README.md
```

> 教程里的主文件名为 `职工管理系统.cpp`，这里改成 `main.cpp`，避免部分 Windows 工具链处理中文文件名时出问题，代码内容完全一致。

## 二、编译运行

### MinGW / g++（Windows、Linux、macOS 通用）

```bash
# Git Bash
cd /e/code/workermanager
# CMD / PowerShell 下用：cd /d E:\code\workermanager

g++ -std=c++17 -Wall -Wextra -o empSys \
    main.cpp workerManager.cpp employee.cpp manager.cpp boss.cpp
./empSys
```

> 注意：`empFile.txt` 生成在**当前工作目录**，所以请先 `cd` 到项目目录再运行，否则数据文件会落到别处。

Windows 下如果控制台中文乱码，代码里已经用 `chcp 65001` 处理过了；若仍乱码，请确认源文件保存为 **UTF-8 无 BOM**。

### Visual Studio

1. 新建空项目，把 5 个 `.h` / 6 个 `.cpp` 全部添加进去；
2. 项目属性 → C/C++ → 命令行 → 其他选项，填入 `/utf-8`；
3. 直接 F5 运行。

### 一键构建

```bash
bash build.sh
```

## 三、功能清单

| 序号 | 功能 | 说明 |
|------|------|------|
| 0 | 退出管理程序 | 释放堆区内存后退出 |
| 1 | 增加职工信息 | 批量添加，按岗位创建不同子类对象，写入文件 |
| 2 | 显示职工信息 | 通过基类指针调用 `showInfo()`，运行时多态 |
| 3 | 删除离职职工 | 按编号删除，同步文件 |
| 4 | 修改职工信息 | 按编号定位，销毁旧对象、创建新对象 |
| 5 | 查找职工信息 | 支持按编号 / 按姓名两种方式（姓名支持重名全显示） |
| 6 | 按照编号排序 | 选择排序，升序 / 降序由用户指定 |
| 7 | 清空所有文档 | 二次确认后清空文件与内存 |

## 四、相对原教程的修正

原教程代码直接照抄是**编译不过**的，另有几处内存问题。以下为逐条修正：

| # | 位置 | 原教程问题 | 修正 |
|---|------|-----------|------|
| 1 | `Mod_Emp()` | 写成 `case1:` / `case2:`，**编译直接报错** | 改为 `case 1:` / `case 2:` |
| 2 | `~WorkerManager()` | 只 `delete[]` 了数组，数组内 `new` 出的职工对象全部泄漏 | 先逐个 `delete` 元素，再 `delete[]` 数组 |
| 3 | `Del_Emp()` | 指针前移覆盖后，被删对象未 `delete`，内存泄漏 | 删除前先 `delete m_EmpArray[index]` |
| 4 | `Worker` 基类 | 无虚析构函数，`delete` 基类指针属未定义行为 | 增加 `virtual ~Worker() {}` |
| 5 | 构造函数「情况 3」 | 漏写 `m_FileIsEmpty = false`，导致有数据却提示「记录为空」 | 补上标志更新 |
| 6 | `Add_Emp()` / `Mod_Emp()` | 岗位选择未校验，输错会往数组里塞 `NULL`，下次显示直接崩溃 | 加 `do-while` 校验，非法输入重新选择 |
| 7 | `Clean_File()` | 清空标志写在 `if (m_EmpArray != NULL)` 内部，数组本为 NULL 时不生效 | 标志更新移到 `if` 外部 |
| 8 | `main()` | `cin >> choice` 未处理非数字输入，会陷入死循环 | 失败时 `clear()` + `ignore()` 后继续 |
| 9 | `main()` | Windows 控制台默认 GBK，中文输出乱码 | 启动时 `chcp 65001` |
| 10 | 头文件 | 教程靠 `.cpp` 里隐式包含，头文件单独编译会缺 `Worker` 定义 | `workerManager.h` 显式 `#include "worker.h"` |

## 五、涉及的知识点

- **多态三要素**：继承 + 虚函数重写 + 基类指针指向派生类对象
- **纯虚函数与抽象类**：`Worker` 不可实例化，只能通过指针/引用使用
- **虚析构函数**：有虚函数且有堆内存时几乎必备
- **指针数组的动态扩容**：`new Worker*[newSize]` → 搬移旧指针 → `delete[]` 旧数组
- **文件流三种状态**：不存在（`!ifs.is_open()`）、空（`ifs.eof()`）、有数据
- **文本文件格式**：每行 `编号 姓名 部门编号`，用空格分隔，`>>` 可自动跳过空白

## 六、数据文件格式

`empFile.txt` 每行一条记录，字段以空格分隔：

```
1 张三 1
2 李四 2
3 王五 3
```

第三列即部门编号：`1` 普通员工、`2` 经理、`3` 老板，程序据此在 `init_Emp()` 中还原出对应的子类对象。
