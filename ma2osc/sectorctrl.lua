local messagePrefix = string.char(27)..'[35m'..'sector plugin: '..string.char(27)..'[33m'
local function alert(message)
	gma.feedback(messagePrefix..message)
	gma.echo(messagePrefix..message)
end

local function setFlashFade(group, inOrOut, fadeTime)
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
	-- TODO: implement
end

sector = {
	bpmMaster = 'SpecialMaster 3.1'
}
function sector.setFlashFadeIn(group, fadeTime)
	setFlashFade(group, "in", fadeTime)
end
function sector.setFlashFadeOut(group, fadeTime)
	setFlashFade(group, "out", fadeTime)
end
function sector.resetFlashFading(group)
	if not group then
		group = "all"
	end
	-- TODO: implement reset
end

function sector.setStrobeSpeed(speed)
	if not speed then
		alert("missing speed param value")
		return
	end
	local speedNum = tonumber(speed)
	if speedNum == nil then
		alert("invalid speed param value")
		return
	end
	-- TODO: implement
end
	
function sector.readSpeedMasterBPM(speedmasterhandle)
	-- Get current BPM
	alert('bpm check')
	local speedmasterhandle = gma.show.getobj.handle(sector.bpmMaster)
	local speedmasterState = gma.show.property.get(speedmasterhandle,1) -- percentage of fader (0-100)
	speedmasterState = speedmasterState:gsub('%%','')
	speedmasterState = tonumber(speedmasterState) 
	local speedmasterbpm = 225 * (speedmasterState / 100)
	alert(speedmasterbpm..'bpm')
end

-- function sector.getBPM() 

-- 	local cmd = gma.cmd
-- 	local input = gma.textinput
-- 	local get = gma.show.getobj
-- 	local handle = get.handle
-- 	local property = gma.show.property
-- 	local fdb = gma.feedback
	
-- 	function readSpeedFromFx()
-- 		local effect = input('which effect?','x') --choose effect
-- 		local handle = handle('effect 1.'..effect..'.1') -- handle effect line 1
-- 		local lev1 = property.get(handle,'speed') -- grab value
-- 		gma.feedback('speed = '..lev1) -- show result in CL feedback
-- 	end
-- end