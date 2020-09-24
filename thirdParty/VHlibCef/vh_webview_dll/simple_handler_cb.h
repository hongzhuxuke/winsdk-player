#pragma once
#include <string>

class SimpleHandleEvent {
public:
    virtual void OnHandleCreateWebView(int id, const char* url) = 0;
    virtual void OnHandleWebViewDestoryed(int id) = 0;
    virtual void OnRecvMsg(const int id, std::string fun_name, std::string msg) = 0;
    virtual void OnTitleChanged(const int id, std::string title_name) = 0;
};
