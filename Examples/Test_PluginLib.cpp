/*
 * @file: Test_PluginLib.cpp
 * @Author: INotFound
 * @Date: 2020-03-16 00:16:18
 * @LastEditTime: 2020-03-16 00:16:31
 */
#include <memory>

class IPluginModule {
public:
    virtual int arg() =0;
    virtual ~IPluginModule() {}
};


class PluginModule:public IPluginModule {
public:
    ~PluginModule() {}
    PluginModule() {}
    int arg() override{
        return 6666666;
    }
};
#if defined(_WIN32) || defined(_WIN64)
//Win
 extern "C"
 {
     _declspec(dllexport) void* create()
     {
         return new PluginModule;
     }
 }
#endif

#if defined(linux) || defined(__linux__)
//Linux
extern "C"
{
    void* create()
    {
        return new PluginModule;
    }
}
#endif