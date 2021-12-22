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
    
    filename = mxArrayToString (prhs[0]);
    data = qoi_read (filename, &params, 0);
    mxFree (filename);
    
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
