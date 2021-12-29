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
    void *data;
    mwSize dims[3] = {0,0,0};
    
    if (nrhs != 1)
        mexErrMsgIdAndTxt ("qoidecode:input", "qoidecode requires one input parameter.");
    if (!mxIsClass (prhs[0], "uint8"))
        mexErrMsgIdAndTxt ("qoidecode:data", "Data must be a uint8 array.");
    
    data = qoi_decode (mxGetPr (prhs[0]), mxGetNumberOfElements (prhs[0]), &params, 0);
    
    if (data == NULL)
        mexErrMsgIdAndTxt ("qoidecode:decode", "Error when decoding data.");
    
    plhs[0] = mxCreateNumericArray (3, dims, mxUINT8_CLASS, mxREAL);
    mxSetPr (plhs[0], data);
    dims[0] = params.channels;
    dims[1] = params.width;
    dims[2] = params.height;
    mxSetDimensions (plhs[0], dims, 3);   
}
