import isel.leic.UsbPort
import java.lang.Thread.sleep

// quick test
fun main(args: Array<String>) {
    UsbPort.write(15)

    while (true) {
        println("inputport_sw = " + UsbPort.read())
        sleep(1000)
    }
}