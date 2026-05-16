NIDS-AC-OpenMP最终仓库⽂件结构（适配
CIC-IDS2017）
⼀、最终固定⽬录结构（直接照这个建，不要乱改）
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
代码块
NIDS-AC-OpenMP/ 
├── .gitignore                 
├── CMakeLists.txt             
├── README.md                  
├── third_party/               
│   └── Eigen/                 
├── src/                       
│   ├── ac_automaton.cpp/h     
│   ├── parallel_ac.cpp/h      
│   ├── eigen_math.cpp/h       
│   ├── file_io.cpp/h          
│   ├── tools.cpp/h            
│   └── main.cpp               
├── rules/                     
│   └── attack_rules.txt       
├── dataset/                   
│   ├── pcap_raw/              
│   ├── http_extracted/        
│   └── benchmark_large.txt    
├── performance_data/          
│   ├── speedup_log.txt        
│   └── time_matrix.csv        
├── test/                      
# Git
忽略编译⽂件、缓存、临时数据
 
# 
编译配置（内置
OpenMP+Eigen
依赖）
 
# 
项⽬说明（突出并⾏算法
+
矩阵统计）
 
# 
【新增】第三⽅库（纯净不⽤安装）
 
# Eigen
线性代数库（头⽂件模式，零配置）
 
# 
核⼼源代码（算法绝对重⼼）
 
# 
串⾏
AC
⾃动机基础实现
 
# 
【核⼼重点】
OpenMP
并⾏
AC
优化
 
# 
【新增】
Eigen
性能矩阵计算
 
# 
⾼精度计时、加速⽐计算
 
# 
轻量化⽂件读取（不深究数据处理）
 
# 
程序⼊⼝、串⾏
/
并⾏对⽐测试
 
# 
攻击特征规则库
 
# 
检测特征词库
 
# 
测试数据集（仅算法压测载体）
 
# 
少量
CIC-IDS2017
原始
pcap
（保留专业性）
 
# 
预处理完成
HTTP
⽂本（直接使⽤）
 
# 
超⼤批量压测数据集
 
# 
【新增】算法性能统计⽂件夹
 
# 
加速⽐原始⽇志
 
# Eigen
⽣成耗时矩阵数据表
 
# 
算法专项测试
 
│   ├── serial_test.cpp        
│   └── parallel_test.cpp      
├── log/                       
├── build/                     
└── demo_img/                  
# 
串⾏
AC
性能测试
 
# 
并⾏
AC
性能测试
 
# 
检测告警⽇志
 
# 
编译缓存（
git
忽略，不上传）
 
# 
性能折线图、矩阵分析截图
 

