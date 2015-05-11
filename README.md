common
======
Core library components - Protocol, C API, C++ API, Implementation

How can YOU help us?
======
 - Provide low-level back ends
   - Erlang
   - Kernel bypass (Direct Access Transport Libraries for Windows and Linux, DAPL-ND, DAPL, vendor-specific...)
 - Provide Tests for existing Front-Ends
 - Provide new Front-Ends
 - Emscripten based JS library (WebSockets oriented)
 - Performance tests on your hardware
 - Stories describing practical usage (HPC, Traiding, MMOs...)
 - Create new backends

Core Back-End
======
Wraps a transport for use in MQCloud Core. 
Provides simple API for end user: one BackEnd structure getter. That BackEnd shall be passed into User Space API CreateContext.

Publicly available Back-End must provide at least one BackEnd structure getter and its C#, Java, Python bindings. More settings can be added depending on back-end requirements.

We Provide a few basic back-ends and hope you will help us to create more!)

Back-End roadmap
	- ZeroMQ (with TCP; reliable multicast transport using PGM; inter-process communication transport and inter-thread communication transport)
	- RabbitMQ
	- uDAPL (WinOF)
	- WebSockets
