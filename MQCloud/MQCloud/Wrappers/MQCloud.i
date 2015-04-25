// swig.exe -c -csharp -namespace MQCloud -outdir ./Wrappers/DotNet/CSharp -o ./Wrappers/DotNet/CXX/MQCloudWrapper.cxx MQCloud.i

%include "MQCloud.h"
%module(directors="1") MQCloud