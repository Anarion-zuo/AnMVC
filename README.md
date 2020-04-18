# AnMVC

*AnMVC* is a naive MVC framework in C++. Like other MVC framework, it helps create a website in C++ with as fewer extra code as possible. I started this project for the purpose of demos and practices. Now that I have implemented some of the basic utility of an MVC framework, I am publishing it with my explanations to its details.

## Hello World

### Environment

I am working on Ubuntu Linux `gcc`, therefore I recommend you to do the same. Do not presume to use Windows, for it is 100% incompatible with Linux system call. If you are using other versions of Linux, such as Centos, there may be some tiny issues when compiling. You must alter some of the code, adding some header files, to make it work. Clang is not recommended. You must also be ware of the `gcc` under MacOSX, for it is actually `clang` by default.

### Build Tool

I am using CMake as build tool, while you may prefer *make*. They are quite alike! To build the project, you must first install it.

```sh
sudo apt install cmake    # Ubuntu
sudo yum install cmake    # Centos
```

### Dependencies

This project makes use of the `zlib` library for compressing files. Please install `zlib` on your machine before building the project.

```shell
sudo apt install zlib1g-dev
sudo yum install zlib
```

The project is not built with the `C++ std`, but with another project of my own [AnBase](https://github.com/Anarion-zuo/AnBase), providing basic components. There are some links under the directory `library` and `include`. The symbolic links under `include` are from my other repository [AnBase](https://github.com/Anarion-zuo/AnBase). To include them here, you must do the following things.

1. Clone that repository.
2. Copy or link all directories under its `include`.

All directories under the `include` directory should be duplicated(copied or linked) to the current [include](../../include/). The directory structure should look like the following.

`include`:

```shell
include
├── allocator
├── concurrent
├── container
├── context
├── exceptions
├── feature
├── http
├── io
├── listener
├── Object
├── parser
├── reflection
└── resource

```

`lib`:

```shell
lib
└── libanarionbase.a 
```

All is ready then.

### Compile

The files are ready. Use CMake to build the project. If you are new to CMake, please follow my steps.

First, set up a directory for the compile results.

```shell
mkdir build
```

Go into that directory.

```shell
cd build
```

Run CMake to generate MakeFiles and more.

```shell
cmake ..
```

Run make to generate the executable file.

```shell
make
```

You may have to do some tiny alternations due to different version of Linux. If you are using Ubuntu, there should be no problem.

If everything is fine, you may find an executable file `MyMVC` under the `build` directory.

### Run You Server

Run the executable file, setting up an HTTP server on port 9898. You may request the page with *Postman* or some Web Browser. The page is a modest one, holding few code for testing only. The `html` file is `/static/html/hello.html`. You may edit it to get a different result. 

It should show this as default:

```html
<h1 style="color: blue">I am your FATHER</h1>
```

<h1 style="color: blue">I am your FATHER</h1>

