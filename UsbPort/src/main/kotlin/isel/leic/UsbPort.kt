package isel.leic

object UsbPort {
    private val conn = WebSocketClient("ws://127.0.0.1:8083")

    init {
        conn.connect()
        while (!conn.isOpen) {
        } // block thread until we are fully connected
    }

    /** Writes [data] into UsbPort output. */
    fun write(data: Int) = conn.send("USBPORT_PUT $data")

    /** Returns UsbPort input bits. */
    fun read(): Int {
        conn.send("USBPORT_GET")
        return conn.getFutureResponse().toInt()
    }
}