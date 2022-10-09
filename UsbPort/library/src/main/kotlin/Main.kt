import isel.leic.UsbPort

fun main(args: Array<String>) {
    UsbPort.write(15)
    println("read = " + UsbPort.read())
}