local tcpService = {}

--local tempSensor=require("ds18b20")
--tempSensor.setup(3)

gpioModule = require("gpioModule")
print("tcpService V0.8")

-- parses a raw tcp message and call the right functions
function tcpService.parseMessage(msg)
    if(msg == "DATA") then
        tcpService.sendData()
    elseif(stringParsing.stringStarts(msg, "CMD:")) then
        tcpService.processCommand(msg)
    elseif(msg == "RST") then
        node.restart()
    end
end


-- process a single command
-- a command is a set of isntruction given by the tcp server
function tcpService.processCommand(msg)
    commandLine = stringParsing.removeSubString(msg, "CMD:")
    for singleCommand in commandLine:gmatch("%S+") do 
    
        -- GPIO command syntax is GPIO:<pin>,<state>
        if(stringParsing.stringStarts(singleCommand, "GPIO:")) then
            gpioCommand = stringParsing.removeSubString(singleCommand, "GPIO:")
            _, _, pinStr, stateStr = string.find(gpioCommand, "(%d+),(%d+)")
            
            pin = tonumber(pinStr)
            state = tonumber(stateStr)
            gpioModule.setGPIOState(pin, state)
            tcpService.send(gpioModule.getGPIOStr(pin))
        end
    end
end

-- send the data from the sensors
function tcpService.sendData()
    local msg = tcpService.getTempStr()..gpioModule.getGPIOStr(4)
    tcpService.send(msg)
end

function tcpService.getTempStr()
    val = 22.1--tempSensor.read()
    return ("<sensor pin='3' type='Temperature' value='"..val.."'/>")
end

function tcpService.sendOK()
    tcpService.send("<ok/>\n")
end

function tcpService.sendID()
    tcpService.send("<chipID value= \""..node.chipid().."\"/>")
end

function tcpService.send(msg)
    socketTCP:send("<module_data>"..msg.."</module_data>\n")
end


return tcpService
