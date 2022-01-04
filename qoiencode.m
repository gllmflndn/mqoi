function data = qoiencode (img)
% Quite OK Image (QOI) encoder
% FORMAT data = qoiencode (img)
% img         - M-by-N-by-3 or M-by-N-by-4 uint8 array of RGB or RGBA data
% data        - encoded uint8 vector

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


%-Placeholder (no compression)
data = [uint8('qoif'), typecast(swapbytes(uint32([size(img,2),size(img,3)])),'uint8'), size(img,1), 0, ...
    reshape([repmat(uint8(251+size(img,1)),1,size(img,2)*size(img,3));reshape(img,size(img,1),[])],1,[]), ...
    0, 0, 0, 0, 0, 0, 0, 1];

return;

ch          = size(img,1);
data        = zeros (1, 4+8+2+size(img,2)*size(img,3)*(ch+1)+8, 'uint8');
data(1:14)  = [uint8('qoif'), ...
               typecast(swapbytes(uint32([size(img,2),size(img,3)])),'uint8'), ...
               ch, 0];
rx          = 1;
tx          = 15;
rgba        = uint8 ([0 0 0])';
rgb2idx     = [3 5 7]';
if ch == 4
    rgba    = [rgba; 255];
    rgb2idx = [rgb2idx; 11];
end
array = zeros (ch, 64, 'uint8');
while rx <= size(img,2)*size(img,3)
    rgba(1:ch) = img(:,rx);
    idx = mod (sum (double (rgba) .* rgb2idx), 64);
    if ch == 3, idx = mod (idx + 255*11, 64); end
    array(:, idx+1) = rgba;
    
    data(tx:tx+ch) = [251+ch;rgba(1:ch)]; tx = tx + ch + 1;
    rx = rx + 1;
end
