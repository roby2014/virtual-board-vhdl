package isel.leic

/**
 * UsbPort library.
 * Implement functions to communicate with VHDL simulation signals (inputport_sw & outputport_sw)
 */
object UsbPort {
    // TODO: get host and port via config file?

    /** Returns UsbPort input port bits. */
    fun read() = khttp.get("http://0.0.0.0:8080/usbport/inputport_sw").text.toInt()

    /** Writes [data] into UsbPort output port. */
    fun write(data: Int) =
        khttp.put(url = "http://0.0.0.0:8080/usbport/outputport_sw", data = mapOf("value" to "$data")).let {
            if (it.statusCode != 200 || it.text.toInt() != data) throw Exception("Something went wrong with UsbPort.write($data)..")
        }
}