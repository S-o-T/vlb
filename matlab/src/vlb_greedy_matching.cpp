/* GREEDYBIPARTITEMATCHING Calc greedy bipartite matching
Syntax:
M = greedyBipartiteMatching(numNodesA,numNodesB,Edges)

Calculates bipartite matching between two sets of nodes {1,numNodesA} and
{1,numNodesB} based on the sorted list of edges.
Edges are defined as n x 2 array where Edges(1,:) = [a b] defines
edge between node a and b. Returns matching M which is array of size
[1,numNodesA] and M(a) = b means that node a was matched to node b. If
node a was not matched, b = 0.

Author: Karel Lenc
*/


#include <mex.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define IS_REAL_2D_FULL_DOUBLE(P) (!mxIsComplex(P) && \
mxGetNumberOfDimensions(P) == 2 && !mxIsSparse(P) && mxIsDouble(P))

#define IS_REAL_SCALAR(P) (IS_REAL_2D_FULL_DOUBLE(P) \
    && mxGetNumberOfElements(P) == 1)

#define NUM_EL1_IN  prhs[0]
#define NUM_EL2_IN  prhs[1]
#define A_IN        prhs[2]
/* Results - matching */
#define RES_ES      plhs[0]
/* Results - second closest features */
#define RES_SC      plhs[1]

void mexFunction(int nlhs,       mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
    int i;
    int numNodesA, numNodesB, numEdges, matchedNodes, maxNumMatches;
    const size_t *dims = mxGetDimensions(A_IN);
    double *startNodes, *endNodes, *matches, *secondClosest;
    int *nodeAAvail, *nodeBAvail;

    if (nrhs > 3) {
        mexErrMsgTxt("Too many input arguments\n");
    } else if (nrhs < 3) {
        mexErrMsgTxt("Too few input arguments\n");
    }

    if(!IS_REAL_SCALAR(NUM_EL1_IN) || !IS_REAL_SCALAR(NUM_EL2_IN)) {
        mexErrMsgTxt("Number of matched elements must be a real scalar number.");
    }

    if(!IS_REAL_2D_FULL_DOUBLE(A_IN)){
        mexErrMsgTxt("Array with edges must be a real 2D full double array.");
    }

    if (dims[1] != 2){
        mexErrMsgTxt("Input array must be n x 2 array.\n");
    }

    numNodesA = (const int)mxGetScalar(NUM_EL1_IN);
    numNodesB = (const int)mxGetScalar(NUM_EL2_IN);
    numEdges = (const int)dims[0];
    startNodes = mxGetPr(A_IN);
    endNodes = startNodes + numEdges;
    /* Prepare the output array for the matches */
    RES_ES = mxCreateDoubleMatrix(2, numNodesA, mxREAL);
    matches = mxGetPr(RES_ES);
    for (i = 0; i < numNodesA*2; ++i) {
        *matches = 0.;
    }

    if (nlhs > 1) {
        /* Prepare the output array for the second closest matches */
        RES_SC = mxCreateDoubleMatrix(2, numNodesA, mxREAL);
        secondClosest = mxGetPr(RES_SC);
        for (i = 0; i < numNodesA*2; ++i) {
            *secondClosest = 0.;
        }
    }

    nodeAAvail = (int*)malloc(numNodesA * sizeof(int));
    nodeBAvail = (int*)malloc(numNodesB * sizeof(int));
    memset(nodeAAvail, 1, numNodesA * sizeof(int));
    memset(nodeBAvail, 1, numNodesB * sizeof(int));

    matchedNodes = 0;
    maxNumMatches = numNodesA < numNodesB ? numNodesA : numNodesB;

    for (i = 0; i < numEdges; ++i) {
        int aIdx = (int)(*startNodes++) - 1;
        int bIdx = (int)(*endNodes++) - 1;
        if (aIdx < 0 || aIdx >= numNodesA || bIdx < 0 || bIdx >= numNodesB) {
            mexPrintf("Invalid Edge from %d to %d.\n",aIdx,bIdx);
            mexErrMsgTxt("Invalid edge.\n");
            return;
        }

        if (nlhs > 1) {
            /* If the feature is matched, but does not have second closest match */
            if (!nodeAAvail[aIdx] && secondClosest[aIdx*2] == 0.) {
                secondClosest[aIdx*2] = (double)(bIdx + 1);
                secondClosest[aIdx*2 + 1] = (double)(i + 1);
            }
        }

        if (nodeAAvail[aIdx] && nodeBAvail[bIdx]){
            /* Save the match */
            matches[aIdx*2] = (double)(bIdx + 1);
            matches[aIdx*2 + 1] = (double)(i + 1);
            nodeAAvail[aIdx] = 0;
            nodeBAvail[bIdx] = 0;
            matchedNodes ++;
            if (matchedNodes >= maxNumMatches){
                break;
            }
        }
    }
    return;
}

//Stupid dummy main function. Otherwise will tigger a linking error on MAC OS
int main()
{
    return 0;
}

