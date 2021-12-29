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
    mwSize ndims;
    const mwSize *dims;
    mxArray *mx[2];
    mxArray *data;
    
    if (nrhs != 2)
        mexErrMsgIdAndTxt ("qoiwrite:inputs", "qoiwrite requires two input parameters.");
    if (!mxIsClass (prhs[0], "uint8"))
        mexErrMsgIdAndTxt ("qoiwrite:image", "Image must be a uint8 array.");
    if (!mxIsChar (prhs[1]))
        mexErrMsgIdAndTxt ("qoiwrite:filename", "Filename must be a string.");
    
    ndims = mxGetNumberOfDimensions (prhs[0]);
    dims = mxGetDimensions (prhs[0]);
    if (ndims != 3 || (dims[2] != 3 && dims[2] != 4))
        mexErrMsgIdAndTxt ("qoiwrite:dimensions", "Input image must be mxnx3 or mxnx4.");
    
    params.width = dims[1];
	params.height = dims[0];
	params.channels = dims[2];
	params.colorspace = QOI_SRGB;
    
    mx[0] = (mxArray *)prhs[0];
    mx[1] = mxCreateNumericMatrix (1, 3, mxDOUBLE_CLASS, mxREAL);
    mxGetPr (mx[1])[0] = 3;
    mxGetPr (mx[1])[1] = 2;
    mxGetPr (mx[1])[2] = 1;
    mexCallMATLAB (1, &data, 2, mx, "permute");
    mxDestroyArray (mx[1]);
    
    filename = mxArrayToString (prhs[1]);
    qoi_write (filename, mxGetPr (data), &params);
    mxDestroyArray (data);
    mxFree (filename);
}
