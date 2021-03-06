function img = qoiread (filename)
% Quite OK Image (QOI) reader
% FORMAT img = qoiread (filename)
% filename   - character vector specifying the file to read
% img        - M-by-N-by-3 or M-by-N-by-4 uint8 array of RGB or RGBA data

% mqoi  Copyright (C) 2021  Guillaume Flandin

% This program is free software: you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation, either version 2 of the License, or
% (at your option) any later version.
% 
% This program is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
% 
% You should have received a copy of the GNU General Public License
% along with this program.  If not, see <https://www.gnu.org/licenses/>.


fid = fopen (filename, 'rb');
if fid == -1
    error ('Cannot open file "%s".', filename);
end
data = fread (fid, '*uint8');
fclose (fid);

img = qoidecode (data);

img = permute (img, [3 2 1]);
