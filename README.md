# Heyiwei

何意味

## 摘要

这是一个使用 C++ 编写的学生水费管理系统。使用的开发工具为：Visual Studio 2022（编译环境 ISO C++14 Standard）、Visual Studio Code（文档编写）。

这是一个控制台应用，用户在终端界面查看信息、输入指令、输入信息完成基础交互功能。

现有的基础功能包括：

1.  分页查看所有学生信息
2.  添加、删除学生
3.  根据学号查询学生
4.  分页查看单个学生的所有水费记录
5.  添加、删除水费记录
6.  查询特定年月份的水费记录
7.  数据的保存与读取

## 参与成员

不在此处呈现。请打开 `members.md` 查看。

## 系统设计

### 总流程图

```mermaid
flowchart TD
    main[main] --> app_run["app.run()"]

    app_run -->enter_student[添加学生]
    app_run -->exit_program[退出程序]
    app_run -->list_all_students[列出所有学生列表]

    list_all_students --> find_student[依据学号查找选择学生]
    find_student --> operate_student[操作选择的学生]

    operate_student --> set_name[修改姓名]
    operate_student --> delete_student[删除学生]
    operate_student --> list_records[列出所有水费记录列表]

    list_records --> find_record[依据年月份查找<br/>选择水费记录]
    list_records --> add_record[添加水费记录]

    find_record --> operate_record[操作单个水费记录]

    operate_record --> set_record[设置水费记录]
    operate_record --> delete_record[删除水费记录]
```

*  **结构体和类储存学生数据。** `Student` 和 `WaterRecord` 定义数据结构。
   数据结构样式：

    ```cpp
    struct WaterRecord {
        int year;
        int month;
        double usage;
        double cost;
    };

    class Student { // 学生类，包含学号、姓名和水费记录等信息
    public:
        std::string id;
        std::string name;
        std::vector<WaterRecord> records;

        double getTotalUsage() const;
        double getTotalCost() const;
        int getWaterRecordIndex(int year, int month);
    };
    ```

	**此处呈现的代码不一定是最终代码，最终代码请翻阅文件查看。*

*  **使用动态数组供运行时修改。** 使用 `std::vector<Students>` 类型动态储存数据，便于在运行时访问和修改。

*  **json 文件数据格式支持。** 引入外部库 `json.hpp` 用于保存和解析数据文件，数据结构一目了然。

*  **清晰明了的架构设计。** `WaterManager` 类执行数组读取、修改的职责，不关心控制台界面设计；`App` 类实现控制台交互功能，不关心数据如何修改。

*  **灵活的指针与引用操作。** 向其他函数传递对象的指针或引用，数据操作方便高效。

### 类型

#### App 类

实现控制台终端交互功能。

##### 声明

```cpp
class App
{
private:
	WaterManager& manager;
	const double PRICE_PER_TON = 2.5; // 定义常量水费
public:
	App(WaterManager& mgr) : manager(mgr) {}
	void run();
	void listAllStudents();
	void listAllRecords(const std::string id);

	void addStudent();
	void addWaterRecord(const std::string id);

	void operateOnStudent(const std::string id);
	void operateOnRecord(const std::string id, int year, int month);

	void setName(const std::string id);
	void setWaterRecord(const std::string id, int year, int month);

	bool promptContinue();
	bool enterWaterRecord(WaterRecord& record);
	bool enterStudent(Student& student);
	bool enterId(std::string& id);
	bool enterName(std::string& name);
	bool enterYear(int& year);
	bool enterMonth(int& month);
	bool enterUsage(double& usage);
};
```

**此处呈现的代码不一定是最终代码，最终代码请翻阅文件查看。*

##### `App.run()` 方法

类型：`void`

说明：展示主菜单列表。输入标识选择操作功能。

源代码：

