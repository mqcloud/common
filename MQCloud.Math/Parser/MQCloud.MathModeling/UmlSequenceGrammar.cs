using Irony.Parsing;

namespace MQCloud.MathModeling {
    [Language( "UML-Sequence", "1.0", "UML Sequence flow format (based on plant uml)" )]
    public class UmlSequenceGrammar: Grammar {
        public UmlSequenceGrammar() {
            NumberLiteral number = new NumberLiteral( "number" );
            number.AddPrefix( "#", NumberOptions.IntOnly );

            IdentifierTerminal IDENTIFIER = new IdentifierTerminal( "IDENTIFIER" );

            CommentTerminal BLOCK_COMMENT = new CommentTerminal( "BLOCK_COMMENT", "/'", "'/" );
            CommentTerminal LINE_COMMENT = new CommentTerminal( "LINE_COMMENT", "'", "\n", "\r\n" );
            CommentTerminal AfterText = new CommentTerminal( "AfterText", ":", "\n", "\r\n" );
            NonGrammarTerminals.Add( BLOCK_COMMENT );
            NonGrammarTerminals.Add( LINE_COMMENT );

            var startumlStatement = new NonTerminal( "startuml" );
            startumlStatement.Rule = ToTerm( "@startuml" );

            var endumlStatement = new NonTerminal( "enduml" );
            endumlStatement.Rule = ToTerm( "@enduml" ) + Eof;

            var directionFrom = new NonTerminal( "directionFrom" );
            directionFrom.Rule = ToTerm( "<" ) | "<-" | "<<" | "-";

            var directionTo = new NonTerminal( "directionTo" );
            directionTo.Rule = ToTerm( "-" ) | ">" | "->" | "->>";

            var direction = new NonTerminal( "direction" );

            direction.Rule = directionFrom + ( ( "[" + number + "]" ) | Empty ) + directionTo;
            var type = new NonTerminal( "type" );
            type.Rule = ToTerm( "actor" ) | "boundary" | "control" | "entity" | "database" | "participant";
                
            var declaration = new NonTerminal( "declaration" );
            declaration.Rule = type + IDENTIFIER + Eos;

            var message = IDENTIFIER + direction + IDENTIFIER + ( Empty | AfterText | Eos );

            var line = new NonTerminal( "line" );
            line.Rule = message | declaration | BLOCK_COMMENT | LINE_COMMENT;

            var lines = new NonTerminal( "lines" );
            lines.Rule = MakePlusRule( lines, line );

            var programm = new NonTerminal( "flow" );
            programm.Rule = startumlStatement + lines + endumlStatement;

            Root = programm;

        }//constructor
    }
}