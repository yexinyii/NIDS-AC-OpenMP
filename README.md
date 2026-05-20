NIDS\-AC\-OpenMP 

# 目录结构

```plain
NIDS-AC-OpenMP/
├── .gitignore                 
├── CMakeLists.txt            
├── README.md                  
├── third_party/               
│   └── Eigen/                 
├── src/                       
│   ├── ac_automaton.cpp/h     
│   ├── parallel_ac.cpp/h      
│   ├── system_core.cpp/h      
│   ├── eigen_simple.cpp/h     
│   ├── file_io.cpp/h          
│   ├── tools.cpp/h            
│   └── main.cpp               
├── ui_qt/                     
│   ├── qt_mainwindow.ui
│   ├── qt_ui.cpp/h
│   └── ui_resource/          
├── rules/                     
│   └── attack_rules.txt       
├── dataset/ #使用CIC\-IDS2017数据集作为测试样例
│   ├── pcap_raw/              
│   ├── http_extracted/        
│   └── test_sample.txt        

```
# NIDS-AC-OpenMP
基于AC自动机 + OpenMP + Eigen的并行网络入侵检测系统

## 依赖环境
1. 支持C++17的编译器
2. CMake 3.10+
3. OpenMP
4. Eigen 3.4.0

## Eigen3库安装方式
Windows：
1. 下载 Eigen 3.4.0：
   https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.zip

2. 解压后将文件夹重命名为 Eigen，并放入项目的 third_party 目录下，结构如下：
   NIDS-AC-OpenMP/third_party/Eigen/

类Unix：
包管理器安装libeigen3-dev库
