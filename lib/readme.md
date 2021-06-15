# zlib库编译指南

zlib版本：1.2.11

代码路径：\contrib\vstudio\vc14

以下为VS2019编译配置：

1. 常规-目标文件名 改为 zlib
2. C/C++-预处理器-预处理器定义 去掉 ASM相关定义（若不修改，会影响/SAFESEH选项）
3. C/C++-代码生成-运行库 改为 /MD
4. C/C++-输出文件-程序数据库文件名 改为 $(OutDir)zlib.pdb
5. 文档管理程序-输出文件 改为 $(OutDir)zlib.lib
