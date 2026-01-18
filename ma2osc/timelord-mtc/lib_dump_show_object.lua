--[[

 File: lib_dump_show_object.lua
 Category: Developer Tools 

 Purpose: Dump Show objects (and their children) to the command line in a tree format.

 Usage:
  Call it from other plugins, or it can be called from MA Command Line by... 
    LUA "dump_show_object('')"            <- asks for object name from user
    LUA "dump_show_object('sequence')"    <- dumps the sequence objects
    LUA "dump_show_object('sequence 1')"  <- dumps the sequence 1 object

                                
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
    Private function: dump_object_children(number,string)

    Purpose: Prints details of an object, it's children, and it's grand children, etc...
    This function is recursive, it will call itself in a cascading manner as it 
    discovers children, until all children of children have be accounted for.
    Intended to be initiated by public function dump_show_object()
   
    Input:   handle - the handle to an object, as returned by gma.show.getobj
             indent - a string used to indent our output, starting with "   "

    Returns: nil
--]]

local function dump_object_children(handle,indent)

  local O=gma.show.getobj
  local echo=gma.feedback
  local child_count = 0
  
  for l=0,O.amount(handle) do
    local child_id = O.child(handle,l);
    if (child_id) then
       child_count = child_count + 1;
       echo("");
       echo(indent .. " \\ Child " .. child_count);
       echo(indent .. " :     Class Type: " .. O.class(child_id));
       echo(indent .. " :     Name is:    '" .. O.name(child_id) .. "'");
       if (O.label(child_id)) then
        echo(indent .. " :     Label is:   '" .. O.label(child_id) .. "'");
       end  
       echo(indent .. " :     MA Number:  " .. O.number(child_id));
       echo(indent .. " :     Children:   " .. O.amount(child_id));
       -- function calls itself, increasing the indent every time
       if (O.amount(child_id)) then dump_object_children(child_id, indent .. "   ") end;
    end
  end
end



--[[
    Public function: dump_show_object(string)

    Purpose: Prints details about a show object, and it's children, 
             and it's grand children, etc...  

    Input:   object - Show object, eg 'Timecode' or 'Sequence 1'. 
      
    Returns: nil
--]]
function dump_show_object(object)

   if (object == '') then
    object = gma.textinput('Which object do you want to dump?','Sequence')
   end;

   local O=gma.show.getobj; 
   local H=O.handle(object);
   local echo=gma.feedback

   if(H) then
      echo("Parent '" .. object .. "' exists!");
      echo("  Class  is: " .. O.class(H));
      echo("  Index  is: " .. O.index(H));
      echo("  Number is: " .. O.number(H));
      if (O.label(H)) then
       echo("  Label  is: '" .. O.label(H) .. "'");
      else
       echo("  Name   is: '" .. O.name(H) .. "'");
      end

      if O.amount(H) then    -- if we have kids
       echo("  Children :  " .. O.amount(H));
       dump_object_children(H, "   ") 
      end
   end
end

