# crow:hello

## Run

```
ccc@DESKTOP-COUK0VS:/mnt/d/ccc/jsondb/crow$ g++ crow1.cpp -lpthread
ccc@DESKTOP-COUK0VS:/mnt/d/ccc/jsondb/crow$ ./a.out
(2022-11-06 23:13:31) [INFO    ] Crow/1.0 server is running at http://0.0.0.0:18080 using 4 threads
(2022-11-06 23:13:31) [INFO    ] Call `app.loglevel(crow::LogLevel::Warning)` to hide Info level logs.
```


於是就成功了，看 http://127.0.0.1:18080/ 會有 hello