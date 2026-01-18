-- gma.cmd('Plugin "dump_object"')
-- gma.cmd('Plugin "dump_props"')

local messagePrefix = string.char(27)..'[35m'..'sector plugin: '..string.char(27)..'[33m'
local function alert(message)
	gma.feedback(messagePrefix..message)
	gma.echo(messagePrefix..message)
end

local knownGroups={ 'jbmh', 'mic', 'suns', 'micpixel', 'sunpixel' }

local function setFlashFade(group, isOut, fadeTime)
	-- NOTE: fadeTime is not premultiplied! range 0-100!
	if not group then
		alert("no group param given")
		return
	end
	if not fadeTime then
		alert("no fadeTime param given")
		return
	end
	local fadeTimeNum = tonumber(fadeTime)
	if fadeTimeNum == nil then
		alert("invalid fadeTime param value")
		return
	end

	local execRange = ""
	if group == "jbmh" then
		execRange = ""
	elseif group == "mic" then
		execRange = ""
	elseif group == "suns" then
		execRange = ""
	elseif group == "micpixel" then
		execRange = ""
	elseif group == "sunpixel" then
		execRange = ""
	end

	if isOut then
		gma.cmd("Assign Executor "..execRange.." /OffTime="..fadeTime)
	else
		gma.cmd("Assign Executor "..execRange.." Cue 1 Fade "..fadeTime)
	end
end

sector = {
	bpmMaster = 'SpecialMaster 3.1',
	strobeMaster = 'SpecialMaster 3.3',
	strobeEffect = 'Effect 360 thru 361',
	strobeWidthMs = 30,
}
function sector.setFlashFadeIn(group, fadeTime)
	setFlashFade(group, "in", fadeTime)
end
function sector.setFlashFadeOut(group, fadeTime)
	setFlashFade(group, "out", fadeTime)
end
function sector.resetFlashFading(group)
	if not group then
		for _, name in knownGroups do
			setFlashFade(name, false, 0.5)
			setFlashFade(name, true, 0.6)
		end
	else
		setFlashFade(group, false, 0.5)
		setFlashFade(group, true, 0.6)
	end
end

function sector.calcBpm2Hz()
    local bpm = tonumber(gma.textinput("Enter BPM", ""))
    local notefrac = tonumber(gma.textinput("Enter Note fraction, e.g. 16", ""))
    local hz = bpm / 60 * (notefrac/4)
    if hz then
        gma.gui.msgbox('Calculated Frequency', hz..'Hz')
    else
        gma.gui.msgbox('Calculated Frequency', 'failed to calculate')
    end
end
	
function sector.readSpeedMasterBPM(speedmasterhandle)
	local speedmasterhandle = gma.show.getobj.handle(sector.bpmMaster)
	local speedmasterState = gma.show.property.get(speedmasterhandle,1) -- percentage of fader (0-100)
	speedmasterState = speedmasterState:gsub('%%','')
	speedmasterState = tonumber(speedmasterState) 
	local speedmasterbpm = 225 * (speedmasterState / 100)
	return speedmasterbpm
end

function sector.setStrobeSpeed(multiplier)
	-- bpm * rate multiplication works only for effects directly assigned to an executor (no cue).
	-- so only selective effects will work. Set the effect speed group to rate and the speed master in executor options to bpm.
	if not multiplier then
		alert("missing speed param value")
		return
	end
	local speedNum = tonumber(multiplier)
	if speedNum == nil then
		alert("invalid speed param value")
		return
	end
	-- Sunstrips have no Strobe channel
	-- Mic has no Strobe channel
	-- JBMH have very limited Strobe channel
	local refSpeed = sector.readSpeedMasterBPM(sector.bpmMaster)
	local strobeSpeed = refSpeed * multiplier
	alert("strobeSpeed in bpm = "..strobeSpeed)

	gma.cmd(sector.strobeMaster.." At "..(strobeSpeed/4))

	local strobeIntervalMs = 1000 / (strobeSpeed / 60)
	local pulseWidth = sector.strobeWidthMs / strobeIntervalMs * 100.0
	alert("strobeInterval in ms = "..strobeIntervalMs)
	alert("pulseWidth for 100ms light in % = "..pulseWidth)

	-- template effects will not be stored as reference in cue
	-- only when using selective effects, updating the effect will change the result.
	gma.cmd("Assign "..sector.strobeEffect.." /width="..pulseWidth)

end

function sector.resetAll()
	sector.setStrobeSpeed(4.0)
	sector.resetFlashFading()
end

	-- dump_props(gma.show.getobj.handle("SpecialMaster 3.1"))
	-- dump_props(gma.show.getobj.handle("Executor 1.6"))
