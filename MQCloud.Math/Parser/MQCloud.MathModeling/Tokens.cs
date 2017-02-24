using System;
using System.Collections.Generic;

namespace MQCloud.MathModeling {
    public class Token {
        public Type Type;
        public static List<Token> Empty = new List<Token>();

        Token paret;
        List<Token> children;
        public Token() { }
    }

    public class Event : Token {}
    public class Topik: Token { }

    public class ResponseRayTrace: Token { }

    public class RequestRayTrace: Token { }

    internal class ResponsePath: Request { }

    internal class RequestPath: Request { }

    public class Id: Token { }

    public class Message: Token { }

    public class Request: Token { }

    public class Response: Token { }

    public class Kbps: Token { }

    public class Load: Token { }

    public class Scedule: Token { }

    public class Scedule<T>: Scedule { }
}