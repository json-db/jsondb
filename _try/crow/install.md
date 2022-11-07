# crow

* https://github.com/CrowCpp/Crow
    * https://crowcpp.org/master/

## Install

* https://crowcpp.org/master/

在 wsl 中安裝成功

1. 直接下載 .deb 檔之後用 sudo apt install ./crow-v1.0+5.deb
2. 再安裝 boost 

```
$  sudo apt-get update  # 必須執行這行更新套件清單，否則會找不到 libboost-all-dev
$  sudo apt-get install libboost-all-dev
```

## Run

接著用

```
ccc@DESKTOP-COUK0VS:/mnt/d/ccc/jsondb/crow$ g++ crow1.cpp -lpthread
ccc@DESKTOP-COUK0VS:/mnt/d/ccc/jsondb/crow$ ./a.out
(2022-11-06 23:13:31) [INFO    ] Crow/1.0 server is running at http://0.0.0.0:18080 using 4 threads
(2022-11-06 23:13:31) [INFO    ] Call `app.loglevel(crow::LogLevel::Warning)` to hide Info level logs.
```

於是就成功了，看 http://127.0.0.1:18080/ 會有 hello

