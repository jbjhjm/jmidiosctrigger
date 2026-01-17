
local function bpm2hz()
	local bpm = tonumber(gma.textinput("Enter BPM", ""))
	local notefrac = tonumber(gma.textinput("Enter Note fraction, e.g. 16", ""))
	local hz = bpm / 60 * (notefrac/4)
	if hz then
		gma.gui.msgbox('Calculated Frequency', hz..'Hz')
	else
		gma.gui.msgbox('Calculated Frequency', 'failed to calculate')
	end
end
local function endBpm2hz()
	
end

return bpm2hz,endBpm2hz
