/* File: example.i */
%module hoot
%{
#include <vector>
%}

%include "std_string.i"
%include "std_vector.i"

%include "PbfInputFormat.i"
%include "PbfInputSplit.i"

