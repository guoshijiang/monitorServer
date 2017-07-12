# monitorServer 高并发中转服务器项目

### 一、项目简介
#### 1.项目架构
图片1： 
    ![图片1](https://github.com/guoshijiang/monitorServer/blob/master/tmp/QQ%E6%88%AA%E5%9B%BE20170712163055.p  "图片1")
#### 2.项目编译
执行以下命令即可编译项目
gcc -o minitorServer minitorServer.c minitorUlaneServerOp.c threadPool.c ulaneLog.c config.c cJson.c parseJson.c createJson.c ulaneJsonLog.c jsonCheck.c queueMsg.c ./libulanemysql.so ./libulanesocket.so -I ../inc/ -I/usr/include/mysql/ -L/usr/lib64/mysql/ -lmysqlclient -lstdc++ -lpthread -ldl -lrt -lm -luuid

#### 注意：
mysql数据库操作组件和Socket连接池可以编译成为动态库，utils目录下有makefile，生成动态库之后把动态库移动到主项目的src目录下。

#### 3.关于项目测试
在项目中有一个board目录，里面是测试程序和测试json串