```cpp
void App::run()
{
	//程序开始运行的地方
	std::cout << "===================================================\n";
	std::cout << "  欢迎使用学生水费管理系统\n";

	while (1) {
		std::cout << "---------------------------------------------------\n";
		std::cout << " 根据以下标识输入操作：\n";
		std::cout << "\t1.\t列出所有学生信息\n";
		std::cout << "\t2.\t添加学生\n";
		std::cout << "\t/exit\t退出程序\n";
		std::cout << "---------------------------------------------------\n\n";
		std::cout << "请输入操作标识：";
 
		// 读取用户输入
		std::string input;
		std::getline(std::cin, input);
  
		// 判断是否输入了退出指令
		if (checkExit(input)) {
			std::cout << "程序即将退出\n";
			return;
		}

		// 判断是否在扣问号
		if (input == "?" || input == "？" || input == "." || input == "。") {
			std::cout << input + "\n";
			continue;
		}

		int operation = 0;
		try {
			// 尝试将用户输入转换为数字。转换失败将进入 catch 内部
			operation = std::stoi(input);
		}
		catch (...) {
			std::cout << "你输的啥？\n";
			continue; // 进入下一个循环，重新输入内容
		}

		// 判断输入的序号是否在操作选项列表内
		if (operation < 1 || operation > 2) {
			std::cout << "输入的序号不在操作选项列表中\n";
			continue;
		}

		std::cout << "\n";

		switch (operation) {
		case 1:
			listAllStudents();
			break;
		case 2:
			addStudent();
			break;
		}
	}
}
```

**此处呈现的代码不一定是最终代码，最终代码请翻阅文件查看。*

流程图：

```mermaid
flowchart TD
	start["App.run()"] --> entry[输入标识]
	entry --> |1| list_all_students["列出所有学生列表<br/>listAllStudents()"]
	entry --> |2| add_student["添加学生<br/>addStudent()"]
	entry --> |/exit| exit[退出函数]
	entry -.-> |其他| prompt_error[提示错误]
	prompt_error --> entry
```

---

##### `App.addStudent()` 方法

类型：`void`

说明：展示添加学生菜单列表。输入学号和姓名添加学生，或输入 `/exit` 标识取消添加操作。

源代码：

```cpp
void App::addStudent() {
	Student student;
	while (1) {
		if (!enterStudent(student)) return;

		result res = manager.addStudent(student);
		if (!res.success) {
			std::cout << "添加失败！原因：" << res.info;
		}
		else {
			std::cout << "添加成功：" << res.info;
		}

		if (!promptContinue()) return;
	}
}
```

**此处呈现的代码不一定是最终代码，最终代码请翻阅文件查看。*

流程图：

```mermaid
flowchart LR
    entry["addStudent()"] --> enter_id[输入学号]
    enter_id --> enter_name[输入姓名]
    enter_name --> manager_add_student["添加学生<br/>addStudent(student)"]
    manager_add_student --> |完成| continue_enter{是否继续输入？}

    continue_enter --> |yes| enter_id
    continue_enter --> |no| exit_function([退出函数])

    enter_id -.-> |"/exit"| exit_function
    enter_name -.-> |"/exit"| exit_function
```

---

##### `App.listAllStudents()` 方法

类型：`void`

说明：展示所有学生列表。输入标识或页码翻阅页面浏览，或输入 s[序号] 查找选择学生。

源代码：

