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
        //�¼��ص�
        virtual void OnVHPlayerEvent(ERRORCODE code,const char* msg) = 0;
        /*����WebView�ɹ���URL�������ʱ�Ļص���
        *������id ��Ӧ����webview��id, url ����webviewʱ��Ӧ���ص�url��ַ��
        *�û����ݷ��ص�id���ɲ���ָ����webview*/
        virtual void OnCreateWebView(int id, const char* url) = 0;
        /*webview�����¼�֪ͨ*/
        virtual void OnWebViewDestoryed(int id) = 0;
    };

    class VHPlayerInterface : public ServerMsgObserver
    {
    public:
        /**
        * ��ʼ��֮ǰ����
        */
        virtual void RegisterCbEventObj(VHPlayerEvent *obj) = 0;
        /*
        * ��ʼ��
        */
        virtual int Init() = 0;
        /*
        * ��GetRoomWatchDetails���óɹ��󣬷��ɴ�������ȡһ��webview������֮����Զ����عۿ���ַ��
        * ������hwnd �����ھ��  ��ָ����ʾ���Ǹ����ڡ�
        *      wnd_width�����ڿ��
        *      wnd_height�����ڸ߶�
        */
        virtual int CreateWebView(const char* url,void* hwnd, int wnd_width, int wnd_height) = 0;
        /**
         * ���¼���url.
         **/
        virtual int Play(int webview_id, const char* url) = 0;
        /**
        *  ˢ��WebView
        */
        virtual int Reload(int webview_id) = 0;
        /*
        *  �޸�webview��С
        */
        virtual void ChangeWebViewSize(int webview_id, int width, int height) = 0;
        /*
        * �ر�WebView
        */
        virtual void Close(int webview_id) = 0;
        /*
        * �ر�����WebView
        */
        virtual void CloseAll() = 0;
    };

    VHPLAYER_EXPORT VHPlayerInterface* GetVHPlayerInstance();
    VHPLAYER_EXPORT void DestoryVHPlayerInstance();
}

