--[[

 File: lib_dump_properties.lua
 Category: Developer Tools 

 Purpose: Dump Properties

 Usage:
    LUA "dump_props(handle)"         
                                
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



--[[
    Private function: dump_props(number)

    Purpose: Prints property names and values for a given handle

    Input:   handle - the handle to a property

    Returns: nil
--]]

function dump_props(handle)
  local O=gma.show.property
  local echo=gma.feedback

 if (O.amount(handle) ~= nil) then
  echo("")
  echo("Properties for Handle ".. handle .. " (Count: " .. O.amount(handle) .. ")" ); 
  for l=0,O.amount(handle) do
      if (O.name(handle,l) ~= nil) then 
       echo(" Property: " .. l); 
       echo("  \\     Name is:    '" .. O.name(handle,l) .. "'");
       if (O.get(handle,l) ~= nil) then
        echo("  \\     Value is:    " .. O.get(handle,l));
       end
      end
  end
 end
end 