```cpp
int allStudentsPageIndex = 1; // 全局变量，可用于保存上次查阅时的页面序号

void App::listAllStudents()
{
	int* pageIndex = &allStudentsPageIndex; // 使用指针
	result res;
	while (1) {
		res = manager.getAllStudents(pageIndex, 16); // 传入页面索引的指针，调用的函数内部对索引的修改会反映在此。
		std::cout << "---------------------------------------------------\n";
		std::cout << res.info;
		std::string input;

		while (1) {
			std::cout << "\n---------------------------------------------------\n";
			std::cout << "\tn\t进入下一页；\n";
			std::cout << "\tp\t进入上一页；\n";
			std::cout << "\t数字\t指定要查询的页数；\n";
			std::cout << "\ts[学号]\t对指定学号的学生执行操作\n";
			std::cout << "\t/exit\t返回上一级；\n";
			std::cout << "---------------------------------------------------\n\n";
			std::cout << "请输入操作标识：";
			std::getline(std::cin, input);

			// 判断是否输入了空内容
			if (input.empty()) {
				std::cout << "你需要输入一些内容，或输入 exit 返回上一级";
				continue;
			}

			// 判断是否输入了退出指令
			if (checkExit(input)) return;

			// 判断输入的 s[学号] 格式
			if (input.length() >= 2 && (input[0] == 's' || input[0] == 'S')) {
				std::string numStr = input.substr(1);
				if (manager.getStudent(numStr) == nullptr) {
					std::cout << "\n没有找到指定学号的学生\n";
					break;
				}
				operateOnStudent(numStr);
				break;
			}

			try {
				// 将输入的字符串转换为数字
				*pageIndex = std::stoi(input);
				break;
			}
			catch (...) {
			}

			if (input == "n" || input == "next" ) {
				(*pageIndex)++; // 下一页
				break;
			}

			if (input == "p" || input == "previous") {
				(*pageIndex)--; // 上一页
				break;
			}

			std::cout << "你输入的标识不合法";
		}
	}
}
```

**此处呈现的代码不一定是最终代码，最终代码请翻阅文件查看。*

流程图：

```mermaid
flowchart TD
    start["listAllStudents()"] --> init_pointer[初始化全局页面指针]
    init_pointer --> get_students["manager.getAllStudents<br/>(pageIndex, 16)"]
    get_students --> show_info[列出所有学生信息]

    show_info --> entry[输入标识]
    entry --> |n| next_page[下一页 pageIndex++]
    entry --> |p| previous_page[上一页 pageIndex--]
    entry --> |数字| target_page[目标页 pageIndex = target]
    entry --> |"s[学号]"| operate_on_student["设置特定学生<br/>operateOnStudent(id)"]
    entry --> |"/exit"| exit([退出函数])

    next_page --> get_students
    previous_page --> get_students
    target_page --> get_students
    operate_on_student --> get_students
```

---

##### `App.listAllRecords(const std::string id)` 方法

类型：`void`

说明：展示所有水费记录列表。输入标识或页码翻阅页面浏览，或输入 s[年-月] 查找选择记录。

源代码：

```cpp
int allRecordsPageIndex = 1;

void App::listAllRecords(const std::string id)
{
	int* pageIndex = &allRecordsPageIndex;
	result res;

	while (1) {
		res = manager.getAllRecords(id, pageIndex, 16);
		std::cout << "\n---------------------------------------------------\n";
		std::cout << res.info;
		std::string input;

		while (1) {
			std::cout << "\n---------------------------------------------------\n";
			std::cout << "\tn\t进入下一页；\n";
			std::cout << "\tp\t进入上一页；\n";
			std::cout << "\t数字\t指定要查询的页数；\n";
			std::cout << "\ts[年-月]\t对指定月份的记录执行操作\n";
			std::cout << "\t/exit\t返回上一级；\n";
			std::cout << "---------------------------------------------------\n\n";
			std::cout << "请输入操作标识：";
			std::getline(std::cin, input);

			if (input.empty()) {
				std::cout << "你需要输入一些内容，或输入 exit 返回上一级";
				continue;
			}
   
			// 判断是否输入了退出指令
			if (checkExit(input)) return;
  
			// 判断输入的 s[年-月] 格式
			if (input.length() >= 2 && (input[0] == 's' || input[0] == 'S')) {
				std::string dateStr = input.substr(1);

				// 解析年-月格式
				size_t dashPos = dateStr.find('-');
				if (dashPos == std::string::npos) {
					std::cout << "格式错误，请使用 年-月 格式，例如：s2026-04";
					continue;
				}

				try {
					//尝试将输入的字符串转换为数字
					int year = std::stoi(dateStr.substr(0, dashPos));
					int month = std::stoi(dateStr.substr(dashPos + 1));

					// 验证年月有效性
					if (year < 1 || month < 1 || month > 12) {
						std::cout << "年份或月份无效";
						continue;
					}

					// 调用操作指定月份记录的函数
					operateOnRecord(id, year, month);

					// 操作完成后刷新当前页面
					break;
				}
				catch (...) {
					std::cout << "年月格式不正确，请使用数字";
					continue;
				}
			}

			try {
				*pageIndex = std::stoi(input);
				break;
			}
			catch (...) {
			}

			if (input == "n" || input == "next") {
				(*pageIndex)++;
				break;
			}

			if (input == "p" || input == "previous") {
				(*pageIndex)--;
				break;
			}

			std::cout << "你输入的标识不合法";
		}
	}
}
```

