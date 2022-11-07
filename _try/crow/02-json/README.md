# crawJson.cpp

```
ccc@DESKTOP-COUK0VS:/mnt/d/ccc/jsondb/_try/crow/02-json$ g++ crowJson.cpp -lpthread
ccc@DESKTOP-COUK0VS:/mnt/d/ccc/jsondb/_try/crow/02-json$ ./a.out
(2022-11-07 00:14:00) [INFO    ] Crow/1.0 server is running at http://0.0.0.0:18080 using 4 threads
(2022-11-07 00:14:00) [INFO    ] Call `app.loglevel(crow::LogLevel::Warning)` to hide Info level logs.
(2022-11-07 00:15:10) [INFO    ] Request: 127.0.0.1:48316 0x7fd19c000c10 HTTP/1.1 GET /json
(2022-11-07 00:15:10) [INFO    ] Response: 0x7fd19c000c10 /json 200 0
```

然後看 

http://127.0.0.1:18080/json

會有下列回應

```
{"amessage":"Hello, World2!","zmessage":"Hello, World!"}
```

