# TINYHTTP

TINYHTTP is a very lightweight http library for basic operations such as GET, POST, HEAD, PUT and DELETE. 
We have a lot of HTTP servers out there for eg. Apache, Microsoft IIS, etc. But they are very time and CPU intensive. 
Sometimes, when the device has less onboard memory or less processing power like a remote sensor where I  need to use 
only basic HTTP requests. Such systems can be used for basic IOT devices or for disaster management sensors 
which can use my “tiny” HTTP communication system.

Server
Created using C++ and sockets. For handling multiple clients concurrently, I implement multi-threaded apartment model. It is 
complete synchrounous server

Client
There are 3 clients.
1. AngularJs
2. Client using C++
3. Client using WPF