**此处呈现的代码不一定是最终代码，最终代码请翻阅文件查看。*

流程图：

```mermaid
flowchart TD
    start["listAllRecords()"] --> init_pointer[初始化全局页面指针]
    init_pointer --> get_records["manager.getAllRecords<br/>(pageIndex, 16)"]
    get_records --> show_info[列出所有水费信息]

    show_info --> entry[输入标识]
    entry --> |n| next_page[下一页 pageIndex++]
    entry --> |p| previous_page[上一页 pageIndex--]
    entry --> |数字| target_page[目标页 pageIndex = target]
    entry --> |"s[年-月]"| operate_on_record["设置特定水费记录<br/>operateOnRecord(id, <br/>year, month)"]
    entry --> |"/exit"| exit([退出函数])

    next_page --> get_records
    previous_page --> get_records
    target_page --> get_records
    operate_on_record --> get_records
```

##### `App.operateOnStudent(const std::string id)` 方法

类型：`void`

说明：对单个学生执行操作。输入指定标识查看所有水费记录、设置姓名、添加水费记录、移除学生。

源代码：

```cpp
void App::operateOnStudent(const std::string id) {
	std::string input;
	auto* student = manager.getStudent(id);
	if (student == nullptr) {
		std::cout << "没有找到目标学号的学生";
		return;
	}
	while (1) {
		std::cout << "\n 学号：" + id + " 姓名：" + student->name + "\n";
		std::cout << "---------------------------------------------------\n";
		std::cout << " 根据以下标识输入操作：\n";
		std::cout << "\t1.\t查询他的所有水费记录\n";
		std::cout << "\t2.\t设置他的姓名\n";
		std::cout << "\t3.\t添加他的水费记录\n";
		std::cout << "\t4.\t移除他\n";
		std::cout << "\t/exit\t返回上一级\n";
		std::cout << "---------------------------------------------------\n\n";
		std::cout << "输入操作标识：";

		std::getline(std::cin, input);

		if (checkExit(input)) return;

		if (input.empty()) {
			std::cout << "你需要输入一些内容，或输入 exit 返回上一级\n";
			continue;
		}

		int operation = 0;
		try {
			operation = std::stoi(input);
		}
		catch (...) {
			std::cout << "你输的啥？ 按下ENTER键重新输入\n";
			std::getline(std::cin, input);
			continue;
		}

		if (operation < 1 || operation > 4) {
			std::cout << "输入的序号不在操作选项列表中 按下ENTER键重新输入\n";
			std::getline(std::cin, input);
			continue;
		}

		result res;

		switch (operation) {
		case 1:
			listAllRecords(id);
			break;
		case 2:
			setName(id);
			break;
		case 3:
			addWaterRecord(id);
			break;
		case 4:
			std::cout << "确定要移除他吗？(yes/no)";
			std::getline(std::cin, input);
			if (input == "yes") {
				res = manager.removeStudent(id);
				std::cout << res.info + "\n";
				return;
			}
			else {
				std::cout << "已取消移除操作";
			}
		}
	}
}
```

**此处呈现的代码不一定是最终代码，最终代码请翻阅文件查看。*

流程图：

