#pragma once
#include "VHPlayer.h"
#include <mutex>
#include <thread>
#include "VHWebView.h"
#include <list>
#include <atomic>
#include <vector>
#include <memory>

using namespace vhall_paas;

enum TaskType
{
    Task_CallDispatchStart = 0,
};


class VHPlayerImpl : public VHPlayerInterface, public VHWebEventCb
{
public:
    VHPlayerImpl();
    ~VHPlayerImpl();

    /*创建WebView成功时的回调。
    *参数：id 对应创建webview的id, url 创建webview时对应加载的url地址。
    *用户根据返回的id，可操作指定的webview*/
    virtual void OnCreateWebView(int id, const char* url);
    /*webview销毁事件通知*/
    virtual void OnWebViewDestoryed(int id);
    //socketio msg 消息
    virtual void NotifyMsy(const char* type, const char* msg);
    /**
    * 注册回调事件通知
    */
    virtual void RegisterCbEventObj(VHPlayerEvent *obj);
    /*
    * 初始化
    */
    virtual int Init();
    /*
    * 创建并获取一个webview;
    * 参数：hwnd 父窗口句柄
    */
    virtual int CreateWebView(const char* url, void* hwnd, int wnd_width, int wnd_height);
    /**
      * 重新加载url.
      **/
    virtual int Play(int webview_id,const char* url);
    /**
    *  刷新WebView
    */
    virtual int Reload(int webview_id);
    /*
    *  修改webview大小
    */
    virtual void ChangeWebViewSize(int webview_id, int width, int height);
    /*
    * 关闭WebView
    */
    virtual void Close(int webview_id);
    /*
    * 关闭所有WebView
    */
    virtual void CloseAll();
private:
    VHWebViewInterface*  mpVHWebViewInterface = nullptr;
    VHPlayerEvent* mVHPlayerEventObj = nullptr;
    void *mViewHwnd = nullptr;

    std::string mPlayUrl;
};

