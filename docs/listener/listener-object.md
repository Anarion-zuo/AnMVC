# Listener Object



The code of the implementation of Listener is under `/src/listener/`. The directory currently has this structure:

```shell
listener
├── Listener.cpp/.h
├── StreamListener.cpp/.h
└── TcpPortListener.cpp/.h
```

If you look closely, the class `Listener` and `StreamListener` are empty. They are for future extensions, not implemented as yet. All real stuff are in the class `TcpPortListener`.

{:toc}

## Iterface Manual

Before diving into the structure of codes and reasons for the design, I would first give the utility of each interface, how to configure a listener object for the use of a web server.

### Constructor

```cpp
TcpPortListener(in_port_t portNum);
```

The constructor of the class `TcpPortListener`  has 1 argument, specifying the port that the listener should listen to. This specification can be altered with method `bind`.

### Listen

```cpp
void listen();
void launch();
```

This method set off the listening procedure on the current thread. The thread is then fully devoted to listening the predefined TCP port. To create a new thread for the execution of `listen`, call `launch`.

## Class Data Members

Let’s firstly look at the data members and their utility. This code block shows the data members of class `TcpPortListener`.

```cpp
class TcpPortListener {
    ...
protected:
    TcpServerSocketChannel server;   // server socket
        
    // epoll
    int epfd;                        
    epoll_event *events = nullptr;
    LaunchRoutine routine;

    // ready queue
    BlockQueue<TcpSocketChannel*> readQueue;
public:
    ...
};
```

### Member `server`

The `TcpServerSocketChannel` is from [AnBase](https://github.com/Anarion-zuo/AnBase), implementing socket read&write with Linux system call. Here, it serves as a server socket channel for accepting connections from potential clients. The following block shows some interfaces provided by the `TcpServerSocketChannel` class.

```cpp
void bind(in_port_t port_num);         // bind the socket to appointed port
void listen(int backlog);              // begin listening on the predefined port

// write to socket from appointed sources
size_type in(char *p, size_type nbytes);
size_type in(Buffer &buffer);
size_type in(Buffer &buffer, size_type nbytes);

// read from socket to appointed destinations
size_type out(char *p, size_type nbytes);
Buffer out(size_type nbytes);
Buffer out();
```

For more details on `TcpServerSocketChannel`, see my other repository [AnBase](https://github.com/Anarion-zuo/AnBase). For more details on Linux socket programming, see [man socket page](http://man7.org/linux/man-pages/man2/socket.2.html), [man accept page](http://man7.org/linux/man-pages/man2/accept.2.html).

### Epoll Reactor

The members marked `epoll` are for a epoll reactor, as is described later.

### Ready Queue

When a packet is received and extracted from the TCP pipeline, its contents are put onto the ready queue. Other procedures on other threads may poll the ready queue in order to extract one packet from it and do something with it. If the queue is empty, the polling thread would be put to sleep, as the class name suggests.

## Epoll Reactor

`epoll` interface is provided  and implemented by the Linux kernel itself, for the purpose of efficient packet collection. Many of the well-known server program use it. The `epoll` reactor for this project is implemented by the method `listen` of class `TcpPortListener`.

### Bind a Port

The first step of any socket programming on a server after initialization is alway to choose a port to bind. The incoming packets on this port can be collected by the programm. The bind operation is implemented by function `bind` of `TcpPortListener`, or can be passed as parameter to the constructor.

### Initialization

An epoll reactor generally needs 2 components, a file descriptor and an array of event structure. The former is a handle to the system call operations. The latter is for collecting runtime data of `epoll`. These 2 components are the data member of class `TcpPortListener` marked *epoll *. Before launching the main procedure, we must first initialize these fields.

```cpp
// init epoll
epfd = ::epoll_create(backlog);  // allocate a file descriptor
events = static_cast<epoll_event *>( operator new( backlog * sizeof(epoll_event) ) );  // allocate space for events
memset(events, 0, backlog * sizeof(epoll_event));  // zero the array
addEvent(epfd, server.getFd(), EPOLLIN | EPOLLET);  // put server socket onto the epoll tree
```

Later when the programm make use of `epoll`, it must read the array `events`.

Function `addEvent` is to register the server socket onto the  service of`epoll`. If an event of interest accured on the socket, such as in-coming connection, in-coming packet, or error, the information of the event would be put onto the `events` array.

