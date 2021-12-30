/*
 mqoi  Copyright (C) 2021  Guillaume Flandin

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <string.h>

#include "mex.h"

#define QOI_IMPLEMENTATION
#define QOI_NO_STDIO
#define QOI_MALLOC mxMalloc
#define QOI_FREE mxFree
#include "qoi.h"

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    qoi_desc params;
    int out_len;
    void *data;
    mwSize ndims;
    const mwSize *dims;
    
    if (nrhs != 1)
        mexErrMsgIdAndTxt ("qoiencode:inputs", "qoiencode requires one input parameter.");
    if (!mxIsClass (prhs[0], "uint8"))
        mexErrMsgIdAndTxt ("qoiencode:image", "Image must be a uint8 array.");
    ndims = mxGetNumberOfDimensions (prhs[0]);
    dims = mxGetDimensions (prhs[0]);
    if (ndims != 3 || (dims[0] != 3 && dims[0] != 4))
        mexErrMsgIdAndTxt ("qoiencode:dimensions", "Input image must be MxNx3 or MxNx4.");
    
    dims = mxGetDimensions (prhs[0]);
    params.width = dims[1];
	params.height = dims[2];
	params.channels = dims[0];
	params.colorspace = QOI_SRGB;

    data = qoi_encode(mxGetPr (prhs[0]), &params, &out_len);
    
    if (data == NULL)
        mexErrMsgIdAndTxt ("qoiencode:encode", "Error when encoding data.");
        
    plhs[0] = mxCreateNumericMatrix (0, 0, mxUINT8_CLASS, mxREAL);
    mxSetPr (plhs[0], data);
    mxSetM (plhs[0], 1);
    mxSetN (plhs[0], out_len);
}
