-- --*******************************************************
-- LUA-Script by Dominik Herderich**************************
-- --*******************************************************


-- *********************************************************
-- ********************* Changelog: ************************
-- Version v1.0 ********************************************
-- Delete all unassinged Sequences
-- *********************************************************

local intern_name    = select(1,...);
gma.feedback("Hello, you loaded the Sequence-Pool-Cleaner-plugin at Slot " .. intern_name);


--**********************************************************
-- Create shortcuts and variables **************************
--**********************************************************
local Feedback          = gma.feedback;
local Echo              = gma.echo;
local Command           = gma.cmd;
local Sleep             = gma.sleep;
local Grab              = gma.show.getobj;
local Input             = gma.textinput;
local GetVar            = gma.show.getvar;
local Progress          = gma.gui.progress;
local Debug             = 0;                                -- For Debug-Feedback set to 1; No Debug-Feedback any other value


--**********************************************************
-- Get Basics for creating the timecode show and sequence **
--**********************************************************
function SeqPoolCleaner_Basics()
    -- find the last sequence object in the pool
    local progress_start = Progress.start("Find last Sequence");
    for i=9999, 1, -1 do
        --Progress-Bar
        if(i == 9999) then
            Progress.setrange(progress_start, 1, 9999);
        elseif(i == 1) then
            Progress.stop(progress_start);
        else
            Progress.set(progress_start, i);
        end
        
        ifdev(i, "Seq Counter: ")
        SeqHandle =    Grab.handle("Sequence "..i);
        if(SeqHandle) then
            break
        end
    end
    Progress.stop(progress_start);
    
    -- find the last used page
    local progress_start = Progress.start("Find last Page");
    for i=9999, 1, -1 do
        --Progress-Bar
        if(i == 9999) then
            Progress.setrange(progress_start, 1, 9999);
        elseif(i == 1) then
            Progress.stop(progress_start);
        else
            Progress.set(progress_start, i);
        end

        ifdev(i, "Page Counter: ")
        PageHandle =    Grab.handle("Page "..i);
        if(PageHandle) then
            break
        end
    end
    Progress.stop(progress_start);


    -- Index is starting at 0, therefore Index + 1 to get the correct pool object number
    LastSeqNumber = Grab.index(SeqHandle) + 1;
    LastSeqHandle = SeqHandle;
    Feedback("Last used sequence: Sequence "..LastSeqNumber);
    ifdev(LastSeqHandle, "Handle of last used sequence");

    LastPageNumber = Grab.index(PageHandle) + 1;
    LastPageHandle = PageHandle;
    Feedback("Last used page: Page "..LastPageNumber);
    ifdev(LastPageHandle, "Handle of last used page");
        
    -- return the last sequence informations to the cleaner function
    return LastSeqNumber, LastPageNumber;
end

--**********************************************************
-- Clean up the Sequence Pool **********************************************
--**********************************************************
function SeqPoolCleaner(LastSeqNumber, LastPageNumber)
    --iterate through the sequence pool until the last used sequence object is reached and cleanup
    for Counter=1, LastSeqNumber, 1 do
        Handle = Grab.handle("Sequence "..Counter);
        if(Handle) then
            SeqName = Grab.label(Handle);
            -- make safe against empty labels
            if(SeqName) then
                Feedback("Sequence "..Counter.." exists.");
            else
                Feedback("Sequence "..Counter.." is empty.");
            end
            
            --set AssignmentCounter variable to have an overview how often a sequence is assigned.
            local AssignmentCounter = 0;
            ifdev(AssignmentCounter, "AssignmentCounter: ");
            Sleep(0.1);

            --go through the execs to find an exec, that has the name of the actual sequence
            for Page =0, LastPageNumber ,1 do
                for Executor = 0, 220, 1 do
                    ExecHandle = Grab.handle("Executor "..Page.."."..Executor);
                    if(ExecHandle) then
                        ExecName = Grab.label(ExecHandle);
                        ifdev(ExecHandle, "Actual ExecHandle: ");
                        ifdev(ExecName, "Actual ExecName: ");
                        --check if ExecName and SeqName are the same. If yes, the Sequence is assigned to an Exec and can't be deleted later. The AssignmentCounter will be increased in this case.
                        if(ExecName == SeqName) then
                            Feedback("Sequence is assigned at executor "..Page.."."..Executor..".");
                            AssignmentCounter = AssignmentCounter + 1;
                            ifdev(AssignmentCounter, "AssignmentCounter increased: ");
                        end
                    end
                end
            end

            --Delete now the sequence, if AssignmentCounter = 0
            if (AssignmentCounter == 0) then
                Command("Delete Sequence "..Counter);
                Feedback("DELETED SEQUENCE "..Counter);
            elseif (AssignmentCounter > 0) then
                Feedback("Sequence "..Counter.." is used "..AssignmentCounter.."x on different executors.");
            end
        end
    end

end


--**********************************************************
-- Debug-Function ******************************************
--**********************************************************
function ifdev(var, name)
    if(Debug == 1)then
        Feedback("###Debug### "..name..":"..var.." ###Debug###");
     end
end


--**********************************************************
-- CleanUp-Function ****************************************
--**********************************************************
function SeqPoolCleaner_CleanUp()
    Feedback("Deleting all unassigned Sequences is finished.");
end


--**********************************************************
-- Execute all Functions ***********************************
--**********************************************************
function SeqPoolCleaner_Start()
    SeqPoolCleaner_Basics();
    SeqPoolCleaner(LastSeqNumber, LastPageNumber);
    SeqPoolCleaner_CleanUp();
end


--**********************************************************
-- Execute Start-Function **********************************
--**********************************************************
return SeqPoolCleaner_Start;