```mermaid
flowchart TD
    start["operateOnStudent(id)"] --> check_exist{检查学号是否存在}

    check_exist --> |不存在| exit(退出函数)
    check_exist --> |存在| entry[输入标识]
    
    entry --> |1| list_all_records["列出所有水费记录列表<br/>listAllRecords(id)"]
    entry --> |2| set_name["设置姓名<br/>set_name(id)"]
    entry --> |3| add_record["添加水费记录<br/>addWaterRecord(id)"]
    entry --> |4| remove_this[移除该学生]
    entry --> |"/exit"| exit

    list_all_records --> entry
    set_name --> entry
    add_record --> entry
    remove_this --> confirm{确认删除}

    confirm --> |yes| exit
    confirm --> |no| entry
```

---

##### `App.operateOnRecord(const std::string id, int year, int month)`

类型：`void`

说明：对单个水费记录执行操作。输入指定标识设置这个水费记录、移除这个水费记录。

源代码：

```cpp
void App::operateOnRecord(const std::string id, int year, int month)
{
	std::string input;
	auto* student = manager.getStudent(id);
	if (student == nullptr) {
		std::cout << "没有找到目标学号的学生";
		return;
	}
	int index = student->getWaterRecordIndex(year, month);
	if (index == -1) {
		std::cout << "未找到目标年月份的水费记录";
		return;
	}
	while (1) {
		std::cout <<
			"\n 学号：" + id +
			std::to_string(year) + " 年 " +
			std::to_string(month) + " 月的水费记录  " +
			std::to_string(student->records[index].usage) + " （吨） " +
			std::to_string(student->records[index].cost) + " （元）\n";
		std::cout << "---------------------------------------------------\n";
		std::cout << " 根据以下标识输入操作：\n";
		std::cout << "\t1.\t设置这个水费记录\n";
		std::cout << "\t2.\t移除这个水费记录\n";
		std::cout << "\t/exit\t返回上一级\n";
		std::cout << "---------------------------------------------------\n\n";
		std::cout << "输入操作标识：";

		std::getline(std::cin, input);

		if (checkExit(input)) return;

		if (input.empty()) {
			std::cout << "你需要输入一些内容，或输入 exit 返回上一级\n";
			continue;
		}

		int operation = 0;
		try {
			operation = std::stoi(input);
		}
		catch (...) {
			std::cout << "你输的啥？ 按下ENTER键重新输入\n";
			std::getline(std::cin, input);
			continue;
		}

		if (operation < 1 || operation > 2) {
			std::cout << "输入的序号不在操作选项列表中 按下ENTER键重新输入\n";
			std::getline(std::cin, input);
			continue;
		}

		result res;

		switch (operation) {
		case 1:
			setWaterRecord(id, year, month);
			break;
		case 2:
			std::cout << "确定要移除这个吗？(yes/no)";
			std::getline(std::cin, input);
			if (input == "yes") {
				res = manager.removeWaterRecord(id, year, month);
				std::cout << res.info + "\n";
				return;
			}
			break;
		}
	}
}
```

**此处呈现的代码不一定是最终代码，最终代码请翻阅文件查看。*

流程图：

```mermaid
flowchart TD
    start["operateOnRecord(id,<br/> year, month)"] --> check_exist{检查学号和水费<br/>记录是否存在}

    check_exist --> |不存在| exit(退出函数)
    check_exist --> |存在| entry[输入标识]
    
    entry --> |1| set_record["设置这个水费记录<br/>setWaterRecord(id,<br/> year, month)"]
    entry --> |2| remove_this[移除这个水费记录]
    entry --> |"/exit"| exit

    set_record --> entry
    remove_this --> confirm{确认删除}

    confirm --> |yes| exit
    confirm --> |no| entry
```

---

##### `App.setName(const std::string id)`

类型：`void`

说明：设置指定学生的名字

源代码：

```cpp
void App::setName(const std::string id) {
	std::string name;
	if (!enterName(name)) return;
	result res = manager.setStudent(id, name);
	std::cout << res.info;
}
```

