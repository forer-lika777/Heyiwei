# Heyiwei

何意味

```mermaid
flowchart TD
    A[开始] --> B[显示欢迎信息]
    B --> C{进入主循环}
    
    C --> D[显示主菜单]
    D --> E[获取用户输入]
    
    E --> F{输入 == exit/e/exit()?}
    F -->|是| G[退出程序]
    F -->|否| H{输入是 ?/。 等符号?}
    
    H -->|是| I[回怼符号]
    I --> D
    
    H -->|否| J[尝试转换为数字]
    J --> K{转换成功?}
    K -->|否| L[提示重新输入]
    L --> D
    
    K -->|是| M{operation 范围?}
    M -->|1| N[listAllStudents]
    M -->|2| O[addStudent]
    M -->|其他| L
```
