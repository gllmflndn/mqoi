function img = qoidecode (data)
% Quite OK Image (QOI) decoder
% FORMAT img = qoidecode (data)
% data       - uint8 vector to decode
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


rx          = 1;
magic       = char (data(rx:rx+3)'); rx = rx + 4;
if ~strcmp (magic,'qoif'), error ('Invalid QOI file.'); end  % LE platform
sz          = swapbytes (typecast (data(rx:rx+7), 'uint32'))'; rx = rx + 8;
channels    = double (data(rx)); rx = rx + 1;
colorspace  = data(rx); rx = rx + 1;

img         = zeros ([channels prod(sz)], 'uint8');
rgba        = uint8 ([0 0 0])';
rgb2idx     = [3 5 7]';
if channels == 4
    rgba    = [rgba; 255];
    rgb2idx = [rgb2idx; 11];
end
array       = zeros (channels, 64, 'uint8');
tx          = 1;
run         = 0;

while tx <= prod (sz)
    if run
        run = run - 1;
    else
        tag8 = data(rx); rx = rx + 1;
        if tag8 == 254
            % QOI_OP_RGB
            rgba(1:3) = data(rx:rx+2); rx = rx + 3;
        elseif tag8 == 255
            % QOI_OP_RGBA
            rgba = data(rx:rx+3); rx = rx + 4;
        else
            tag2 = bitand (tag8, uint8 (192));
            if tag2 == 0
                % QOI_OP_INDEX
                idx  = bitand (tag8, uint8 (63));
                rgba = array(1:channels, idx+1);
            elseif tag2 == 64
                % QOI_OP_DIFF
                db = int16 (bitand (tag8, uint8 (3))) - 2;
                dg = int16 (bitshift (bitand (tag8, uint8 (12)), -2)) - 2;
                dr = int16 (bitshift (bitand (tag8, uint8 (48)), -4)) - 2;
                rgba(1:3) = mod (int16 (rgba(1:3)) + [dr dg db]', 256);
            elseif tag2 == 128
                % QOI_OP_LUMA
                dg = int16 (bitand (tag8, uint8 (63))) - 32;
                b  = data(rx); rx = rx + 1;
                db = int16 (bitand (b, uint8 (15))) - 8;
                dr = int16 (bitshift (bitand (b, uint8 (240)), -4)) - 8;
                rgba(2) = mod (int16 (rgba(2)) + dg, 256);
                rgba(1) = mod (int16 (rgba(1)) + dg + dr, 256);
                rgba(3) = mod (int16 (rgba(3)) + dg + db, 256);
            elseif tag2 == 192
                % QOI_OP_RUN
                run = bitand (tag8, uint8 (63));
            else
                error ('qoidecode:decode', 'Invalid tag.');
            end
        end
    end
    img(:,tx) = rgba;
    idx = mod (sum (double (rgba) .* rgb2idx), 64);
    if channels == 3, idx = mod (idx + 255*11, 64); end
    array(:, idx+1) = rgba;
    tx = tx + 1;
end

img = reshape (img, [channels sz]);
