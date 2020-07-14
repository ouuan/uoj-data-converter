# UOJ Data Converter

将各种各样格式的数据转换为 [UOJ](https://github.com/UniversalOJ/UOJ-System/) 的格式。

[![Build Status](https://github.com/ouuan/uoj-data-converter/workflows/Build/badge.svg?branch=master)](https://github.com/ouuan/uoj-data-converter/actions?query=workflow%3ABuild+branch%3Amaster)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/069303dace1d445487962249b7b96407)](https://www.codacy.com/manual/ouuan/uoj-data-converter)

## Features

-   将各种格式的文件名转换成 UOJ 格式（`<题目名><编号>.in/ans`）。
-   设置并检查子任务的组成、分数和依赖。
-   从文件或输入添加样例。
-   自动生成 `problem.conf`。
-   将 std 复制到 `std.cpp`，可选地删除 `freopen`，复制到剪贴板。
-   跨平台。

## 下载安装

1.  在 [Releases](https://github.com/ouuan/uoj-data-converter/releases) 中下载。
2.  Windows 下安装后使用，Linux 下直接运行（可能需要 `chmod +x uoj-data-converter*.AppImage`）。

暂不提供 macOS 的可执行文件，需要的话可以自行构建。

## 构建

1.  安装 [Qt](https://www.qt.io/download) 和 [CMake](https://cmake.org/download/)。

2.  clone：`git clone https://github.com/ouuan/uoj-data-converter`

3.  构建：

    ```sh
    mkdir build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    cmake --build .
    ```
