# 海德薇

`海德薇`是一个用户用于防止屏幕休眠，保持屏幕常亮的命令行程序。

海德薇很久没出来活动了，你的场地看起来不错，让她纵情飞翔吧！

[English](./README.md) | 中文

## 编译

在开始编译之前，请确保你已经安装了[vcpkg](https://github.com/microsoft/vcpkg)。设置`VCPKG_ROOT`环境变量，将其指向`vcpkg`的安装目录。将`vcpkg`的路径添加到系统的`PATH`环境变量中。

### Linux & macOS

```sh
git clone https://github.com/TaipaXu/hedwig.git && cd hedwig
vcpkg install
cmake --preset=unix-release
cmake --build build/release -j $(nproc)
```

# 许可证

[GPL-3.0](LICENSE)
