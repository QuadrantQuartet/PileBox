# Qt-Template

克隆此项目：
```shell
git clone git@github.com:QuadrantQuartet/Qt-Template.git
```

或者已经克隆过，改为从远程库拉取：

```shell
git pull
```

打开一个命令行窗口，将 Qt 的 MinGW 添加到环境变量中：

```shell
# 在 cmd 中
set PATH=%PATH%;D:\Qt\Tools\mingw1120_64\bin

# 在 Powershell 中
$env:PATH += ";D:\Qt\Tools\mingw1120_64\bin"
```

注意修改地址为你的 Qt 安装位置。

如果之前环境变量中有 MinGW，请提前删除防止冲突。

然后运行在这个命令行窗口里输入 `./setup_vcpkg.bat` 运行，配置 vcpkg 环境。
注意，此过程会从 Github 下载文件，推荐打开 Clash for Windows 的 TUN 模式，接管全局流量，以防止网络问题。
