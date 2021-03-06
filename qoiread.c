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
#define QOI_MALLOC mxMalloc
#define QOI_FREE mxFree
#include "qoi.h"

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    qoi_desc params;
    char *filename;
    void *data;
    mxArray *mx[2];
    mwSize dims[3] = {0,0,0};
    
    if (nrhs != 1)
        mexErrMsgIdAndTxt ("qoiread:input", "qoiread requires one input parameter.");
    if (!mxIsChar (prhs[0]))
        mexErrMsgIdAndTxt ("qoiread:filename", "Filename must be a string.");
    
    filename = mxArrayToString (prhs[0]);
    data = qoi_read (filename, &params, 0);
    mxFree (filename);
    
    if (data == NULL)
        mexErrMsgIdAndTxt ("qoiread:read", "Error when reading and decoding file.");
    
    plhs[0] = mxCreateNumericArray (3, dims, mxUINT8_CLASS, mxREAL);
    mxSetPr (plhs[0], data);
    dims[0] = params.channels;
    dims[1] = params.width;
    dims[2] = params.height;
    mxSetDimensions (plhs[0], dims, 3);

    mx[0] = plhs[0];
    mx[1] = mxCreateNumericMatrix (1, 3, mxDOUBLE_CLASS, mxREAL);
    mxGetPr (mx[1])[0] = 3;
    mxGetPr (mx[1])[1] = 2;
    mxGetPr (mx[1])[2] = 1;
    mexCallMATLAB (1, plhs, 2, mx, "permute");
    mxDestroyArray (mx[1]);
}
