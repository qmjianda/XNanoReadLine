# XNanoReadLine

#### 介绍
XNanoReadLine是一个嵌入式行编辑库，类似一个简易的GNU readline，可以实现以下功能。基于XNanoReadLine可以快速实现一个基于串口、TCP等其他字符流接口的终端软件。
1. 光标移动
2. 行的字符增删
3. 快捷键检测
4. 历史记录
5. 环境变量存储
6. 行字符串变量替换，实现${var1} $var2
7. 行字符串分割计算，实现"max 1 2"，输出parma[0]="max", parma[1]="1", parma[2]="2"

#### 编译
```shell
make
```
#### 使用
见example.c

#### 文档
详细文档见doc目录

