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
本项目依赖 Eigen 3.4.0 线性代数库，不同环境安装配置如下：
🪟 Windows (Visual Studio)
下载 Eigen 3.4.0：
https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.zip
解压并将文件夹重命名为 Eigen
放置到项目 third_party/ 目录下
🪟 Windows (MinGW)
下载 Eigen 3.4.0 并解压
重命名为 Eigen，放入 third_party/
CMake 构建时指定 MinGW 编译器即可自动识别
🐧 Linux / 🧑‍💻 macOS
bash
运行
# Ubuntu / Debian
sudo apt install libeigen3-dev

# Fedora
sudo dnf install eigen3-devel

# macOS (Homebrew)
brew install eigen
📁 库路径要求
Eigen 必须放置在以下路径，项目才能正常编译：
plaintext
项目根目录/third_party/Eigen/
