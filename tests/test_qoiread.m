function test_qoiread (data_dir)
% Test Quite OK Image (QOI) decoder

% mqoi  Copyright (C) 2021  Guillaume Flandin

if ~nargin
    data_dir = pwd;
end

files = dir (fullfile (data_dir, '*.qoi'));
files = files(~[files.isdir]);
for i=1:numel (files)
    fprintf ('Reading "%s"... ',files(i).name);
    try
        IMG1 = qoiread (fullfile (files(i).folder, files(i).name));
        [IMG2, ~, A] = imread (fullfile (files(i).folder, [files(i).name(1:end-3) 'png']));
        if ~isempty (A), IMG2 = cat (3, IMG2, A); end
        if isequal(IMG1,IMG2)
            fprintf ('pass\n');
        else
            fprintf ('fail\n');
        end
    catch
        fprintf ('error\n');
    end
end
