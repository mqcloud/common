using System;
using System.IO;
using System.Linq;
using Graphviz4Net.Dot.AntlrParser;
using MQCloud.MathModeling.Demo.Logic;
using MQCloud.MathModeling.Immitation.Runtime;

namespace MQCloud.MathModeling {
    class Program {
        static void Main(string[] args) {
            var attakLogic = new AttakLogic();
            attakLogic.ApproachTarget();
            attakLogic.AttakTarget();
            Simulation.Run();
        }

        /*
            var parser = AntlrParserAdapter<string>.GetParser();

            var f = "a->b";//File.ReadAllText("C:\\Users\\Oleg\\Desktop\\Диплом\\test-out.dot");
            var result = parser.Parse(f);
            foreach ( var edge in result.VerticesEdges ) {
                string attr;
                edge.Attributes.TryGetValue("style", out attr);

                Console.WriteLine( "{0}->{1} : {2}", edge.Source.Id, edge.Destination.Id, attr ?? "" );
            }
            
            Console.WriteLine("----");

            result.SubGraphs.ToList().ForEach( graph => {
                Console.WriteLine( graph.Name );
                foreach ( var dotVertex in graph.Vertices ) {
                    Console.WriteLine( dotVertex );
                }
                Console.WriteLine();
            } );

            Console.WriteLine( "----" );

            foreach (var edge in result.SubGraphsEdges) {
                Console.WriteLine(edge.Source.Name);
            }


            Console.ReadLine();
        }
        */
    }

}
