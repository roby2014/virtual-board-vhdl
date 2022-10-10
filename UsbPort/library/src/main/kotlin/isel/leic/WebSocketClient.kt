package isel.leic

import org.java_websocket.client.WebSocketClient
import org.java_websocket.handshake.ServerHandshake
import java.net.URI
import java.util.concurrent.CompletableFuture


internal class WebSocketClient(url: String) : WebSocketClient(URI(url)) {
    var future = CompletableFuture<String>()

    fun getResponse(): String {
        future = CompletableFuture<String>()
        return future.get()
    }

    override fun onOpen(handshakedata: ServerHandshake) {
    }

    override fun onMessage(message: String) {
        future.complete(message)
    }

    override fun onClose(code: Int, reason: String, remote: Boolean) {
        // The close codes are documented in class org.java_websocket.framing.CloseFrame
        println(
            "Connection closed by: ${if (remote) "server" else "client"}\nCode: $code \nReason: $reason"
        )
    }

    override fun onError(ex: Exception) {
        ex.printStackTrace()
        // if the error is fatal then onClose will be called additionally
    }

}
