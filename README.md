NIDS\-AC\-OpenMP 仓库文件结构（使用CIC\-IDS2017数据集）

# 最终固定目录结构

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
├── dataset/                   
│   ├── pcap_raw/              
│   ├── http_extracted/        
│   └── test_sample.txt        

```
# NIDS-AC-OpenMP
基于AC自动机 + OpenMP + Eigen的并行网络入侵检测系统

## 依赖环境
1. C++17 及以上
2. CMake 3.10+
3. OpenMP（一般编译器自带）
4. Eigen 3.4.0（线性代数加速库）

## 第三方库说明
本项目依赖 Eigen 3 库，请自行下载并放置到指定目录：

1. 下载 Eigen 3.4.0：
   https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.zip

2. 解压后将文件夹重命名为 Eigen，并放入项目的 third_party 目录下，结构如下：
   NIDS-AC-OpenMP/third_party/Eigen/
