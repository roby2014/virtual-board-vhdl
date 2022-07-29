#include "MyApp.hpp"
#include "InteropFunctions.hpp"
#include <JavaScriptCore/JavaScript.h>

#define WINDOW_WIDTH 1440
#define WINDOW_HEIGHT 720

/// Create our main App instance.
MyApp::MyApp() {
    app_ = App::Create();
    window_ = Window::Create(app_->main_monitor(), WINDOW_WIDTH, WINDOW_HEIGHT,
                             false, kWindowFlags_Titled);
    overlay_ = Overlay::Create(window_, 1, 1, 0, 0);
    OnResize(window_.get(), window_->width(), window_->height());
    overlay_->view()->LoadURL("file:///index.html");
    app_->set_listener(this);
    window_->set_listener(this);
    overlay_->view()->set_load_listener(this);
    overlay_->view()->set_view_listener(this);
}

MyApp::~MyApp() {}

/// Start the run loop.
void MyApp::Run() {
    app_->Run();
}

/// This is called continuously from the app's main loop.
void MyApp::OnUpdate() {}

/// This is called when the window is closing.
void MyApp::OnClose(ultralight::Window *window) {
    app_->Quit();
}

/// This is called whenever the window resizes.
void MyApp::OnResize(ultralight::Window *window, uint32_t width,
                     uint32_t height) {
    overlay_->Resize(width, height);
}

/// This is called when the page finishes a load in one of its frames.
void MyApp::OnFinishLoading(ultralight::View *caller, uint64_t frame_id,
                            bool is_main_frame, const String &url) {}

/// This is called when the DOM has loaded in one of its frames.
void MyApp::OnDOMReady(ultralight::View *caller, uint64_t frame_id,
                       bool is_main_frame, const String &url) {
    RefPtr<JSContext> context = caller->LockJSContext();
    JSContextRef ctx = context->ctx();

    // register functions that can be called by JavaScript
    // register cpp_OnSwitchToggle
    JSStringRef name = JSStringCreateWithUTF8CString("cpp_OnSwitchToggle");
    JSObjectRef func =
        JSObjectMakeFunctionWithCallback(ctx, name, interop::OnSwitchToggle);
    JSObjectRef globalObj = JSContextGetGlobalObject(ctx);
    JSObjectSetProperty(ctx, globalObj, name, func, 0, 0);
    JSStringRelease(name);

    // register cpp_OnButtonPress
    name = JSStringCreateWithUTF8CString("cpp_OnButtonPress");
    func = JSObjectMakeFunctionWithCallback(ctx, name, interop::OnButtonPress);
    JSObjectSetProperty(ctx, globalObj, name, func, 0, 0);
    JSStringRelease(name);

    //
    overlay_->view()->EvaluateScript("jsSetLed('9', 1)");
}

// This is called when the page requests to change the Cursor.
void MyApp::OnChangeCursor(ultralight::View *caller, Cursor cursor) {
    window_->SetCursor(cursor);
}

void MyApp::OnChangeTitle(ultralight::View *caller, const String &title) {
    window_->SetTitle(title.utf8().data());
}
