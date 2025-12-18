csvFileName = "Requirements_Capstone.csv";
data = readtable(csvFileName, 'VariableNamingRule', 'preserve');
filename = "CapstoneRequirements";
reqSet = slreq.new(filename);

reqMap = containers.Map('KeyType', 'char', 'ValueType', 'any');

% addAttribute(reqSet, 'Status', 'Edit');
debug = false;

for i = 1:height(data)
    reqID = char(string(data.ID(i)));
    if debug
        disp(reqID)
    end
    reqSummary = char(data.Description(i)); 
    % reqStatus = char(data.Status(i));
    if ismissing(data.Parent(i)) || isempty(char(string(data.Parent(i))))
        parent = reqSet;
    else
        parent = reqMap(char(string(data.Parent(i))));
    end    
    req = add(parent, 'Id', reqID, ...
                      'Summary', reqSummary);
    reqMap(reqID) = req;
end

save(reqSet);
disp('Requirements imported successfully.');