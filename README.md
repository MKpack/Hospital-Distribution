# Hospital-Distribution

基于 Qt + 百度地图 WebGL 的医院分布可视化与泰森多边形（Voronoi）分析工具。

搜索指定区域的医院并在地图上标注，通过 Voronoi 图算法计算各医院的服务辐射范围，辅助医疗资源规划与分析。

## 功能

- **医院搜索** — 输入城市或区域名称（如「北京市海淀区」），调用百度地图 POI 搜索，在地图上标注所有医院
- **信息查看** — 点击医院标注点弹出信息窗口，显示医院名称与地址
- **Voronoi 分析** — 基于当前搜索结果生成泰森多边形（Voronoi Diagram），直观展示各医院的服务辐射区域
- **地图交互** — 支持鼠标滚轮缩放、拖拽平移

## 技术栈

| 层      | 技术                                                   |
| ------- | ------------------------------------------------------ |
| 桌面端  | C++17, Qt6 / Qt5 (Widgets, WebEngineWidgets, WebChannel) |
| 地图    | 百度地图 WebGL API（BMapGL）                           |
| 几何计算 | 前端 JavaScript 增量裁剪算法（半平面相交法）            |
| 构建    | CMake 3.16+                                            |

## 环境要求

- **Qt** 6.x（推荐）或 Qt 5.x，需包含以下组件：
  - `Widgets`
  - `WebEngineWidgets`
  - `WebChannel`
  - `LinguistTools`
- **CMake** 3.16+
- **编译器** — 支持 C++17 的编译器：MSVC 2019+ / GCC 8+ / Clang 10+
- **网络** — 运行时需联网加载百度地图

## 构建与运行

### 1. 安装 Qt

**Windows:** 使用 [Qt Online Installer](https://www.qt.io/download) 安装，勾选 Qt WebEngine 组件。

**Linux (Ubuntu/Debian):**
```bash
sudo apt install qt6-base-dev qt6-webengine-dev qt6-tools-dev qt6-tools-dev-tools libqt6webchannel6-dev
```

**macOS:**
```bash
brew install qt
```

### 2. 构建项目

```bash
git clone https://github.com/your-username/Hospital-Distribution.git
cd Hospital-Distribution
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt
cmake --build .
```

> 如果 Qt 已加入系统 PATH，可省略 `-DCMAKE_PREFIX_PATH`。  
> Windows 下可使用 Qt 官方提供的 "Qt x.x.x (MSVC 20xx 64-bit)" 命令行提示符。

### 3. 运行

```bash
./hospital_distribution
```

## 使用指南

1. **启动程序** — 主窗口内嵌百度地图，默认居中于北京
2. **搜索医院** — 在底部输入框输入区域名称（如 `北京市海淀区`、`上海市浦东新区`），点击「搜索医院」或按 Enter
   - 地图自动缩放至该区域
   - 所有医院以标记点 + 名称标签形式展示
   - 点击标记点弹出信息窗口显示详情
3. **Voronoi 分析** — 点击「Voronoi 分析」按钮
   - 每个医院站点生成一个彩色多边形
   - 多边形内任意点到对应医院的距离最近
   - 多边形的边为相邻医院连线的中垂线
4. **重新搜索** — 再次搜索会自动清除上次结果

## 项目结构

```
Hospital-Distribution/
├── CMakeLists.txt                   # CMake 构建配置
├── main.cpp                         # 应用入口，初始化翻译与主窗口
├── mainwindow.h / mainwindow.cpp    # 主窗口，连接 UI 信号与地图操作
├── mainwindow.ui                    # Qt Designer UI 布局文件
├── mapcontroller.h / mapcontroller.cpp  # 地图控制器，桥接 C++ 与 JavaScript
├── index.html                       # 前端页面 — 百度地图初始化、POI 搜索、Voronoi 算法
├── res.qrc                          # Qt 资源文件，注册 index.html
├── hospital_distribution_zh_CN.ts   # 简体中文翻译文件
└── README.md
```

## Voronoi 算法说明

前端实现采用**增量裁剪法**（Incremental Clipping Algorithm）：

1. 以地图当前视口外扩后的矩形作为初始多边形
2. 对每个站点，用其与其他所有站点的中垂线依次裁剪多边形
3. 保留靠近当前站点的半平面一侧
4. 最终得到每个站点的 Voronoi 单元格

适用于中小规模站点（数十个以内）。对于大规模点集，建议改用 Fortune 算法（扫描线法）。

## 百度地图 API Key

`index.html` 中使用的 API Key 为示例密钥：

```javascript
src="https://api.map.baidu.com/api?v=1.0&type=webgl&ak="
```

如遇到配额超限或权限问题，请自行申请密钥：

1. 访问 [百度地图开放平台](https://lbsyun.baidu.com/)
2. 注册账号并创建应用（选择「浏览器端」）
3. 将 `index.html` 中的 `ak` 参数替换为你的密钥

## 常见问题

**Q: 编译时报错找不到 Qt WebEngine？**  
A: 确保安装了 `qt6-webengine-dev`（Linux）或在 Qt Installer 中勾选了 WebEngine 组件。

**Q: 运行时地图区域空白？**  
A: 检查网络连接；百度地图需要在线加载。也可能是 API Key 过期或受限，请参照上文替换密钥。

**Q: Voronoi 分析按钮点了没反应？**  
A: 需要先搜索医院，确保地图上至少有两家医院标注点。


