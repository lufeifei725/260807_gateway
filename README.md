## 简介
基于 Linux C 实现的嵌入式智能边缘网关。
实现守护进程后台运行、信号处理、子进程回收、MQTT消息收发、系统日志、OTA升级基础框架。
适合学习 Linux 进程、信号、多进程、MQTT、Makefile工程，作为嵌入式Linux应用开发实战项目。

## 目录结构
 
 
gateway
├── app                 # 业务层：数据采集、业务逻辑
├── daemon              # 守护进程模块，实现程序后台化
├── init                # 初始化模块：配置解析、资源初始化
├── ota                 # OTA远程固件升级模块
├── test                # 单元测试、功能测试代码
├── Makefile            # 多目录编译构建脚本
├── .gitignore          # Git忽略规则：编译中间文件、IDE配置
└── README.md           # 项目说明
 
plaintext
  

## 环境依赖
- OS：Ubuntu 20.04+ / Embedded Linux
- Compiler：gcc / make
- MQTT库：libmosquitto‑dev
- Broker：mosquitto

### 安装依赖
```bash
sudo apt update
sudo apt install gcc make
sudo apt install mosquitto mosquitto-clients libmosquitto-dev
 
 
主要功能
 
- ✅ Daemon守护进程：程序后台运行，脱离控制终端
- ✅ 信号优雅退出：捕获  SIGINT  /  SIGTERM ，释放资源安全退出
- ✅ SIGCHLD 子进程回收： waitpid+WNOHANG 循环回收，杜绝僵尸进程
- ✅ MQTT客户端：连接Broker，上报设备数据，订阅接收云端下发指令，支持遗嘱消息
- ✅ syslog系统日志：守护进程使用syslog记录日志，不依赖终端printf
- ✅ 多进程业务模型：子进程负责采集，主进程负责网络上报
- ✅ OTA固件升级框架：版本校验、远程更新基础框架
- ✅ Makefile多目录工程管理
 
编译与运行
 
编译项目
 
bash
  
make
 
 
运行程序
 
bash
  
# 前台模式，用于调试
./gateway

# 守护进程后台模式
./gateway -d
 
 
优雅停止程序
 
bash
  
ps -ef | grep gateway
kill -SIGTERM <pid>
 
 
MQTT本地测试
 
bash
  
# 订阅网关上报主题
mosquitto_sub -t gateway/data/upload -h 127.0.0.1

# 模拟云端下发控制命令
mosquitto_pub -t gateway/cmd/down -m '{"action":"reboot"}' -h 127.0.0.1
 
 
查看系统运行日志
 
bash
  
tail -f /var/log/syslog
 
 
面试核心技术点
 
1. Linux守护进程标准实现，会话、工作目录、文件描述符重定向
2. 信号处理：全局标记变量优雅退出，SIGCHLD+waitpid非阻塞循环回收子进程
3. MQTT发布‑订阅模型，QoS、遗嘱消息使用场景
4. syslog日志体系，守护进程不能依赖标准输出
5. Makefile多目录项目编译
6. Git工程管理， .gitignore 过滤编译产物
 
注意事项
 
1. 启动mosquitto服务： sudo systemctl start mosquitto 
2. 部分系统操作需要root权限，可使用 sudo 运行程序
3. 磁盘占满会造成日志写入失败、进程异常，注意监控磁盘空间
 
License
 
For study only，仅供个人学习，禁止商用。
 
plaintext
  

操作步骤：
1. VSCode新建文件，命名 `README.md`
2. 粘贴全部上面文本，保存
3. git add README.md && git commit -m "add readme"
4. push到GitHub，仓库首页自动渲染。