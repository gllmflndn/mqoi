function test_qoiwrite (data_dir)

if ~nargin
    data_dir = pwd;
end

files = dir (fullfile (data_dir, '*.png'));
files = files(~[files.isdir]);
for i=1:numel (files)
    fprintf ('Writing "%s"... ', files(i).name(1:end-4));
    try
        [IMG1, ~, A] = imread (fullfile (files(i).folder, files(i).name));
        if ~isempty (A), IMG1 = cat (3, IMG1, A); end
        qoifile = [tempname '.qoi'];
        rmFile = onCleanup(@()delete (qoifile));
        qoiwrite (IMG1, qoifile);
        qoistat = dir (qoifile);
        imgstat = whos ('IMG1');
        fprintf ('(raw:%d, png: %d, qoi:%d) ', imgstat.bytes, qoistat.bytes, files(i).bytes);
        IMG2 = qoiread (qoifile);
        if isequal(IMG1,IMG2)
            fprintf ('pass\n');
        else
            fprintf ('fail\n');
        end
    catch
        fprintf ('error\n');
    end
end
