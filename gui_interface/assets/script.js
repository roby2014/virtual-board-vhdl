/// on switch press
$(document).ready(function () {
  $(".switchs").click(function () {
    let on = $(this).children(".switch-on");
    let off = $(this).children(".switch-off");

    let value = $(this).attr("data-value");
    cpp_OnSwitchToggle(value);
    
    $(this).toggleClass("active");

    if ($(this).hasClass("active")) {
      on.removeClass("on");
      off.addClass("on");
    } else {
      on.addClass("on");
      off.removeClass("on");
    }
  });

  $(".btn").click(function () {
    $(this).children(".btn-circle").toggleClass("btn-active");
  });
});

/// sets specific led to [value]
/// led_id should be [0..9]
function jsSetLed(led_id, value) {
  if (value == 0) {
    $(`#led-${led_id}`).removeClass("light-active");
  } else {
    $(`#led-${led_id}`).addClass("light-active");
  }
}
