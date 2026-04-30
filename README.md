# Heyiwei

何意味

## 摘要

Heyiwei 是一个使用 C++ 编写的学生水费管理系统。使用的开发工具为：Visual Studio 2022（编译环境 ISO C++14 Standard）、Visual Studio Code（文档编写）。

这是一个控制台应用，用户在终端界面查看信息、输入指令、输入信息，完成基础交互功能。

### 学生信息管理

Heyiwei 支持对学生基础信息的完整管理。

1.  **分页查看所有学生信息：** 以列表形式展示所有学生，每页默认显示 16 条记录，支持上一页、下一页、跳转指定页等操作。列表中仅显示学号和姓名，界面简洁清晰。
2.  **添加学生：** 用户输入学号和姓名即可添加新学生。系统会自动检测学号是否已存在，若重复则拒绝添加并提示用户。学号和姓名不能以 `/` 开头，因为该符号被保留为程序指令标识符。
3.  **删除学生：** 根据学号删除指定学生。删除前会要求用户二次确认，防止误操作。删除学生时，该学生名下的所有水费记录也会一并移除。
4.  **根据学号查询学生：** 在学生列表界面输入 `s[学号]` 即可快速定位到指定学生，并进入该学生的操作菜单，可进一步查看其水费记录、修改姓名、添加记录或删除学生。

### 水费记录管理

Heyiwei 支持对每位学生的水费记录进行精细管理。

1.  **分页查看单个学生的所有水费记录：** 进入指定学生的操作菜单后，可查看其所有水费记录。记录以列表形式展示，包含年月份、用水量（吨）、费用（元），同样支持分页浏览。

2.  **添加水费记录：** 为指定学生添加某年某月的水费记录。需要输入年份、月份和用水量，费用自动按单价（2.5 元/吨）计算。系统会检查该年月份是否已有记录，若已存在则拒绝添加。

3.  **删除水费记录：** 删除指定学生在某年某月的水费记录。删除前需要用户二次确认。

4.  **查询特定年月份的水费记录：** 在水费记录列表界面输入 `s[年-月]`（例如 `s2026-04`）即可快速定位到指定年月份的水费记录，并进入该记录的操作菜单，可修改用水量或删除记录。

### 数据持久化

Heyiwei 支持完整的数据保存与读取功能。

1.  **数据的保存与读取：** 所有学生和水费记录以 JSON 格式保存在 `students.json` 文件中。程序启动时自动读取该文件加载历史数据，程序运行期间的每次添加、修改、删除操作都会自动保存到文件，程序正常退出时也会保存。若文件损坏或格式错误，系统会自动备份原文件为 `students.json.bak` 并重新初始化数据。

### 交互设计

Heyiwei 拥有细致完善的界面交互设计。

1.  **统一的退出机制：** 在所有输入界面输入 `/e` 即可返回到上一级菜单或退出当前操作，无需强行关闭程序。

2.  **输入合法性校验：** 系统对用户的每一次输入都进行校验，包括学号是否重复、月份是否在 1–12 范围内、用水量是否为非负数等，并给出明确的错误提示。

3.  **操作结果反馈：** 每次添加、修改、删除操作完成后，系统都会显示明确的成功或失败信息，失败时说明具体原因。

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

### 设计说明

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

*  **使用动态数组供运行时修改。** 使用 `std::vector<Student>` 类型动态储存数据，便于在运行时访问和修改。

*  **json 文件数据格式支持。** 引入外部库 `json.hpp` 用于保存和解析数据文件，数据结构一目了然。

*  **清晰明了的架构设计。** `WaterManager` 类执行数组读取、修改的职责，不关心控制台界面设计；`App` 类实现控制台交互功能，不关心数据如何修改。

*  **灵活的指针与引用操作。** 向其他函数传递对象的指针或引用，数据操作方便高效。

*  **分页查询支持。** `getAllStudents` 和 `getAllRecords` 支持分页显示，每页默认 12 条，页码自动边界检查。

*  **操作结果统一返回。** `WaterManager` 的所有修改操作返回 `result` 结构体，包含 `success`（是否成功）和 `info`（详细信息）两个字段。

