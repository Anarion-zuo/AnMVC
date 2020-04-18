# AnMVC

*AnMVC* is a naive MVC framework in C++. Like other MVC framework, it helps create a website in C++ with as fewer extra code as possible. I started this project for the purpose of demos and practices. Now that I have implemented some of the basic utility of an MVC framework, I am publishing it with my explanations to its details.

## Hello World

### Environment

I am working on Ubuntu Linux `gcc`, therefore I recommend you to do the same. Do not presume to use Windows, for it is 100% incompatible with Linux system call. If you are using other versions of Linux, such as Centos, there may be some tiny issues when compiling. You must alter some of the code, adding some header files, to make it work. Clang is not recommended. You must also be ware of the `gcc` under MacOSX, for it is actually `clang` by default.

### Build Tool

I am using CMake as build tool, while you may prefer *make*. They are quite alike! To build the project, you must first install it.

```
sudo apt install cmake    # Ubuntu
sudo yum install cmake    # Centos
brew install cmake        # Mac OS X
```

### Dependencies

The project is not built with the `C++ std`, but with another project of my own [AnBase](https://github.com/Anarion-zuo/AnBase), providing basic components. There are some symbolic links under the directory `library` and ``