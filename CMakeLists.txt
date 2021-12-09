cmake_minimum_required(VERSION 3.16.3)
project(GraphOptimization)

set(CMAKE_CXX_STANDARD 14)

add_executable(GraphOptimization main.cpp
        Automaton.h
        BlockCommentAutomaton.cpp
        BlockCommentAutomaton.h
        ColonAutomaton.h
        ColonAutomaton.cpp
        ColonDashAutomaton.cpp
        ColonDashAutomaton.h
        CommaAutomaton.cpp
        CommaAutomaton.h
        CommentAutomaton.cpp
        CommentAutomaton.h
        ID_Automaton.cpp
        ID_Automaton.h
        Lexer.cpp
        Lexer.h
        MatcherFSA.h
        MatcherFSA.cpp
        StringAutomaton.cpp
        StringAutomaton.h
        Token.h
        Token.cpp
        Parser.cpp
        Parser.h
        ParserHelper.cpp
        ParserHelper.h
        DatalogProgram.cpp
        DatalogProgram.h
        Rule.cpp
        Rule.h
        Predicate.cpp
        Predicate.h
        Parameter.cpp
        Parameter.h
        Interpreter.cpp
        Interpreter.h
        Database.h
        Database.cpp
        Relation.cpp
        Relation.h
        Header.cpp
        Header.h
        Tuple.h
        Tuple.cpp
        Graph.h
        Graph.cpp
        )
