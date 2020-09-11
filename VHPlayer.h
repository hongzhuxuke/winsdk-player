#pragma once

#ifdef  VHPLAYER_EXPORT
#define VHPLAYER_EXPORT     __declspec(dllimport)
#else
#define VHPLAYER_EXPORT     __declspec(dllexport)
#endif

#include "vhall_paas_def.h"

namespace vhall_paas {

    class VHPlayerEvent {
    public:
        //事件回调
        virtual void OnVHPlayerEvent(ERRORCODE code,const char* msg) = 0;
        /*创建WebView成功与URL加载完成时的回调。
        *参数：id 对应创建webview的id, url 创建webview时对应加载的url地址。
        *用户根据返回的id，可操作指定的webview*/
        virtual void OnCreateWebView(int id, const char* url) = 0;
        /*webview销毁事件通知*/
        virtual void OnWebViewDestoryed(int id) = 0;
    };

    class VHPlayerInterface : public ServerMsgObserver
    {
    public:
        /**
        * 初始化之前调用
        */
        virtual void RegisterCbEventObj(VHPlayerEvent *obj) = 0;
        /*
        * 初始化
        */
        virtual int Init() = 0;
        /*
        * 当GetRoomWatchDetails调用成功后，方可创建并获取一个webview。创建之后会自动加载观看地址。
        * 参数：hwnd 父窗口句柄  ，指定显示在那个窗口。
        *      wnd_width：窗口宽度
        *      wnd_height：窗口高度
        */
        virtual int CreateWebView(const char* url,void* hwnd, int wnd_width, int wnd_height) = 0;
        /**
         * 重新加载url.
         **/
        virtual int Play(int webview_id, const char* url) = 0;
        /**
        *  刷新WebView
        */
        virtual int Reload(int webview_id) = 0;
        /*
        *  修改webview大小
        */
        virtual void ChangeWebViewSize(int webview_id, int width, int height) = 0;
        /*
        * 关闭WebView
        */
        virtual void Close(int webview_id) = 0;
        /*
        * 关闭所有WebView
        */
        virtual void CloseAll() = 0;
    };

    VHPLAYER_EXPORT VHPlayerInterface* GetVHPlayerInstance();
    VHPLAYER_EXPORT void DestoryVHPlayerInstance();
}