*  **输入辅助函数统一处理。** `App` 类提供 `enterId`、`enterName`、`enterYear`、`enterMonth`、`enterUsage` 等函数，统一处理用户输入和合法性校验，支持 `/e` 中途退出。

*  **编码转换处理。** 提供 `gbkToUtf8` 和 `utf8ToGbk` 函数，解决 Windows 控制台（GBK）与 JSON 文件（UTF-8）之间的中文编码转换问题。

*  **数据自动保存。** `WaterManager` 构造时调用 `loadFromFile()` 读取数据，析构时调用 `saveToFile()` 保存数据，每次修改操作后也会自动保存。

*  **文件加载容错。** 加载 `students.json` 时若文件不存在或为空则正常启动；若 JSON 解析失败则自动备份原文件为 `students.json.bak`，并清空数据重新开始。

### 类型

#### App 类

##### 概要

实现控制台终端交互功能。

##### `App.run()` 方法

类型：`void`

说明：展示主菜单列表。输入标识选择操作功能。

流程图：

```mermaid
flowchart TD
	start["App.run()"] --> entry[输入标识]
	entry --> |1| list_all_students["列出所有学生列表<br/>listAllStudents()"]
	entry --> |2| add_student["添加学生<br/>addStudent()"]
	entry --> |/e| exit[退出函数]
	entry -.-> |其他| prompt_error[提示错误]
	prompt_error --> entry
```

演示截图：

![main](./images/main.png)

---

##### `App.addStudent()` 方法

类型：`void`

说明：展示添加学生菜单列表。输入学号和姓名添加学生，或输入 `/e` 标识取消添加操作。

流程图：

```mermaid
flowchart LR
    entry["addStudent()"] --> enter_id[输入学号]
    enter_id --> enter_name[输入姓名]
    enter_name --> manager_add_student["添加学生<br/>addStudent(student)"]
    manager_add_student --> |完成| continue_enter{是否继续输入？}

    continue_enter --> |yes| enter_id
    continue_enter --> |no| exit_function([退出函数])

    enter_id -.-> |"/e"| exit_function
    enter_name -.-> |"/e"| exit_function
```

演示截图：

![add-student](./images/add-student.png)

---

##### `App.listAllStudents()` 方法

类型：`void`

说明：展示所有学生列表。输入标识或页码翻阅页面浏览，或输入 `s[学号]` 查找选择学生。

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
    entry --> |"/e"| exit([退出函数])

    next_page --> get_students
    previous_page --> get_students
    target_page --> get_students
    operate_on_student --> get_students
```

演示截图：

![list-all-students](./images/list-all-students.png)

---

##### `App.listAllRecords(const std::string id)` 方法

类型：`void`

说明：展示所有水费记录列表。输入标识或页码翻阅页面浏览，或输入 `s[年-月]` 查找选择记录。

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
    entry --> |"/e"| exit([退出函数])

    next_page --> get_records
    previous_page --> get_records
    target_page --> get_records
    operate_on_record --> get_records
```

演示截图：

![list-all-records](./images/list-all-records.png)

##### `App.operateOnStudent(const std::string id)` 方法

类型：`void`

说明：对单个学生执行操作。输入指定标识查看所有水费记录、设置姓名、添加水费记录、移除学生。

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
    entry --> |"/e"| exit

    list_all_records --> entry
    set_name --> entry
    add_record --> entry
    remove_this --> confirm{确认删除}

    confirm --> |yes| exit
    confirm --> |no| entry
```

演示截图：

![operate-on-student](./images/operate-on-student.png)

---

##### `App.operateOnRecord(const std::string id, int year, int month)`

类型：`void`

说明：对单个水费记录执行操作。输入指定标识设置这个水费记录、移除这个水费记录。

流程图：

```mermaid
flowchart TD
    start["operateOnRecord(id,<br/> year, month)"] --> check_exist{检查学号和水费<br/>记录是否存在}

    check_exist --> |不存在| exit(退出函数)
    check_exist --> |存在| entry[输入标识]
    
    entry --> |1| set_record["设置这个水费记录<br/>setWaterRecord(id,<br/> year, month)"]
    entry --> |2| remove_this[移除这个水费记录]
    entry --> |"/e"| exit

    set_record --> entry
    remove_this --> confirm{确认删除}

    confirm --> |yes| exit
    confirm --> |no| entry
