-- gma.cmd('Plugin "dump_object"')
-- gma.cmd('Plugin "dump_props"')

local messagePrefix = string.char(27)..'[35m'..'sectorctrl: '..string.char(27)..'[33m'
local function alert(message)
	gma.feedback(messagePrefix..message)
	gma.echo(messagePrefix..message)
end

local fadeGroups={ 'jbmh', 'mic', 'suns', 'micpixel', 'sunpixel', 'gl-colors' }
local strobeGroups={ 'jbmh', 'mic', 'suns' }

local function setFadeTime(group, isOut, fadeTimeSec)
	-- local fadeTimeNum = tonumber(fadeTime)
	-- if fadeTimeNum == nil then
	-- 	alert("invalid fadeTime param value")
	-- 	return
	-- end

	local execRange = ""
	if group == "jbmh" then
		execRange = isOut and "4.114 thru 4.117" or "4.106 thru 4.109"
	elseif group == "mic" then
		execRange = isOut and "5.114 thru 5.117" or "5.106 thru 5.109"
	elseif group == "suns" then
		execRange = isOut and "3.114 thru 3.117" or "3.106 thru 3.109"
	elseif group == "gl-colors" then
		execRange = "2.161 thru 2.185"
	elseif group == "micpixel" then
		execRange = "5.131 thru 5.146"
	elseif group == "sunpixel" then
		execRange = "3.131 thru 3.170"
	else
		alert('Unknown fadeTime group '..group)
	end

	if isOut then
		gma.cmd("Assign Executor "..execRange.." /OffTime="..fadeTimeSec)
	else
		gma.cmd("Assign Executor "..execRange.." Cue 1 Fade "..fadeTimeSec)
	end
end

sector = {
	bpmMaster = 'SpecialMaster 3.1',
	strobeMaster = 'SpecialMaster 3.3',
	strobeEffect = 'Effect 360 thru 361',
	strobeWidthMs = 30,
}

-- NOTE: fadeTime sent via OSC is not premultiplied! range 0-100!
-- fadeTime - 1 because velocity min value is 1!
function sector.setFadeInTime(group, fadeTime)
	fadeTime = fadeTime 
	    and (fadeTime-1)*0.05 
		or ((group == 'micpixel' or group == 'sunpixel' or group == 'gl-colors') and 0 or 0.5)
	setFadeTime(group, false, fadeTime)
end
function sector.setFadeOutTime(group, fadeTime)
	fadeTime = fadeTime 
	    and (fadeTime-1)*0.05 
		or ((group == 'micpixel' or group == 'sunpixel' or group == 'gl-colors') and 0 or 0.6)
	setFadeTime(group, true, fadeTime)
end
function sector.resetFlashFading(group)
	if not group then
		for _, name in pairs(fadeGroups) do
			sector.setFadeInTime(name)
			sector.setFadeOutTime(name)
		end
	else
		sector.setFadeInTime(group)
		sector.setFadeOutTime(group)
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

function sector.setStrobeType(group, setWhite)
	-- local fadeTimeNum = tonumber(fadeTime)
	-- if fadeTimeNum == nil then
	-- 	alert("invalid fadeTime param value")
	-- 	return
	-- end

	local execRange = ""
	if group == "jbmh" then
		local startSeq = setWhite and 604 or 600
		gma.cmd("Assign Sequence "..(startSeq+0).." At Executor 4.118")
		gma.cmd("Assign Sequence "..(startSeq+1).." At Executor 4.119")
		gma.cmd("Assign Sequence "..(startSeq+2).." At Executor 4.120")
		gma.cmd("Assign Sequence "..(startSeq+3).." At Executor 4.121")
	elseif group == "mic" then
		local startSeq = setWhite and 609 or 608
		gma.cmd("Assign Sequence "..(startSeq+0).." At Executor 5.118")
	elseif group == "suns" then
		-- ignore, suns have no colors
	else
		alert('Unknown strobeGroup '..group)
	end

end

function sector.resetAll()
	alert('resetAll - strobeSpeed 4')
	sector.setStrobeSpeed(4.0)
	alert('resetAll - flash fade times')
	sector.resetFlashFading()
	alert('resetAll - strobe to dim mode')
	sector.setStrobeType('jbmh',false)
	sector.setStrobeType('mic',false)
	alert('resetAll - reset Song Execs')
	sector.resetSongExec()
	alert('resetAll - complete')
end

function sector.resetSongExec()
	-- cmd does not allow Faders to be set across multiple pages.
	-- they could be off'd using "Off Page 15 Thru 44 Fader 1 Thru".
	-- but off'd faders will only be enabled again after hitting 0, it seems.
	-- so we automate it for each song page.
	for page=15, 44 do
		gma.cmd('Fader '..page..'.1 Thru '..page..'.15 At 0')
	end
	
end

	-- dump_props(gma.show.getobj.handle("SpecialMaster 3.1"))
	-- dump_props(gma.show.getobj.handle("Executor 1.6"))
