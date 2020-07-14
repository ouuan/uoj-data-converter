# UOJ Data Converter

将各种各样格式的数据转换为 [UOJ](https://github.com/UniversalOJ/UOJ-System/) 的格式。

## Features

-   将各种格式的文件名转换成 UOJ 格式（`<题目名><编号>.in/ans`）。
-   设置并检查子任务的组成、分数和依赖。
-   从文件或输入添加样例。
-   自动生成 problem.conf。
-   跨平台。

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