```

演示截图：

![operate-on-record](./images/operate-on-record.png)

---

##### `App.setName(const std::string id)`

类型：`void`

说明：设置指定学生的名字。

流程图：

未制作

演示截图：

![set-name](./images/set-name.png)

---

##### `App.setWaterRecord(const std::string id, int year, int month)`

类型：`void`

说明：设置指定学生在指定年月的水费记录。

流程图：

未制作

演示截图：

![set-record](./images/set-record.png)

---

##### `App.enterStudent(Student& student)`

类型：`bool`

流程图：

未制作

---

##### `App.enterId(std::string& id)`

类型：`bool`

说明：输入学生学号。

流程图：

未制作

---

##### `App.enterName(std::string& name)`

类型：`bool`

说明：输入学生姓名。

流程图：

未制作

---

##### `App.enterMonth(int& month)`

类型：`bool`

说明：输入月份。

流程图：

未制作

---

##### `App.enterUsage(double& usage)`

类型：`bool`

说明：输入水费记录。

流程图：

未制作

---

##### `App.promptContinue()`

类型：`bool`

说明：提示是否继续。

流程图：

未制作

---

##### `App.addWaterRecord(const std::string id)`

类型：`void`

说明：添加水费记录。

流程图：

未制作

演示截图：

![add-water-record](./images/add-water-record.png)

---

#### WaterManager 类

##### 概要

实现数据管理功能。

##### 编码转换辅助函数

由于 Windows 控制台使用 GBK 编码，而 JSON 文件使用 UTF-8 编码，需要两个辅助函数进行转换：

```cpp
std::string gbkToUtf8(const std::string& gbkStr);   // GBK → UTF-8（保存时使用）
std::string utf8ToGbk(const std::string& utf8Str);  // UTF-8 → GBK（加载时使用）
```

---

##### JSON 序列化/反序列化

使用 `nlohmann/json` 库，为 `WaterRecord` 和 `Student` 定义了 `to_json` 和 `from_json` 重载：

---

##### `WaterManager()` 构造函数

类型：无

说明：实例构造时自动调用 `loadFromFile()`，从 `students.json` 加载已有数据。

流程图：

```mermaid
flowchart LR
    start["WaterManager()"] --> load["loadFromFile()"]
    load --> exit["初始化完成"]
```

---

##### `~WaterManager()` 析构函数

类型：无

说明：实例销毁时自动调用 `saveToFile()`，将数据保存到 `students.json`。

流程图：

```mermaid
flowchart LR
    start["~WaterManager()"] --> save["saveToFile()"]
    save --> exit["程序退出，数据已保存"]
```

---

##### `WaterManager.loadFromFile()` 方法

类型：`void`

说明：从 `data.json` 文件加载数据。如果文件不存在、为空或格式错误，会进行相应处理（空文件或解析失败时会备份原文件）。

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

---

##### `WaterManager.saveToFile()` 方法

类型：`void`

说明：将当前数据保存到 `data.json` 文件。在程序退出前或每次数据修改后自动调用。

流程图：

```mermaid
flowchart LR
    start["saveToFile()"] --> convert["将 students 转换为 JSON 格式"]
    convert --> open["std::ofstream 打开 data.json"]
    open --> write["写入 JSON 数据"]
    write --> close["关闭文件"]
    close --> exit["退出"]
```

---

##### `WaterManager.findStudentIndex(const std::string& id)` 方法

类型：`int`

说明：根据学号遍历 `students` 数组，若找到匹配的学生返回其索引，否则返回 -1。

流程图：

```mermaid
flowchart LR
    start["findStudentIndex(id)"] --> loop["遍历 students 数组"]
    loop --> check{"students[i].id == id?"}
    check --> |是| return_i["返回索引 i"]
    check --> |否| next["继续下一个"]
    next --> loop_end{遍历结束？}
    loop_end --> |否| loop
    loop_end --> |是| return_minus["返回 -1"]
