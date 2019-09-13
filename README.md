# LTEMonitor
## Multiplatform LTE Router status monitor

**LTEMonitor** collects data from your Huawei router or modem.
Technically LTEMonitor is a self contained Web Server and Angular Client
Backend uses https://gitlab.com/eidheim/Simple-Web-Server 
Frontend is a simple Angular project available here https://github.com/kruzer/lmgui
works on platforms:

* Linux
* MacOS
* Windows

## Build

* Clone the project with submodules:
```sh
git clone --recursive https://github.com/kruzer/ltemonitor
```
* Build Angular frontend:
```sh
cd LTEMonitor/lmgui
npm install
ng build --prod
```
* Build backend:
```sh
cd ..
mkdir build
cd build
cmake ..
make
```

## Download
Standalone executables for Linux, macOS and Windows are provided in the Releases section https://github.com/kruzer/LTEMonitor/releases. Download an archive for your operating system and run the executable.

## Usage
The LTEMonitor accepts command line parameters, however it can be configured via web interface. Simply start with router ip address and credentials:
```sh
LTEMonitor -s 192.168.8.1 -u admin -p admin
```
and open your browser at http://localhost:8080 (the default local port is 8080, but it can be changed in command line)

### JSON API
You can comunicate with the router using LTEMonitor as an authorised proxy, all API calls ('/api/...') are encapsulated in credentials and forwarded to it.
```sh
user@host:~$ curl localhost:8080/api/monitoring/traffic-statistics
{
    "response": {
        "CurrentConnectTime": "230018",
        "CurrentUpload": "1693966933",
        "CurrentDownload": "23237432602",
        "CurrentDownloadRate": "507",
        "CurrentUploadRate": "232",
        "TotalUpload": "4635469521",
        "TotalDownload": "53432878388",
        "TotalConnectTime": "604240",
        "showtraffic": "1"
    }
}
```

![Screeenshot1](img/scr1.png?raw=true "Screenshot 1")
![Screeenshot2](img/scr2.png?raw=true "Screenshot 2")

