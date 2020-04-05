# LTEMonitor
## Multiplatform LTE Router status monitor
## An example of C++ & Angular Desktop Application
![Screeenshot1](img/scr1.png?raw=true "Screenshot 1")
**LTEMonitor** presents data from your Huawei router or modem.
Technically LTEMonitor is a self contained Web Server (uses https://gitlab.com/eidheim/Simple-Web-Server as a backend) and Angular Client (uses https://github.com/kruzer/lmgui ).

## Features

- Cross-platform (Linux, OSX, Windows),
- Single binary without any runtime dependencies,
- Fast startup and low memory footprint (due to native code and static binaries),
- Server/client architecture,
- Configuration with command line parameters,
- API Console - to test your router/modem features

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

There is complete list of current command line parameters:
```sh
./LTEMonitor --help

LTE Monitor version 0.16
Options:
  -h [ --help ]                      Display this help message describing all 
                                     options
  -b [ --block ]                     Prevent auto starting browser at 
                                     http://locahost:[port]
  -v [ --version ]                   Display the version number
  -s [ --server ] arg (=192.168.0.1) IP or router's domain name
  -u [ --user ] arg (=admin)         router's username
  -p [ --password ] arg (=admin)     password
  -o [ --port ] arg (=8080)          local server port number

```

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

![Screeenshot2](img/scr2.png?raw=true "Screenshot 2")

