# UI example
This is an example on how you can build an UI and use the websocket server to manipulate IO signals.

In case you want to test the given example, go into run_script, run the simulation via: 

`./virtual_board.py . up_counter /home/roby/repos/board-emulator-vhdl/vhdl_vpi/build/virtual_board.vpi `

and open `index.html` to see the counter working (dont forget to activate the 'enable' signal!).

https://user-images.githubusercontent.com/47612455/197650594-275280c8-55eb-46e3-923b-9dbaa9074d76.mp4

## Read ME!!!
This is a "static" UI, which means the developer that made this UI, depends on a "static" board.cfg.
**The right approach would be to create a UI that reads `board.json` and creates a UI with IO signals depending on what pins the config file has (which is my next goal on this project)**