```

---

##### `WaterManager.addStudent(Student student)` 方法

类型：`Result`

说明：添加新学生。会检查学号是否已存在，以及学号是否包含程序保留标识符。

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

##### `WaterManager.setStudent(const std::string& id, const std::string& name)` 方法

类型：`result`

说明：修改指定学生的姓名。

流程图：

```mermaid
flowchart LR
    start["setStudent(id, name)"] --> find["findStudentIndex(id)"]
    find --> exist{学生是否存在？}
    exist --> |否| fail["返回失败：学生不存在"]
    exist --> |是| update["students[index]<br/>.name = name"]
    update --> save["saveToFile()"]
    save --> success["返回成功：显示新旧姓名"]
```

---

##### `WaterManager.removeStudent(const std::string& id)` 方法

类型：`result`

说明：删除指定学生及其所有水费记录。

流程图：

```mermaid
flowchart LR
    start["removeStudent(id)"] --> find["findStudentIndex(id)"]
    find --> exist{学生是否存在？}
    exist --> |否| fail["返回失败：学生不存在"]
    exist --> |是| erase["students.erase<br/>(students.begin() + index)"]
    erase --> save["saveToFile()"]
    save --> success["返回成功：删除成功"]
```

---

##### `WaterManager.addWaterRecord(const std::string& id, const WaterRecord& record)` 方法

类型：`result`

说明：为指定学生添加水费记录。会检查该年月份是否已有记录（不允许重复）。

流程图：

```mermaid
flowchart LR
    start["addWaterRecord(id, record)"] --> find["findStudentIndex(id)"]
    find --> exist{学生是否存在？}
    exist --> |否| fail_student["返回失败：学生不存在"]
    exist --> |是| check_dup["遍历 records 检查年月是否重复"]
    check_dup --> dup{是否重复？}
    dup --> |是| fail_dup["返回失败：该年月已有记录"]
    dup --> |否| add["students[index].records.push_back(record)"]
    add --> save["saveToFile()"]
    save --> success["返回成功：显示添加的记录详情"]
```

---

##### `WaterManager.setWaterRecord(const std::string& id, int year, int month, double usage)` 方法

类型：`result`

说明：修改指定学生在指定月份的水费记录（用水量）。费用自动按单价重新计算。

流程图：

```mermaid
flowchart LR
    start["setWaterRecord(id, year, month, usage)"] --> find["findStudentIndex(id)"]
    find --> exist{学生是否存在？}
    exist --> |否| fail_student["返回失败：学生不存在"]
    exist --> |是| find_record["遍历 records 查找年月"]
    find_record --> record_exist{记录是否存在？}
    record_exist --> |否| fail_record["返回失败：未找到记录"]
    record_exist --> |是| update["更新 usage 和 cost"]
    update --> save["saveToFile()"]
    save --> query["调用 queryTotalRecord 获取汇总"]
    query --> success["返回成功：修改记录成功 + 汇总信息"]
```

---

##### `WaterManager.removeWaterRecord(const std::string& id, int year, int month)` 方法

类型：`result`

说明：删除指定学生在指定月份的水费记录。

流程图：

```mermaid
flowchart LR
    start["removeWaterRecord(id, year, month)"] --> find["findStudentIndex(id)"]
    find --> exist{学生是否存在？}
    exist --> |否| fail_student["返回失败：学生不存在"]
    exist --> |是| get_index["getWaterRecordIndex(year, month)"]
    get_index --> record_exist{记录是否存在？}
    record_exist --> |否| fail_record["返回失败：未找到记录"]
    record_exist --> |是| erase["records.erase(records.begin() + index)"]
    erase --> save["saveToFile()"]
    save --> success["返回成功：删除成功"]
```

---

##### `WaterManager.getStudent(const std::string& id)` 方法

类型：`Student*`

说明：根据学号获取指向学生的指针。若不存在返回 `nullptr`。

流程图：

```mermaid
flowchart LR
    start["getStudent(id)"] --> find["findStudentIndex(id)"]
    find --> exist{索引是否为 -1？}
    exist --> |是| return_null["返回 nullptr"]
    exist --> |否| return_ptr["返回 &students[index]"]
```

---

## 另请参阅

[C++ 文档 | Microsoft Learn](https://learn.microsoft.com/zh-cn/cpp/cpp/?view=msvc-170 "https://learn.microsoft.com/zh-cn/cpp/cpp/?view=msvc-170")  
[JSON for Modern C++ (nlohmann/json.hpp)](https://json.nlohmann.me/ "https://json.nlohmann.me/")