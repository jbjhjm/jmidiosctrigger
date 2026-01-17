-- *********************************************
-- MA2OSC failsafe 
-- checks state of ma2osc plugin every few seconds 
-- and restarts it if its state variable reports it was stopped.
-- *********************************************


local messagePrefix = string.char(27)..'[35m'..'MA2OSC failsafe: '..string.char(27)..'[33m'
local function alert(message)
	gma.feedback(messagePrefix..message);
	gma.echo(messagePrefix..message)
end

local function runCommand(command)
	command = command:gsub('[|\\^]','');
	return gma.cmd(command)
end

local plugin={
	receive_every=5, -- receive interval in seconds
	ma2osc_name = "ma2osc",
}

function plugin.observe()
	while true do
		-- alert("checking state")
		local state = gma.show.getvar('OSC_INPUT_STATE')
		if state == "stopped" then
			alert("restarting stopped ma2osc")
			runCommand("Plugin "..plugin.ma2osc_name)
		end
		gma.sleep(plugin.receive_every)
	end
end

local function startOSCFailsafe()
	plugin.observe()
end
local function endOSCFailsafe()
	
end

return startOSCFailsafe,endOSCFailsafe

