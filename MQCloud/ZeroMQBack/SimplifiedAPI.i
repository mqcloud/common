// External ZeroMQ interface. Client Use via CreateContext
// swig.exe -c -csharp -namespace ZeroMQBackEnd -I../MQCloud/include/ -outdir ./Sharp/ -o ./SimpleAPIWrapperSharp.cxx SimplifiedAPI.i
// swig.exe -c -java -I../MQCloud/include/ -outdir ./Java/ -o ./SimpleAPIWrapperJava.cxx SimplifiedAPI.i
// swig.exe -c -python -I../MQCloud/include/ -outdir ./Python/ -o ./SimpleAPIWrapperPython.cxx SimplifiedAPI.i

%include "SimplifiedAPI.h"
%module(directors="1") ZMQBackEnds