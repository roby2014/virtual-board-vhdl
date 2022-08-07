#include "InteropFunctions.hpp"
#include <AppCore/AppCore.h>
#include <JavaScriptCore/JavaScript.h>

// Interoperability functions between our C++ and JS/HTML code
namespace interop {

/// Called by JavaScript to tell us that a switch has been toggled
/// JS: cpp_OnSwitchToggle(sw_id)
JSValueRef OnSwitchToggle(JSContextRef ctx, JSObjectRef function,
                          JSObjectRef thisObject, size_t argumentCount,
                          const JSValueRef arguments[], JSValueRef *exception) {
    int sw_id =
        JSValueToNumber(ctx, JSValueToObject(ctx, arguments[0], NULL), 0);
    // TODO: toggle SW[sw_id] (vpi_put_value)

    char *str;
    sprintf(
        str,
        "document.getElementById('result').innerText = 'OnSwitchToggle %d!'",
        sw_id);
    JSStringRef script = JSStringCreateWithUTF8CString(str);
    JSEvaluateScript(ctx, script, 0, 0, 0, 0);
    JSStringRelease(script);

    return JSValueMakeNull(ctx);
}

/// Called by JavaScript to tell us that a button has been pressed
/// JS: cpp_OnButtonPress(sw_id)
JSValueRef OnButtonPress(JSContextRef ctx, JSObjectRef function,
                         JSObjectRef thisObject, size_t argumentCount,
                         const JSValueRef arguments[], JSValueRef *exception) {
    int btn = JSValueToNumber(ctx, JSValueToObject(ctx, arguments[0], NULL), 0);
    // TODO: toggle buttons[btn] (vpi_put_value)

    char *str;
    sprintf(str,
            "document.getElementById('result').innerText = 'OnButtonPress %d!'",
            btn);
    JSStringRef script = JSStringCreateWithUTF8CString(str);
    JSEvaluateScript(ctx, script, 0, 0, 0, 0);
    JSStringRelease(script);

    return JSValueMakeNull(ctx);
}

} // namespace interop
