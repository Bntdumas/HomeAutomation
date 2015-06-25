local tcpSend = {}

-- send the data form the sensors
function tcpSend.sendData(cu)
    addrs=t.addrs()
    cu:send("$TEMP:"..t.read(nil,t.C)..'\n')
end

function tcpSend.sendPong()
    cu:send("PONG\n")
end

function tcpSend.sendID()
    print("sending")
    cu:send(node.chipid()..'\n')
    print("sent")
end

return tcpSend