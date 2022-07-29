#ifndef INTEROP_FUNCTIONS_HPP
#define INTEROP_FUNCTIONS_HPP

#include <AppCore/AppCore.h>
#include <JavaScriptCore/JavaScript.h>

// Interoperability functions between our C++ and JS/HTML code
namespace interop {

/// Called by JavaScript to tell us that a switch has been toggled
/// JS: cpp_OnSwitchToggle(sw_id)
JSValueRef OnSwitchToggle(JSContextRef ctx, JSObjectRef function,
                          JSObjectRef thisObject, size_t argumentCount,
                          const JSValueRef arguments[], JSValueRef *exception);

/// Called by JavaScript to tell us that a button has been pressed
/// JS: cpp_OnButtonPress(sw_id)
JSValueRef OnButtonPress(JSContextRef ctx, JSObjectRef function,
                         JSObjectRef thisObject, size_t argumentCount,
                         const JSValueRef arguments[], JSValueRef *exception);

} // namespace interop

#endif // !INTEROP_FUNCTIONS_HPP