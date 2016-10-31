# How to add potrace to your progect

## Step1. Build your .pro file
Open the QxPotrace.pro file with you QtCreator,then build it.In your project dir,you will find a bin/ dir. The "libQxPotrace.a" and "QxPortace.dll" is what you need.If you change any code of the QtPortace,you show rebuild the QtPotrace project,then add the .dll file to your project.

## Step2.Add dll to you own project.
After you complie the QtPotrace,add the following line to your own .pro file:

```c++
include(./QxPotrace.pri)
```

change the include(...) to your own path.
