# nats


```
PS C:\WINDOWS\system32> choco install nats-server
```

## 過程

```
PS C:\WINDOWS\system32> choco install nats-server
Chocolatey v1.2.0
Installing the following packages:
nats-server
By installing, you accept licenses for the packages.

nats-server v2.0.0.20190610 [Approved]
nats-server package files install completed. Performing other installation steps.
The package nats-server wants to run 'ChocolateyInstall.ps1'.
Note: If you don't run this script, the installation will fail.
Note: To confirm automatically next time, use '-y' or consider:
choco feature enable -n allowGlobalConfirmation
Do you want to run the script?([Y]es/[A]ll - yes to all/[N]o/[P]rint): y

Downloading nats-server 64 bit
  from 'https://github.com/nats-io/nats-server/releases/download/v2.0.0/nats-server-v2.0.0-windows-amd64.zip'
Progress: 100% - Completed download of C:\Users\Hero3C\AppData\Local\Temp\chocolatey\nats-server\2.0.0.20190610\nats-server-v2.0.0-windows-amd64.zip (3.37 MB).
Download of nats-server-v2.0.0-windows-amd64.zip (3.37 MB) completed.
Hashes match.
Extracting C:\Users\Hero3C\AppData\Local\Temp\chocolatey\nats-server\2.0.0.20190610\nats-server-v2.0.0-windows-amd64.zip to C:\ProgramData\chocolatey\lib\nats-server\tools...
C:\ProgramData\chocolatey\lib\nats-server\tools
 ShimGen has successfully created a shim for nats-server.exe
 The install of nats-server was successful.
  Software installed to 'C:\ProgramData\chocolatey\lib\nats-server\tools'

Chocolatey installed 1/1 packages.
 See the log for details (C:\ProgramData\chocolatey\logs\chocolatey.log).
PS C:\WINDOWS\system32> nats-server
[11968] 2022/11/08 08:40:20.788304 [[32mINF[0m] Starting nats-server version 2.0.0
[11968] 2022/11/08 08:40:20.957300 [[32mINF[0m] Git commit [not set]
[11968] 2022/11/08 08:40:20.959301 [[32mINF[0m] Listening for client connections on 0.0.0.0:4222
[11968] 2022/11/08 08:40:20.960302 [[32mINF[0m] Server id is NC36VLSSW7UZDPALOBLASHZKWWKX2IQPYAEB7QD5TFYLFGWT3XJHZ2LA
[11968] 2022/11/08 08:40:20.960302 [[32mINF[0m] Server is ready
```
