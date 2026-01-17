-- *********************************************
-- MA2OSC listens for OSC messages and allows to execute certain commands in ma2.
-- *********************************************


local messagePrefix = string.char(27)..'[35m'..'MA2OSC: '..string.char(27)..'[33m'
local function alert(message)
	gma.feedback(messagePrefix..message)
	gma.echo(messagePrefix..message)
end

local function unsupportedCmd(extra)
	alert('Unsupported or wrong command: '..extra)
end

local function runCommand(command)
	command = command:gsub('[|\\^]','')
	return gma.cmd(command)
end

local socket = require('socket/socket')

-- "#var" returns the LENGTH of string, size of table etc

-- https://midisoft.de/Hints_and_Tools/OSC_Open_Sound_Control.html

-- Full OSC supports various variable types indicated by a single letter
-- i int32
-- f float32
-- s OSC-string
-- b OSC-blob
-- h 64 bit big-endian two’s complement integer
-- t OSC-timetag
-- d 64 bit (“double”) IEEE 754 floating point number
-- S alternate type represented as an OSC-string
-- c ASCII character
-- r 32 bit RGBA color
-- m 4 byte MIDI message
-- T True
-- F False
-- N Nil
-- I Infinitum
local type_int32='i'
local type_float32='f'
local type_string='s'
local type_true='T'
local type_false='F'
local type_infinite='I'
local type_nil='N'
local oscBundlePrefix="#bundle"

-- OSC is based on blocks of 4 bytes = int32.
local function readInt32(data,offset)
	local b1,b2,b3,b4 = string.byte(data, offset, offset+3)
	local int32 = b1 * 16777216 + b2 * 65536 + b3 * 256 + b4
	if int32 > 2147483647 then
		int32 = int32-4294967296
	end
	return int32, offset+4
end

local function readFloat32(data,offset)
	local floatValue = string.unpack(">f", string.sub(data, offset, offset+3))
	floatValue = math.floor(floatValue*1000000+.5)/1000000
	return floatValue, offset+4
end

local function readMessageDataUntilPadding(message,offset)
	-- find next padding byte
	local paddingStartPosition = string.find(message,"\0",offset)
	-- read data up to the padding
	local data = string.sub(message, offset, paddingStartPosition-1)
	-- calc the number of padding bytes - data is based on blocks of 4 bytes!
	local paddingLength = (4-((paddingStartPosition-1)%4))
	return data, paddingStartPosition + paddingLength
end

-- local function setSendingIpAndPort(ip,port)
-- 	gma.show.setvar('OSC_SEND_IP_PORT',ip..':'..port)
-- 	alert('Set sending IP: '..ip..', port: '..port)
-- end

-- local function setInputPort(port)
-- 	gma.show.setvar('OSC_INPUT_PORT',port)
-- 	alert('Set listening port: '..port)
-- end

local function wrapNonNumericString(val)
	if val==nil then
		return ""
	end
	if not tonumber(val) then
		return '"'..val..'"'
	else
		return val
	end
end
local function getOptionalFadeDuration(val)
	if val==nil then
		return ''
	end

	local fade = tonumber(val)
	if not fade then
		return ''
	else
		return ' Fade '..fade
	end
end

local function isSegmentASpecificCommand(val)
	if val=='go' or val=='goto' or val=='top' or val=='off' or val=='master' or val=='fader' then
		return true
	end
end


local plugin={
	receive_every=.1, -- receive interval in seconds
	receivedDataOnPrimaryPort=false,
}


