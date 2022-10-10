package isel.leic

/**
 * UsbPort library.
 * Implement functions to communicate with VHDL simulation signals (inputport_sw & outputport_sw)
 */
object UsbPort {
    private val conn = WebSocketClient("ws://127.0.0.1:8083")
    // TODO: make the url via file? constructor?...

    init {
        conn.connect()
        while (!conn.isOpen) {
        } // block thread until we are fully connected
    }

    /** Writes [data] into UsbPort output. */
    fun write(data: Int) = when {
        data > 127 -> throw Exception("$data is bigger than 7 bits")
        else -> conn.send("UPUT $data")
    }

    /** Returns UsbPort input bits. */
    fun read(): Int {
        conn.send("UGET")
        return conn.getResponse().toInt()
    }
}