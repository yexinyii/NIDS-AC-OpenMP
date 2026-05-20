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
# Eigen3 安装配置

本项目依赖 **Eigen 3.4.0** 线性代数库，各系统安装方式如下：

| 系统环境 | 安装方法 |
| :--- | :--- |
| **Windows (VS/MinGW)** | 1. 下载：[eigen-3.4.0.zip](https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.zip)<br>2. 解压重命名为 `Eigen`，放入项目 `third_party/` 目录下 |
| **Linux (Ubuntu/Debian)** | `sudo apt install libeigen3-dev` |
| **macOS (Homebrew)** | `brew install eigen` |

> **路径要求**：本地安装版可自动识别，手动版需保证路径为 `项目/third_party/Eigen/`
plaintext
项目根目录/third_party/Eigen/