**此处呈现的代码不一定是最终代码，最终代码请翻阅文件查看。*

流程图：

未制作

---

##### `App.setWaterRecord(const std::string id, int year, int month)`

类型：`void`

说明：设置指定学生在指定年月的水费记录

源代码：

```cpp
void App::setWaterRecord(const std::string id, int year, int month)
{
	double usage;
	if (!enterUsage(usage)) return;
	result res = manager.setWaterRecord(id, year, month, usage);
	std::cout << res.info;
}
```

**此处呈现的代码不一定是最终代码，最终代码请翻阅文件查看。*

流程图：

未制作

---

##### `App.enterStudent(Student& student)`

类型：`bool`

说明：设置指定学生在指定年月的水费记录

源代码：

```cpp
bool App::enterStudent(Student& student) {

	std::cout << "---------------------------------------------------\n";
	std::cout << "\t学号和姓名\t添加学生；\n";
	std::cout << "\t/exit\t在中途取消操作；\n";
	std::cout << "---------------------------------------------------\n\n";

	std::string id;
	if (!enterId(id)) return false;

	std::string name;
	if (!enterName(name)) return false;

	student.id = id;
	student.name = name;

	return true;
}
```

**此处呈现的代码不一定是最终代码，最终代码请翻阅文件查看。*

流程图：

未制作

---

##### `App.enterId(std::string& id)`

类型：`bool`

说明：输入学生学号

源代码：

```cpp
bool App::enterId(std::string& id)
{
	std::string input;

	while (1) {
		std::cout << "请输入学号：";
		std::getline(std::cin, input);

		if (checkExit(input)) return false;

		if (input.empty()) {
			std::cout << "你需要输入一些内容，或输入 exit 取消添加操作\n";
			continue;
		}

		Student* student = manager.getStudent(input);
		if (student == nullptr) break;
		std::cout << "不可添加具有相同学号的学生\n";
		continue;
	}

	id = input;
	return true;
}
```

**此处呈现的代码不一定是最终代码，最终代码请翻阅文件查看。*

流程图：

未制作

---

##### `App.enterName(std::string& name)`

类型：`bool`

说明：输入学生姓名

源代码：

```cpp
bool App::enterName(std::string& name)
{
	std::string input;

	while (1) {
		std::cout << "请输入姓名：";
		std::getline(std::cin, input);

		if (checkExit(input)) return false;

		if (input.empty()) {
			std::cout << "你需要输入一些内容，或输入 exit 取消添加操作\n";
			continue;
		}
		break;
	}

	name = input;
	return true;
}
```

**此处呈现的代码不一定是最终代码，最终代码请翻阅文件查看。*

流程图：

未制作

---

##### `App.enterMonth(int& month)`

类型：`bool`

说明：输入月份

源代码：

```cpp
bool App::enterMonth(int& month) {
	std::string input;
	while (1) {
		std::cout << "\n请输入月份：";
		std::getline(std::cin, input);

		if (checkExit(input)) return false;

		if (input.empty()) {
			std::cout << "你需要输入一些内容，或输入 exit 取消添加操作\n";
			continue;
		}

		try {
			month = std::stoi(input);
			if (month >= 1 && month <= 12) break;
		}
		catch (...) {
			std::cout << "请输入一个有效数字\n";
			continue;
		}

		std::cout << "你输入的月份不合法\n";
	}
	return true;
}
```

**此处呈现的代码不一定是最终代码，最终代码请翻阅文件查看。*

流程图：

未制作

---

##### `App.enterUsage(double& usage)`

类型：`bool`

说明：输入水费记录

源代码：

```cpp
bool App::enterUsage(double& usage) {
	std::string input;
	while (1) {
		std::cout << "\n请输入用水量（吨数）：";
		std::getline(std::cin, input);

		if (checkExit(input)) return false;

		if (input.empty()) {
			std::cout << "你需要输入一些内容，或输入 exit 取消添加操作\n";
			continue;
		}

		try {
			usage = std::stod(input);
			if (usage < 0) throw;
			break;
		}
		catch (...) {
			std::cout << "你输入的数据不合法\n";
			continue;
		}
	}
	return true;
}
```

