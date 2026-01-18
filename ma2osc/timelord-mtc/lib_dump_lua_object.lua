--[[

 File: lib_dump_lua_object.lua
 Category: Developer Tools 

 Purpose: Dump Lua objects (and their children) to the command line in a tree format.

 Usage:
   Can be called from MA Command Line by... 
    LUA "dump_object()"              <- dump all objects
    LUA "dump_object(gma)"           <- dumps the objects under gma

                                
 Script by:                                                               
        .        .--._________   _...._    _________   _...._                    
      .':        :__:\        :.'      '-. \        :.'      '-. .-.          .-  '2015
     <  :        .--. \        .'```'.    '.\        .'```'.    '.\ \        / / 
      : :        :  :  \      :       \     \\      :       \     \\ \      / /  
      : : .'''-. :  :   :     :        :    : :     :        :    : \ \    / /   
      : :/.'''. \:  :   :      \      /    .  :      \      /    .   \ \  / /    
      :  /    : ::  :   :     :\`'-.-'   .'   :     :\`'-.-'   .'     \ `  /     
      : :     : ::__:   :     : '-....-'`     :     : '-....-'`        \  /      
      : :     : :      .'     '.             .'     '.                 / /       
      : '.    : '.   '-----------'         '-----------'           :`-' /        
      '---'   '---'                                                 '..'         
                       ..::[ http://timelord-mtc.com/LUA ]::..                                                           
--]]


local seen={}

--[[
    Public function: dump_object(object,string)

    Purpose: Prints details about a Lua object, and it's children, 
             and it's grand children, etc...  

    Input:   object - eg 'gma' or 'math'. If not supplied, all objects.
      
    Returns: nil
--]]
function dump_object(object,indent)
  local F=gma.feedback

  if (object == nil) then object = _G end;
  if (indent == nil) then indent = '' end;
  if (object) then
   seen[object]=true;
   local s={}
   local n=0
   for k in pairs(object) do
    n=n+1 s[n]=k
   end
    table.sort(s)
   for k,v in ipairs(s) do
    F(indent,v)
    v=object[v]
    if type(v)=="table" and not seen[v] then dump_object(v,indent .. " \\ ") end;
   end
  end
end


return gma.feedback("Lua Object Dumper Loaded.")
