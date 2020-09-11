#include "VHPlayerImpl.h"
#include "vhall_log.h"
#include "vhall_sdk_def.h"

static std::mutex gSDKMutex;
static VHPlayerImpl* gPaasSDKInstance = nullptr;


VHPlayerImpl::VHPlayerImpl(){
   //mPlayUrl = "http://e.vhall.com";
   //mPlayUrl = "http://e.vhall.com/room/assitant/755760929?assistantType=doc&assistant_token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpZCI6IjE4NDYxNjgxIiwicGhvbmUiOiIxMzU1MjE0NDA2MCIsIm5pY2tfbmFtZSI6Ilx1OTBiMVx1NWI4NzExMTExIn0.IiHoFHjc65jfDM9XLHs6oQxP4xZtokGNuJNC-D9oR_o&hide=1";
  // mPlayUrl = "https://live.vhall.com/room/watch/755760929?";
   mPlayUrl = "file:///I:/pro/vhall-saas-player-client/text.html";
}


VHPlayerImpl::~VHPlayerImpl(){

}

void VHPlayerImpl::NotifyMsy(const char* type, const char* msg) {

}

void VHPlayerImpl::OnWebViewDestoryed(int id) {
   if (mVHPlayerEventObj) {
      mVHPlayerEventObj->OnWebViewDestoryed(id);
   }
}

void VHPlayerImpl::OnCreateWebView(int id, const char* url) {
    if (mVHPlayerEventObj) {
       mVHPlayerEventObj->OnCreateWebView(id, url);
    }
}

void VHPlayerImpl::RegisterCbEventObj(VHPlayerEvent *obj) {
    mVHPlayerEventObj = obj;
}

int VHPlayerImpl::Init() {
    if (mpVHWebViewInterface == nullptr) {
        mpVHWebViewInterface = GetVHWebViewInstance();
        mpVHWebViewInterface->InitlibCef(this);
    }
    InitLog(L"");
    return 0;
}

void VHPlayerImpl::Close(int webview_id) {
   if (mpVHWebViewInterface) {
      mpVHWebViewInterface->CloseWebView(webview_id);
   }
}

/*
* 创建并获取一个webview;
* 参数：hwnd 父窗口句柄
*/
int VHPlayerImpl::CreateWebView(const char* url, void* hwnd, int wnd_width, int wnd_height) {
    if (mpVHWebViewInterface) {
        mpVHWebViewInterface->CreateWebView(hwnd, wnd_width, wnd_height, std::string(url));
    }
    return 0;
}
/**
  * 重新加载url.
  **/
int VHPlayerImpl::Play(int webview_id, const char* url) {
    if (mpVHWebViewInterface) {
        mpVHWebViewInterface->LoadUrl(webview_id, url);
    }
    return 0;
}
/**
*  刷新WebView
*/
int VHPlayerImpl::Reload(int webview_id) {
    if (mpVHWebViewInterface) {
        mpVHWebViewInterface->Reload(webview_id);
    }
    return 0;
}
/*
*  修改webview大小
*/
void VHPlayerImpl::ChangeWebViewSize(int webview_id, int width, int height) {
    if (mpVHWebViewInterface) {
        mpVHWebViewInterface->ChangeWebViewSize(webview_id, width, height);
    }
}
/*
* 关闭所有WebView
*/
void VHPlayerImpl::CloseAll() {
    if (mpVHWebViewInterface) {
        mpVHWebViewInterface->CloseAll();
    }
}

namespace vhall_paas {
    VHPLAYER_EXPORT VHPlayerInterface* GetVHPlayerInstance() {
        std::unique_lock<std::mutex> lock(gSDKMutex);
        if (gPaasSDKInstance == nullptr) {
            gPaasSDKInstance = new VHPlayerImpl();
        }
        return (VHPlayerInterface*)gPaasSDKInstance;
    }

    VHPLAYER_EXPORT void DestoryVHPlayerInstance() {
        std::unique_lock<std::mutex> lock(gSDKMutex);
        if (gPaasSDKInstance) {
            delete gPaasSDKInstance;
            gPaasSDKInstance = nullptr;
        }
    }
}

