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

    /*����WebView�ɹ�ʱ�Ļص���
    *������id ��Ӧ����webview��id, url ����webviewʱ��Ӧ���ص�url��ַ��
    *�û����ݷ��ص�id���ɲ���ָ����webview*/
    virtual void OnCreateWebView(int id, const char* url);
    /*webview�����¼�֪ͨ*/
    virtual void OnWebViewDestoryed(int id);
    //socketio msg ��Ϣ
    virtual void NotifyMsy(const char* type, const char* msg);
    /**
    * ע��ص��¼�֪ͨ
    */
    virtual void RegisterCbEventObj(VHPlayerEvent *obj);
    /*
    * ��ʼ��
    */
    virtual int Init();
    /*
    * ��������ȡһ��webview;
    * ������hwnd �����ھ��
    */
    virtual int CreateWebView(const char* url, void* hwnd, int wnd_width, int wnd_height);
    /**
      * ���¼���url.
      **/
    virtual int Play(int webview_id,const char* url);
    /**
    *  ˢ��WebView
    */
    virtual int Reload(int webview_id);
    /*
    *  �޸�webview��С
    */
    virtual void ChangeWebViewSize(int webview_id, int width, int height);
    /*
    * �ر�WebView
    */
    virtual void Close(int webview_id);
    /*
    * �ر�����WebView
    */
    virtual void CloseAll();
private:
    VHWebViewInterface*  mpVHWebViewInterface = nullptr;
    VHPlayerEvent* mVHPlayerEventObj = nullptr;
    void *mViewHwnd = nullptr;

    std::string mPlayUrl;
};