function plugin.start_listening(ip, port, fallbackPort)
	plugin.udp = assert(socket.udp(),"Failed to create UDP socket")
	plugin.udp:settimeout(0)
	plugin.udp:setsockname(ip,port)
	alert('Listening for OSC messages on '..ip..':'..port..'.')
	if fallbackPort then
		plugin.udp2 = assert(socket.udp(),"Failed to create UDP fallback socket")
		plugin.udp2:settimeout(0)
		plugin.udp2:setsockname(ip,fallbackPort)
		alert('Listening for fallback OSC messages on '..ip..':'..fallbackPort..'.')
	end
	while true do
		repeat
			local package = plugin.udp:receive() -- package is a "datagram" - could be a simple string?!
			if package then 
				plugin.receivedDataOnPrimaryPort = true
			elseif not plugin.receivedDataOnPrimaryPort and plugin.udp2 then
				-- check for packages on fallback port
				package = plugin.udp2:receive()
			end
			if package then
				if package:sub(1,#oscBundlePrefix)==oscBundlePrefix then
					plugin.handle_bundle(package)
				else
					plugin.handle_single_message(package)
				end
			end
		until not package
		gma.sleep(plugin.receive_every)
	end
end
function plugin.handle_bundle(package)
	-- bundle package starts with #bundle prefix followed by 0 padding
	-- after that 8 Bytes of time tag follows
	-- so the payload begins after 12 bytes: "#bundle0TTTTTTTT"
	local offset = 12
	while offset < #package do
		-- bundle starts with an int32 specifying the payload length
		local bundleSize, newOffset = readInt32(package,offset)
		-- extract bundle payload
		local packageBody = package:sub(newOffset, newOffset + bundleSize - 1)
		if packageBody:sub(1,#oscBundlePrefix)==oscBundlePrefix then
			plugin.handle_bundle(packageBody)
		else
			plugin.handle_single_message(packageBody)
		end
		-- advance offset to read contents after the just processed bundle
		offset = newOffset + bundleSize
	end
end
function plugin.handle_single_message(inputMessage)
	-- inputMessage format = <address>[padding]<type>[padding]<payload>
	-- alert('handle_single_message: '..inputMessage)
	local addressData,nextOffset = readMessageDataUntilPadding(inputMessage,1)
	local varTypeList
	varTypeList,nextOffset = readMessageDataUntilPadding(inputMessage,nextOffset)
	varTypeList = varTypeList:sub(2)
	-- alert('handle_single_message - address: '..addressData..', var types: '..varTypeList)
	local parsedValues={}

	-- NOTE: this looks like an unfinished implementation. Index is not increased anywhere.
	-- likely an implementation that was going to handle multiple values later? 
	-- for index=1, #chunk do
	local parsedValues={}
	for index=1,#varTypeList do
		local char=varTypeList:sub(index,index)
		local value
		if char==type_int32 then
			value,nextOffset=readInt32(inputMessage,nextOffset)
		elseif char==type_float32 then
			value,nextOffset=readFloat32(inputMessage,nextOffset)
		elseif char==type_string then
			value,nextOffset=readMessageDataUntilPadding(inputMessage,nextOffset)
		elseif char==type_infinite then
			local n
			n,nextOffset=readInt32(inputMessage,nextOffset)
			value=(n~=0)
		elseif char==type_true then
			value=true
		elseif char==type_false then
			value=false
		elseif char==type_nil then
			value=0
		else
			alert('Unsupported OSC type: "'..char..'"')
			return
		end
		table.insert(parsedValues,value)
	end
	-- end
	plugin.process_message(addressData,parsedValues)
end

function plugin.process_message(addressData,parsedValues)
	local stringified=''
	for i,value in ipairs(parsedValues) do
		stringified = stringified..tostring(value)..','
	end
	-- not sure if ,$ is a literal to be replaced or if gsub interpretes it as regex
	stringified = string.gsub(stringified, ',$', '')
	-- alert('Received OSC: '..addressData..' '..stringified)

	-- OSC standard does not permit spaces in address. 
	-- as an alternative, we interprete ~ in incoming address as whitespace.
	addressData = string.gsub(addressData, '~', ' ')

	local addressSegments={}
	for n in string.gmatch(string.lower(addressData)..'/', "(.-)/") do
		if n~='' then
			table.insert(addressSegments,n)
		end
	end

	-- alert("given address segments: "..table.concat(addressSegments,','))

	-- if addressSegments[1]=='oscmate' then
	-- 	if addressSegments[2]=='set_destination_ip' then
	-- 		-- /oscmate/set_destination_ip/x.x.x.x:dddd
	-- 		local ipString=addressSegments[3] or parsedValues[1]
	-- 		if ipString then
	-- 			local ip,port = ipString:match('^(%d+%.%d+%.%d+%.%d+):(%d+)')
	-- 			if ip and port then
	-- 				setSendingIpAndPort(ip,port)
	-- 				return
	-- 			end
	-- 		end
	-- 		unsupportedCmd()
	-- 		return
	-- 	else
	-- 		-- /oscmate/<somethingelse> -- remove oscmate segment and continue
	-- 		table.remove(addressSegments,1)
	-- 	end
	-- end

	-- /gma2/<somethingelse> -- remove gma2 segment and continue
	if addressSegments[1]=='gma2' then
		table.remove(addressSegments,1)
	end

	if #addressSegments==0 then
		unsupportedCmd("no address segments left!")
		return
	end

	-- /cmd/<data> or /cmd + string-value
	if addressSegments[1]=='cmd' then
		local cmd=parsedValues[1]
		if cmd then
			-- alert("Execute cmd"..parsedValues[1])
			runCommand(cmd)
		else
			unsupportedCmd('No command to execute was passed')
		end
		return

	-- /macro/number or /macro + number-value
	elseif addressSegments[1]=='macro' then
		local macroNumber = addressSegments[2] or parsedValues[1]
		if macroNumber then
			runCommand('Macro '..(wrapNonNumericString(macroNumber)))
			return
		else
			unsupportedCmd('no macroNumber found')
		end
	end

	local pageId
	-- handle /page/xxx by either executing Page command or by storing pageId and removing both segments from list
	if addressSegments[1]=='page' then
		table.remove(addressSegments, 1)
		-- this more complicated setup is required to support chaining with sub commands I guess.
		-- something like /page/16/exec/3/go.
		pageId = addressSegments[1]
		if #addressSegments==1 then
			runCommand('Page '..(wrapNonNumericString(pageId)))
			return
		else
			table.remove(addressSegments, 1)
		end
	end
	-- alert("left address segments: "..table.concat(addressSegments,','))

	-- if segment begins with fader, replace it with "executer" and toggle matchedFaderKeyword
	local matchedFaderKeyword
	if addressSegments[1]:match('^fader') then
		matchedFaderKeyword=true
		addressSegments[1] = string.gsub(addressSegments[1], '^fader', 'executor')
	end

	-- handle executor
	-- left possibilities: 
	-- - /executor/<execId>
	-- - /executor/<execId>/<action>
	-- - /executor/<pageId>.<execId>
	-- - /executor/<pageId>.<execId>/<action>
	if addressSegments[1]=='executor' then
		local execId, cmdAction

		execId = table.remove(addressSegments,2)
		execId = wrapNonNumericString(execId)

		if isSegmentASpecificCommand(addressSegments[2]) then
			cmdAction = table.remove(addressSegments,2)
		else
			cmdAction = 'master'
		end
		-- alert("solved cmdAction: "..cmdAction)

		-- if a page was given, join pageId with execId
		if pageId then
			pageId = wrapNonNumericString(pageId)
			if not execId then
				unsupportedCmd('no exec ID found')
				return
			elseif (tonumber(execId) and string.find(execId,'%.')) then
				unsupportedCmd('conflict: exec ID contains page/dot')
			else
				execId = pageId..'.'..execId
			end
		end

		local arg1 = parsedValues[1] or nil
		local arg2 = parsedValues[2] or nil

		local cmdTarget = (matchedFaderKeyword and 'Fader' or 'Executor')..(execId and ' '..execId or '')

		-- master/fader: arg1 -> cue to go to, arg2 -> fade (optional)
		if cmdAction=='go' or cmdAction=='goto' then
			-- alert("matched exec go/goto action")
			local baseCmd = cmdAction..' '..cmdTarget
			if arg1 then
				arg1 = wrapNonNumericString(arg1)
				local optionalFade = getOptionalFadeDuration(arg2)
				runCommand(baseCmd..' Cue '..arg1..optionalFade)
			else
				runCommand(baseCmd)
			end
			return

		-- on/off: arg1 -> fade (optional)
		elseif cmdAction=='top' or cmdAction == 'off' then
			local optionalFade = getOptionalFadeDuration(arg1)
			runCommand(cmdAction..' '..cmdTarget..optionalFade)
			return

		-- master/fader: arg1 -> new value, arg2 -> fade (optional)
		elseif cmdAction=='master' or cmdAction=='fader' then
			-- alert("matched exec master/fader action")
			local targetValue = tonumber(arg1)
			if not targetValue then
				unsupportedCmd('no target value found')
				return
			end
			local optionalFade = getOptionalFadeDuration(arg2)
			runCommand(cmdTarget..' At '..targetValue..optionalFade)
			return
		end
	end

	unsupportedCmd('could not interprete OSC address '..addressData)
end

local function startOSC()
	local inputIp = gma.show.getvar('OSC_INPUT_IP')
	if not inputIp then
		gma.show.setvar('OSC_INPUT_STATE','startupfailure')
		alert('No OSC input IP specified!')
	else
		gma.show.setvar('OSC_INPUT_STATE','running')
		local inputPort = gma.show.getvar('OSC_INPUT_PORT')
		inputPort = math.tointeger(inputPort)
		local inputPort2 = gma.show.getvar('OSC_INPUT_PORT2')
		inputPort2 = math.tointeger(inputPort2)
		plugin.start_listening(inputIp, inputPort, inputPort2)
	end
end
local function endOSC()
	gma.show.setvar('OSC_INPUT_STATE','stopped')
	if plugin.udp then
		plugin.udp:close()
		plugin.udp=nil
		alert('Stopped listening to OSC.')
	end
	if plugin.udp2 then
		plugin.udp2:close()
		plugin.udp2=nil
		alert('Stopped listening to OSC udp fallback port.')
	end
end

return startOSC,endOSC