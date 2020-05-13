# Build a Web Application

This section shows you how to build an application with `AnMVC` without understanding the details of its implementation.

## Derive from `HttpApplet`

An `HttpApplet` is a building-block of a web application. You must begin with implementing this interface. Build a class derived from `HttpApplet`:

```cpp
class HelloApplet : public virtual HttpApplet {
	// ...
};
```

## Request Pattern

An `applet` must be appointed a way of access, a request pattern, which maps and distributes requests to many applets. Edit `HttpDispatcher::registerApplets()` in file `/src/http/HttpDispatcher.cpp` to specify a pattern for your applet.

```cpp
void HttpDispatcher::registerApplets() {
    /*
    A request with directory "/"
    should be handled by an pInstance of HelloApplet
    */
    requestMap.put(SString("/"), new HelloApplet());
}
```

## Set up Behavior

The interface `HttpApplet` has many methods corresponding to the different request methods (GET, POST, etc). Override one of them to specify `applet` behaviour under certain request method. The applet does nothing if the correpsonding method is not overridden.

```cpp
void onGet() override;  // specify behavior for GET requests
// does nothing for POST requests and others
```

## Resource Directory

The static resource directory contains static files such as `html`, `css`, `js`, and images. The directory should be specified by static variable `HttpApplet::staticResources` of `/src/http/HttApplet.cpp`. The default directory is my working directory, therefore definitely not valid for you. It is recommended to use an absolute directory.

```cpp
anarion::StaticResources anarion::HttpApplet::staticResources(SString("/path/to/static"));
```

It is also recommended to have subdirectories `html`, `css`, `js`, and `img`, or some of these, under your static directory, for the convenience of finding files. Files should be put into these subdirectories according to their suffix. I used the following directory structure when writing and testing this project.

```shell
static
└── html
    └── hello.html
```



## Return a Page

To have the applet return a page to the browser, you must explicitly ask for the page resource in the `onXxx()` method. 

```cpp
void anarion::HelloApplet::onGet() {
    // request for resource by name
    Payload *payload = staticResources.getPayload(SString("hello.html"));
    // load resource into the response
    response->setPayload(payload);
}
```

The string passed to the function `staticResources.getPayload` is a relative directory of the file `hello.html`. With the valid suffix `html`, the program goes to the subdirectory `html` of the static directory to find the file with that name. Once the file is found, associate it with the response by function `response->setPayload`.

## Try Your Server

The server should be ready by now. Run it and try it with a browser or *Postman*. If you have any problem, feel free to raise an issue.

