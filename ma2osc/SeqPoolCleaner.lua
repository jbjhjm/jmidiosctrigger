-- --*******************************************************
-- LUA-Script by Dominik Herderich**************************
-- Rewritten to report instead of immediately deleting *****
-- by Jannik Mewes  ****************************************
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
local Delete = false -- if false, a report will be created. if true, unused seqs will immediately be deleted.
local MaxExec = 220; -- to check ALL, this must be 220

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
	local unusedSeqIds = {}

    for Counter=1, LastSeqNumber, 1 do
        Handle = Grab.handle("Sequence "..Counter);
        if(Handle) then
            SeqName = Grab.label(Handle);
            -- make safe against empty labels
            if(SeqName) then
                Feedback("Sequence "..Counter.." ("..SeqName..") exists.");
            else
                Feedback("Sequence "..Counter.." is empty.");
            end

			SeqName = SeqName or 'unnamed';
            
            --set AssignmentCounter variable to have an overview how often a sequence is assigned.
            local AssignmentCounter = 0;
            ifdev(AssignmentCounter, "AssignmentCounter: ");
            Sleep(0.03);

            --go through the execs to find an exec, that has the name of the actual sequence
            for Page =0, LastPageNumber ,1 do
                for Executor = 0, MaxExec, 1 do
                    ExecHandle = Grab.handle("Executor "..Page.."."..Executor);
                    if(ExecHandle) then
                        ExecName = Grab.label(ExecHandle);
                        -- ifdev(ExecHandle, "Actual ExecHandle: ");
                        -- ifdev(ExecName, "Actual ExecName: ");
                        --check if ExecName and SeqName are the same. If yes, the Sequence is assigned to an Exec and can't be deleted later. The AssignmentCounter will be increased in this case.
                        if(ExecName == SeqName) then
                            -- Feedback("Sequence is assigned at executor "..Page.."."..Executor..".");
                            AssignmentCounter = AssignmentCounter + 1;
                            -- ifdev(AssignmentCounter, "AssignmentCounter increased: ");
                        end
                    end
                end
            end

            --Delete now the sequence, if AssignmentCounter = 0
            if (AssignmentCounter == 0) then
                Feedback("Sequence "..Counter.." ("..SeqName..") is unused.");
				if Delete then
                	Command("Delete Sequence "..Counter);
				else 
               	 table.insert(unusedSeqIds, Counter);
				end
            elseif (AssignmentCounter > 0) then
                Feedback("Sequence "..Counter.." ("..SeqName..") is used "..AssignmentCounter.."x on different executors.");
            end
        end
    end

    Feedback("Unused sequences: "..(#unusedSeqIds));

	if not Delete then 
		local currIndex = 1; -- LUA is not zero based!!!!!!!!!!!!!
		local nextIndex = 1;
		local a, b;
		local ranges = {}
		while currIndex < #unusedSeqIds do
			-- Feedback("Start at offset "..currIndex);
			nextIndex = currIndex+1;
			while nextIndex < #unusedSeqIds - 1 do
				-- Feedback("start sub loop at "..nextIndex);
				a = unusedSeqIds[nextIndex - 1];
				-- Feedback("scan next seq, a= "..a);
				b = unusedSeqIds[nextIndex];
				-- Feedback("scan next seq, b= "..b);
				if (a+1 == b) then
					-- Feedback("Increase subindex cause "..a.." + 1 == "..b);
					nextIndex = nextIndex + 1;
				else
					break
				end
			end
			-- Feedback("Report");
			if nextIndex - 1 > currIndex then
				table.insert(ranges, ""..unusedSeqIds[currIndex].." thru "..unusedSeqIds[nextIndex-1]);
			else
				table.insert(ranges, ""..unusedSeqIds[nextIndex-1]);
			end
		
			currIndex = nextIndex;
		end
		
		Feedback(table.concat(ranges,' + '));
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