# 04-templates2

```
ccc@DESKTOP-COUK0VS:/mnt/d/ccc/jsondb/_try/crow/04-templates2$ g++ main.cpp -lpthread
ccc@DESKTOP-COUK0VS:/mnt/d/ccc/jsondb/_try/crow/04-templates2$ ./a.out
(2022-11-07 00:39:53) [INFO    ] Crow/1.0 server is running at http://0.0.0.0:18080 using 4 threads
(2022-11-07 00:39:53) [INFO    ] Call `app.loglevel(crow::LogLevel::Warning)` to hide Info level logs.  
(2022-11-07 00:40:24) [INFO    ] Request: 127.0.0.1:48330 0x7f36cc000c10 HTTP/1.1 GET /ccc
(2022-11-07 00:40:25) [INFO    ] Response: 0x7f36cc000c10 /ccc 200 0
(2022-11-07 00:40:33) [INFO    ] Request: 127.0.0.1:48334 0x7f36cc0038a0 HTTP/1.1 GET /snoopy
(2022-11-07 00:40:33) [INFO    ] Response: 0x7f36cc0038a0 /snoopy 200 0
```

然後在 

http://127.0.0.1:18080/snoopy

會看到

Hello snoopy!

在 

http://127.0.0.1:18080/ccc

會看到

Hello ccc!

