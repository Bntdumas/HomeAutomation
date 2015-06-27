local gpioModule = {}

-- set input/output for GPIOs
gpio.mode(4,gpio.OUTPUT)
gpio.write(4,gpio.HIGH)
local GPIO2 = 1

function gpioModule.setGPIOState(pin, state)
    if(state == 1) then
        gpio.write(pin, gpio.HIGH)
        gpioModule.setGPIOVar(pin, state)
    else 
        gpio.write(pin,gpio.LOW)
        gpioModule.setGPIOVar(pin, state)
    end
end

function gpioModule.setGPIOVar(pin, state)   
    if(pin == 4) then
        GPIO2 = state
    end
end

function gpioModule.GPIOState(pin)
    if(pin == 4) then
        return GPIO2
    else 
        return "err"
    end
end

function gpioModule.getGPIOStr(pin)
    local msg = "<gpio pin='"..tonumber(pin).."' value='"..tonumber(gpioModule.GPIOState(pin)).."'/>"
    return msg
end

return gpioModule
