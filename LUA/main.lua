-- Starts the module and connect to server. 
print("main V0.1")

-- wifi connection
wifi.setmode(wifi.STATION)
wifi.sta.config("JESUS","iletaitunefois")
local cfg =
{
    ip="192.168.0.53",
    netmask="255.255.255.0",
    gateway="192.168.0.1"
}    

-- connection to home server
local server_ip = "192.168.0.14"
local server_port = 5003
socketTCP = net.createConnection(net.TCP, 0)

--some global vars
tcpService = require("tcpService")
stringParsing = require("stringParsing")

socketTCP:on("connection", function(cu)
    print("Connected.")
    tcpService.sendID()
end)

-- callback data received
socketTCP:on("receive", function(cu, msg)
    -- Check if there's several commands and process each one 
    tcpService.parseMessage(msg)
end)

-- check every 0.5seconds if we can get an ip, once we get it start the TCP client
tmr.alarm(0, 500, 1, function()
    gotIP = wifi.sta.setip(cfg)
    if ( wifi.sta.status() == 5 ) then
        socketTCP:connect(server_port, server_ip)
        tmr.stop(0)
    end
end)
