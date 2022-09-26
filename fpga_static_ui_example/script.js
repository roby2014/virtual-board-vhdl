/// on switch press
$(document).ready(function () {
  const virtual_board = new WebSocket('ws://0.0.0.0:8083')

  virtual_board.onmessage = event => {
    const msg = event.data
    if (msg.includes('[ERROR]')) {
      alert(event.data)
      return
    }

    const words = msg.split(' = ')
    const pin_id = words[0]
    const value = Number(words[1])

    jsSetLed(pin_id, value)
  }

  virtual_board.onerror = function (error) {
    alert(error)
  }

  $('.switchs').click(function () {
    let on = $(this).children('.switch-on')
    let off = $(this).children('.switch-off')

    let pin_id = $(this).attr('data-value')

    $(this).toggleClass('active')

    const value = !$(this).hasClass('active') ? '1' : '0'

    console.log(`${pin_id} changed to ${value}`)

    virtual_board.send(`CHANGE ${pin_id} ${value}`)

    if ($(this).hasClass('active')) {
      on.removeClass('on')
      off.addClass('on')
    } else {
      on.addClass('on')
      off.removeClass('on')
    }
  })

  $('.btn').click(function () {
    $(this)
      .children('.btn-circle')
      .toggleClass('btn-active')
  })
})

/// sets specific led to [value]
/// obviously, the led div should have the id html attribute
/// where id can be PIN_A[0..9]
function jsSetLed (led_id, value) {
  if (value == 0) {
    $(`#${led_id}`).removeClass('light-active')
  } else {
    $(`#${led_id}`).addClass('light-active')
  }
}