**此处呈现的代码不一定是最终代码，最终代码请翻阅文件查看。*

流程图：

未制作

---

##### `App.promptContinue()`

类型：`bool`

说明：提示是否继续

源代码：

```cpp
bool App::promptContinue() {
	std::string input;

	std::cout << "\n---------------------------------------------------\n";
	std::cout << "\t按下ENTER\t继续添加操作；\n";
	std::cout << "\t/exit\t返回上一级；\n";
	std::cout << "---------------------------------------------------\n";
	std::cout << "请输入标识：";
	std::getline(std::cin, input);

	return !checkExit(input);
}
```

**此处呈现的代码不一定是最终代码，最终代码请翻阅文件查看。*

流程图：

未制作

---

##### `App.addWaterRecord(const std::string id)`

类型：`void`

说明：添加水费记录

源代码：

```cpp
void App::addWaterRecord(const std::string id)
{
	while (1) {
		WaterRecord record;
		if (!enterWaterRecord(record)) return;

		result res = manager.addWaterRecord(id, record);
		std::cout << (res.success ? "成功：" : "失败：") << res.info << "\n";

		if (!promptContinue()) return;
	}
}
```

**此处呈现的代码不一定是最终代码，最终代码请翻阅文件查看。*

流程图：

未制作

---

#### WaterManager 类

实现数据管理功能。

##### 声明

```cpp
struct result {
	bool success; // 利用布尔值判断成功与否
	std::string info; // 利用一个字符串字段来对成功与否的说明
};

// 水费管理类的声明
class WaterManager
{
private: // 外部函数无法调用部分
	std::vector<Student> students; // 定义一个用于储存每个学生数据的动态数组
	const double PRICE_PER_TON = 2.5; // 定义常量水费

	int findStudentIndex(const std::string& id); // 传递id这个数据用传递地址的方法
	void saveToFile();
	void loadFromFile();

public: // 外部函数可以调用部分
	WaterManager();
	~WaterManager();

	result getAllStudents(int* pageIndex, int count);
	result getAllRecords(const std::string& id, int* paggeIndex, int count);
	Student* getStudent(const std::string& id);

	result queryTotalRecord(const std::string& id);

	result addStudent(Student student);
	result setStudent(const std::string& id, const std::string& name);
	result removeStudent(const std::string& id);

	result addWaterRecord(const std::string& id, const WaterRecord& record);
	result setWaterRecord(const std::string& id, int year, int month, double usage);
	result removeWaterRecord(const std::string& id, int year, int month);
};
```

**此处呈现的代码不一定是最终代码，最终代码请翻阅文件查看。*

##### `addStudent(Student student)` 方法

类型：

`void`

说明：

源代码：

流程图：

```mermaid
flowchart LR
    start["addStudent(student)"] --> check_exist{检查学号是否存在}

    check_exist --> |yes| exit[退出函数]
    check_exist --> |no| check_format{检查是否包含程序标识符}

    check_format --> |yes| exit
    check_format --> |no| add_student[添加学生]

    add_student --> exit
```

---

##### `loadFromFile()` 方法

类型：

`void`

说明：

源代码：

流程图：

```mermaid
flowchart LR
    start["loadFromFile()"] --> open_file[使用 std::ifstream 打开文件]
    open_file --> check_open{检查是否打开}
    check_open --> |no| open_failed[读取失败]

    open_failed --> exit[退出函数]

    check_open --> |yes| check_empty{检查是否为空}

    check_empty --> |yes| open_failed

    check_empty --> |no| check_format{解析格式是否正确}
    check_format --> |yes| load_data[加载数据]
    load_data --> exit

    check_format --> |no| parse_failed[解析失败]

    parse_failed --> backup_file[备份文件]
    backup_file --> exit

```