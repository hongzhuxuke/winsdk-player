#pragma once
#ifdef  VH_WEBVIEW_EXPORT
#define VH_WEBVIEW_EXPORT     __declspec(dllimport)
#else
#define VH_WEBVIEW_EXPORT     __declspec(dllexport)
#endif

#include <string>
#include <windows.h>
#include "simple_handler_cb.h"




namespace vhall_paas {

    //class VHWebEventCb {
    //public:
    //    /*����WebView�ɹ�ʱ�Ļص���
    //    *������id ��Ӧ����webview��id, url ����webviewʱ��Ӧ���ص�url��ַ��
    //    *�û����ݷ��ص�id���ɲ���ָ����webview*/
    //    virtual void OnCreateWebView(int id, const char* url) = 0;
    //    /*webview�����¼�֪ͨ*/       
    //    virtual void OnWebViewDestoryed(int id) = 0;

    //    virtual void OnRecvMsg(const int id, std::string fun_name, std::string msg) = 0;

    //    virtual void OnTitleChanged(const int id, std::string title_name) = 0;
    //};


    class VHWebViewInterface
    {
    public:

       VHWebViewInterface() {};

       virtual ~VHWebViewInterface() {};
        /**
        * ��ʼ����
        * ����ֵ��0 �ɹ���-1ʧ��
        */
       virtual int InitlibCef() = 0;
        /*
        * ��������ȡһ��webview;
        * ������hwnd �����ھ��
        */
        virtual int CreateWebView(SimpleHandleEvent* cb, void* hwnd, int wnd_width, int wnd_height, const std::string url) = 0;
        /**
        * ���¼���url.
        **/
        virtual int LoadUrl(const int id, const char* url) = 0;
        /**
        * Ӧ�ò����JS.
        **/
        virtual int AppCallJsFunction(const int web_id,const char* js_function) = 0;
        /**
        *  ˢ��WebView
        */
        virtual int Reload(const int id) = 0;
        /*
        *  �޸�webview��С
        */
        virtual void ChangeWebViewSize(const int id, int width, int height) = 0;
        /*
        * �ر�ָ��webview        
        */
        virtual void CloseWebView(const int id) = 0;
        /*
        * �ر�����WebView
        */
        virtual void CloseAll() = 0;
        /*
         * http ����
         * proxy_server�� server:port
         */
        virtual void SetProxy(bool enable, const char* proxy_server, const char* proxy_user, const char* proxy_pwd) = 0;
    };

    VH_WEBVIEW_EXPORT VHWebViewInterface* GetVHWebViewObject();
    VH_WEBVIEW_EXPORT void DestoryVHWebViewObject(VHWebViewInterface* obj);
}