-- *********************************************
-- OSC_Mate for GrandMA2
-- by vldurnov.com
-- support@vldurnov.com for any questions
-- *********************************************


local e=string.char(27)..'[35m'..'OSC_Mate: '..string.char(27)..'[33m'
local function a(n)
	gma.feedback(e..n);
	gma.echo(e..n)
end
local function r(e)
	e=e:gsub('[|\\^]','');
	return gma.cmd(e)
end
local o=require('socket/socket')
local e="#bundle"
local f='i'
local h='f'
local u='s'
local c='T'
local s='F'
local m='I'
local g='N'
local function d(e,n)
	local t,l,e,o=e:byte(n,n+3)
	local e=t*16777216+l*65536+e*256+o
	if e>2147483647 then
		e=e-4294967296
	end
	return e,n+4
end
local function b(e,n)
	local e=string.unpack(">f",e:sub(n,n+3))e=math.floor(e*1000000+.5)/1000000
	return e,n+4
end
local function i(t,n)
	local e=t:find("\0",n)
	local n=t:sub(n,e-1)
	return n,e+(4-((e-1)%4))
end
local n={receive_IP='0.0.0.0',receive_every=.1,}
function n.start_listening(t)
	n.udp=assert(o.udp(),"Failed to create UDP socket")
	n.udp:settimeout(0)
	n.udp:setsockname(n.receive_IP,t)
	a('Listening for OSC messages on port '..t..'. To change the listening port, use the command: "Plugin OSC_Mate_INPUT port_number".')
	while true do
	repeat
		local t=n.udp:receive()
		if t then
			if t:sub(1,#e)==e then
				n.handle_bundle(t)
			else
				n.handle_single_message(t)
			end
		end
	until not t
	gma.sleep(n.receive_every)
	end
end
function n.handle_bundle(t)local o=#e+9
	while o<#t do
		local a,l=d(t,o)
		local t=t:sub(l,l+a-1)
		if t:sub(1,#e)==e then
			n.handle_bundle(t)
		else
			n.handle_single_message(t)
		end
		o=l+a
	end
end
function n.handle_single_message(o)
	a('handle_single_message: '..o)

	
	local _,e=i(o,1)
	a('handle_single_message - address: '.._)
	local t
	t,e=i(o,e)
	t=t:sub(2)
	a('handle_single_message - chunk: '..t)
	local l={}
	for r=1,#t do
		local n=t:sub(r,r)
		local t
		if n==f then
			t,e=d(o,e)
		elseif n==h then
			t,e=b(o,e)
		elseif n==u then
			t,e=i(o,e)
		elseif n==m then
			local n
			n,e=d(o,e)t=(n~=0)
		elseif n==c then
			t=true
		elseif n==s then
			t=false
		elseif n==g then
			t=0
		else
			a('Unsupported OSC type: '..n)
			return
		end
		table.insert(l,t)
	end
	n.handle_message(_,l)
end
local function t(n,e)
	gma.show.setvar('OSC_SEND_IP_PORT',n..':'..e)
	a('Set sending IP: '..n..', port: '..e)
end
function n.handle_message(n,l)
	local e=''
	for t,n in ipairs(l)do
		e=e..tostring(n)..','
	end
	e=e:gsub(',$','')
	a('Received OSC: '..n..' '..e)
	local function d()
		a('Unsupported or wrong command')
	end

	local e={}
	for n in(n:lower()..'/'):gmatch("(.-)/")do
		if n~=''then
			table.insert(e,n)
		end
	end
	if e[1]=='oscmate' then
		if e[2]=='set_destination_ip' then
			local e=e[3] or l[1]
			if e then
				local e,n=e:match('^(%d+%.%d+%.%d+%.%d+):(%d+)')
				if e and n then
					t(e,n)
					return
				end
			end
			d()
			return
		else
			table.remove(e,1)
		end
	end
	if e[1]=='gma2' then
		table.remove(e,1)
	end
	if#e==0 then
		d()return
	end
	if e[1]=='cmd' then
		local e=e[2] or l[1]
		if e then
			r(e)
			return
		end
	elseif e[1]:match('^macro') then
		local e=e[1]:match('^macro(%d+)') or e[2] or l[1]
		if e then
			r('Macro '..(tonumber(e)or'"'..e..'"'))
			return
		end
	else
		local n,t
		if e[1]:match('^page') then
			n=e[1]:match('^page(%d+)')
			if not n then
				n=e[2]
				if n then
					table.remove(e,2)
				else
					n=l[1]
					if not n then
						d()
						return
					end
				end
			end
			if#e==1 then
				r('Page '..(math.tointeger(n)or'"'..n..'"'))
				return
			else
				table.remove(e,1)
			end
		end
		local i
		if e[1]:match('^fader') then
			e[1]=e[1]:gsub('^fader','executor')i=true
		end
		if e[1]:match('^executor') then
			local function a(e)
				if e=='go' or e=='goto' or e=='top' or e=='off' or e=='master' or e=='fader' then
					return true
				end
			end
			t=e[1]:match('^executor(.+)')
			if not t then
				if not a(e[2])then
					t=e[2]
					table.remove(e,2)
				end
			end
			if t and not tonumber(t) then
				t='"'..t..'"'
			end
			if n and not math.tointeger(n) then
				n='"'..n..'"'
			end
			local o
			if a(e[2]) then
				o=e[2]table.remove(e,2)
			else
				o='master'
			end
			if n then
				if not t or (tonumber(t) and t:find('%.')) then
					d()
					return
				else
					t=n..'.'..t
				end
			end
			local n,a

			if e[2] then
				n=e[2]a=e[3] or l[1]
			else
				n=l[1]a=l[2]
			end
			local e=(i and'Fader'or'Executor')..(t and' '..t or'')
			if o=='go' or o=='goto' then
				local t=n and (tonumber(n)or'"'..n..'"')
				local n=tonumber(a)
				r(o..' '..e..(t and' Cue '..t..(n and' Fade '..n or'')or''))
				return
			elseif o=='top'or o=='off' then
				local n=tonumber(n)r(o..' '..e..(n and' Fade '..n or''))
				return
			elseif o=='master'or o=='fader' then
				local n=tonumber(n)
				local t=tonumber(a)
				if not n then
					d()
					return
				end
				r(e..' At '..n..(t and' Fade '..t or''))
				return
			end
		end
	end
	d()
end

local function t(e)
	gma.show.setvar('OSC_INPUT_PORT',e)
	a('Set listening port: '..e)
end

local function o(e)
	if e then
		e=math.tointeger(e)
		if e then
			t(e)
		end
	else
		local e=gma.show.getvar('OSC_INPUT_PORT')
		if not e then
			local n=gma.show.getvar('OSC_SEND_IP_PORT')
			if n then
				e=n:match('[^:]+:(%d+)')
			end
		end
		e=math.tointeger(e)or 8000
		n.start_listening(e)
	end
end

local function e()
	if n.udp then
		n.udp:close()
		n.udp=nil
		a('Stopped listening to OSC. Thank you for using OSC_Mate! For more plugins and updates, visit www.vldurnov.com')
	end
end
return o,e