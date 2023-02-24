
# CPP Server  

## Context

> With the development of Cloud Computing, people can access servers more conveniently than before. And almost all highly concurrent servers are run on Linux. That is why I did this project of developing a library to support server run on Linux with C++

## Action

> In this project, I programmed the basic logic of how server works in a single file at first. And then what I did is to make it more and more abstract iteratively.

## Result

> The core modules of a basic server is completed. In this project, I applied Main&Sub Reactor Multithreading structure.
> Within this structure, the server is driven by events. And the thread occupied by the server, also called mainReactor, is only responsible for accepting new connections and maintaining the threadpool. Other threads called subReactor are responsible for listening to the socket and process the data.  

## Learning  

> My biggest takeaway from this project is revisiting what I have learned so far, including OOP of C++, first classes in C++ and multithread programming and basic knowledge about computer network.

## Need to finish

1. To CMakeLists
2. Use format, cpplint, clang-tidy to make the code formalized
3. The logic of the events is set. I have to let the logic be cusomized by the user
4. Reconstruct the code, now it is too C
