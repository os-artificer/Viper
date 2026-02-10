# Viper 依赖管理系统

本项目使用 CMake 的 ExternalProject 模块实现自动依赖管理，支持自动检测和安装缺失的依赖库。

## 特性

- ✅ 自动检测系统中已安装的依赖库
- ✅ 仅在依赖缺失时才触发下载和编译
- ✅ 统一使用静态链接
- ✅ 所有依赖安装到 `/usr/local` 下的独立目录
- ✅ 支持并行编译加速构建
- ✅ 可选择性安装特定依赖

## 依赖库列表

| 库名 | 版本 | 类型 | 用途 |
|------|------|------|------|
| OpenSSL | 3.2.1 | 静态库 | SSL/TLS 加密 |
| Boost | 1.87.0 | 静态库 | C++ 工具库 |
| libevent | 2.1.12 | 静态库 | 事件驱动网络库 |
| CURL | 8.11.1 | 静态库 | HTTP 客户端 |
| yaml-cpp | 0.8.0 | 静态库 | YAML 解析 |
| spdlog | 1.13.0 | 静态库 | 日志库 |
| RapidJSON | 1.1.0 | 头文件 | JSON 解析 |
| concurrentqueue | 1.0.4 | 头文件 | 并发队列 |
| Abseil | 20250127.1 | 静态库 | C++ 基础库 |
| Protobuf | 30.2 | 静态库 | 序列化协议 |
| gRPC | 1.71.0 | 静态库 | RPC 框架 |
| etcd-cpp-apiv3 | latest | 静态库 | etcd 客户端 |

## Controller 静态链接

当检测到以下静态库存在时，controller 会通过 `controller_static_deps` 目标进行静态链接，减少运行时对动态库的依赖：

- **已静态链接**：curl（libcurl.a）、libevent（libevent_core/extra/pthreads/openssl.a）、z（libz.a，在 controller_static_deps 中）、zstd（若找到 libzstd.a）、stdc++（-static-libstdc++）、absl（absl_static + gRPC 的 Abseil 覆盖为 .a）
- **仍可能为动态**：protobuf、cpprest、z、zstd、ssl/crypto、absl（若 gRPC 拉入系统 OpenSSL/Abseil）

若系统中有 `libcrypto.a`（如 gRPC 使用系统 OpenSSL），链接阶段会需要 libz.so.1 在命令行上，此时 z 会保留为动态库。为尽量全静态，建议：

- 使用本项目的 OpenSSL 静态库（`OPENSSL_PREFIX`），并让 gRPC 通过 `OPENSSL_ROOT_DIR` 使用同一套 OpenSSL
- 安装 protobuf/cpprest/zstd 的静态版（如 `-DBUILD_SHARED_LIBS=OFF` 编译安装），以便 CMake 找到对应的 .a

## 使用方法

### 1. 标准构建流程

```bash
# 创建构建目录
mkdir build && cd build

# 配置项目（会自动检测依赖）
cmake ..

# 如果有缺失的依赖，先安装它们
cmake --build . --target install_dependencies

# 重新配置（使用新安装的依赖）
cmake ..

# 编译项目
cmake --build . -j$(nproc)
```

### 2. 一键构建脚本

```bash
# 使用项目提供的构建脚本
./build.sh
```

### 3. 自定义选项

```bash
# 禁用自动安装依赖
cmake .. -DAUTO_INSTALL_DEPENDENCIES=OFF

# 只安装特定依赖
cmake .. -DINSTALL_OPENSSL=OFF -DINSTALL_BOOST=OFF

# 指定依赖安装位置
cmake .. -DDEPS_INSTALL_PREFIX=/opt/viper-deps

# 指定并行编译数量
cmake .. -DCMAKE_BUILD_PARALLEL_LEVEL=8
```

## 依赖安装位置

所有依赖库默认安装到 `/usr/local` 下的独立目录：

```
/usr/local/
├── openssl-3.2.1/
│   ├── include/
│   ├── lib64/
│   │   ├── libssl.a
│   │   ├── libcrypto.a
│   │   ├── libssl.so
│   │   └── libcrypto.so
│   └── bin/
├── boost-1.87.0/
│   ├── include/
│   └── lib/
├── libevent-2.1.12/
│   ├── include/
│   └── lib/
└── ...
```

每个库的目录包含：
- `include/` - 头文件
- `lib/` 或 `lib64/` - 静态库（.a）和动态库（.so）
- `bin/` - 可执行文件（如果有）

## 手动安装单个依赖

如果只需要安装某个特定的依赖：

```bash
cd build
cmake --build . --target openssl_external
cmake --build . --target boost_external
cmake --build . --target libevent_external
# ... 其他依赖
```

## 清理依赖

```bash
# 清理构建目录中的依赖下载和编译文件
rm -rf build/external/

# 完全卸载已安装的依赖（谨慎操作）
sudo rm -rf /usr/local/openssl-3.2.1
sudo rm -rf /usr/local/boost-1.87.0
# ... 其他依赖
```

## 故障排除

### 问题：依赖检测失败

**解决方案**：
```bash
# 清理 CMake 缓存
rm -rf build/CMakeCache.txt build/CMakeFiles/

# 重新配置
cmake ..
```

### 问题：编译依赖时内存不足

**解决方案**：
```bash
# 减少并行编译数量
cmake .. -DCMAKE_BUILD_PARALLEL_LEVEL=2
```

### 问题：网络下载失败

**解决方案**：
1. 检查网络连接
2. 使用代理：`export https_proxy=http://proxy:port`
3. 手动下载源码包到 `build/external/` 目录

### 问题：权限不足

**解决方案**：
```bash
# 使用 sudo 安装依赖
sudo cmake --build . --target install_dependencies

# 或更改安装位置到用户目录
cmake .. -DDEPS_INSTALL_PREFIX=$HOME/viper-deps
```

## 高级配置

### 使用系统已有的依赖

如果系统中已经安装了某些依赖，可以通过环境变量指定：

```bash
cmake .. \
  -DOPENSSL_ROOT_DIR=/usr \
  -DBOOST_ROOT=/usr/local/boost \
  -DProtobuf_DIR=/usr/local/lib/cmake/protobuf
```

### 交叉编译

```bash
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain.cmake \
  -DDEPS_INSTALL_PREFIX=/path/to/sysroot/usr/local
```

## 依赖关系图

```
Viper Project
├── OpenSSL (基础)
├── Boost (基础)
├── libevent (基础)
├── CURL ← OpenSSL
├── yaml-cpp (基础)
├── spdlog (基础)
├── RapidJSON (基础)
├── concurrentqueue (基础)
├── Abseil (基础)
├── Protobuf ← Abseil
├── gRPC ← Protobuf, Abseil, OpenSSL
└── etcd-cpp-apiv3 ← gRPC, Protobuf, Abseil, OpenSSL
```

## 贡献

如需添加新的依赖库：

1. 在 `cmake/Dependencies.cmake` 中添加版本和路径定义
2. 创建 `cmake/external/NewLib.cmake` 配置文件
3. 在 `cmake/Dependencies.cmake` 中添加检测和加载逻辑
4. 更新本文档
