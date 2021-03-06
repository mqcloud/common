﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text.RegularExpressions;
using NodesManager;
using ProtoBuf;
using ProtoBuf.Meta;

namespace NodesManager {
    public class SchemaPrinter {
        private static readonly RuntimeTypeModel Model = RuntimeTypeModel.Default;

        private static List<Type> GetTypes() {
            var assembly = Assembly.GetCallingAssembly(); //Assembly.GetAssembly(typeof (ConnectRequest));
            var types = assembly.GetTypes();
            return
                (from t in types
                    let attrs = Attribute.GetCustomAttributes(t)
                    where attrs.Any(a => a is ProtoContractAttribute)
                    select t).ToList();
        }

        private static string PrepareSchema(string schema) {
            var result = schema;

            var matches = Regex.Matches(result, @"message\s+\w+_\w+\s+\{[^}]*\}", RegexOptions.Singleline);
            result = matches.Cast<Match>()
                .Aggregate(result, (current, match)
                    => current.Replace(match.Value, ""));

            matches = Regex.Matches(result, @"enum\s+\w+\s+\{[^}]*\}", RegexOptions.Singleline);
            foreach (Match match in matches) {
                var newmatch = match.Value;
                var names = Regex.Matches(newmatch, @"[a-zA-Z]\w+", RegexOptions.Multiline)
                    .Cast<Match>()
                    .Select(match1 => match1.Value)
                    .ToDictionary(s => s, s => s);
                var enumName = names.ElementAt(1).Value;

                newmatch = names.Skip(2)
                    .Aggregate(newmatch, (current, pair)
                        => Regex.Replace(current,
                            @"(?<=\s)" + names[pair.Key] + @"(?=\s)",
                            enumName + names[pair.Key]));

                result = result.Replace(match.Value, newmatch);

                result = Regex.Replace(result,
                    string.Format(@"(?<=(.*\s+{0}.*\[default\s+=\s+))(?<name1>\w+)", enumName),
                    enumName + "${name1}");
            }

            return result;
        }

        public static string Print() {
            var types = GetTypes();
            types.ForEach(t => Model.Add(t, true));

            var schema = Model.GetSchema(null);
            return PrepareSchema(schema);
        }
    }
}

internal class Program {
    public static void Save(string fileName = "MQCloud.Internal.Protocol.proto") {
        var schema = SchemaPrinter.Print();
        schema = schema.Insert(0, "package MQCloud.Internal.Protocol;\n");

        Console.WriteLine(schema);
        File.WriteAllText(fileName, schema);

        Process.Start(string.Format("{0}/protoc.exe", AppDomain.CurrentDomain.BaseDirectory),
            string.Format("--cpp_out=\"./\" {0}", fileName));
    }

    private static void Main() {
        Save();
    }
}