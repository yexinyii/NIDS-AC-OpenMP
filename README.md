NIDS\-AC\-OpenMP 仓库文件结构（使用CIC\-IDS2017数据集）

# 一、最终固定目录结构

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
