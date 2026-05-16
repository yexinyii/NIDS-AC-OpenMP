NIDS\-AC\-OpenMP 最终仓库文件结构（适配CIC\-IDS2017）

# 一、最终固定目录结构

```plain
NIDS-AC-OpenMP/
├── .gitignore                 # Git忽略编译文件、缓存、临时数据
├── CMakeLists.txt             # 编译配置（内置OpenMP+Eigen+Qt依赖预留）
├── README.md                  # 项目说明（突出并行AC+系统实现+可视化预留）
├── third_party/               # 第三方库（纯净不用安装）
│   └── Eigen/                 # Eigen轻量辅助计算（极简使用）
├── src/                       # 核心源代码（系统+算法为绝对重心）
│   ├── ac_automaton.cpp/h     # 串行AC自动机基础实现
│   ├── parallel_ac.cpp/h      # 【核心重点】OpenMP并行AC优化
│   ├── system_core.cpp/h      # 【新增】系统业务核心调度模块
│   ├── eigen_simple.cpp/h     # Eigen极简辅助计算（弱化数据分析）
│   ├── file_io.cpp/h          # 轻量化文件读写、规则/数据加载
│   ├── tools.cpp/h            # 计时工具、基础数据统计
│   └── main.cpp               # 控制台程序入口
├── ui_qt/                     # 【新增预留】Qt可视化界面模块（后期开发）
│   ├── qt_mainwindow.ui
│   ├── qt_ui.cpp/h
│   └── ui_resource/           # 界面图标、样式资源
├── rules/                     # 攻击特征规则库
│   └── attack_rules.txt       # 检测特征词库
├── dataset/                   # 测试数据集（纯系统测试载体）
│   ├── pcap_raw/              # 少量CIC-IDS2017原始pcap
│   ├── http_extracted/        # 预处理完成HTTP文本
│   └── test_sample.txt        # 简易测试样本

```
