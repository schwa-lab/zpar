/* -*- Mode: C++; -*- */

#include <schwa/dr.h>
 
namespace model {

    class Token : public schwa::dr::Ann {
    public:
        schwa::dr::Slice<uint64_t> span;
        std::string raw;
        std::string norm;
        std::string pos;
        std::string lemma;

        class Schema;
    };


    class ParseNode;
    class Sentence : public schwa::dr::Ann {
    public:
        schwa::dr::Slice<Token *> span;
        schwa::dr::Pointer<ParseNode> parse;

        uint64_t size(void) { return span.stop - span.start; }

        class Schema;
    };


    class ParseNode : public schwa::dr::Ann {
    public:
        std::string label;
        schwa::dr::Pointers<ParseNode> children;
        schwa::dr::Slice<Token *> span;

        class Schema;
    };


    class Dependency : public schwa::dr::Ann {
    public:
        std::string label;
        schwa::dr::Pointer<Token> head;
        schwa::dr::Pointer<Token> dep;

        class Schema;
    };


    class Doc : public schwa::dr::Doc {
    public:
        schwa::dr::Store<Token> tokens;
        schwa::dr::Store<Sentence> sentences;
        schwa::dr::BlockStore<ParseNode> parse_nodes;
        schwa::dr::BlockStore<Dependency> dependencies;

        class Schema;
    };


    class Token::Schema : public schwa::dr::Ann::Schema<Token> {
    public:
        DR_FIELD(&Token::span) span;
        DR_FIELD(&Token::raw) raw;
        DR_FIELD(&Token::norm) norm;
        DR_FIELD(&Token::pos) pos;
        DR_FIELD(&Token::lemma) lemma;

        Schema(void);
        virtual ~Schema(void) { }
    };


    class Sentence::Schema : public schwa::dr::Ann::Schema<Sentence> {
    public:
        DR_POINTER(&Sentence::span, &Doc::tokens) span;
        DR_POINTER(&Sentence::parse, &Doc::parse_nodes) parse;

        Schema(void);
        virtual ~Schema(void) { }
    };


    class Dependency::Schema : public schwa::dr::Ann::Schema<Dependency> {
    public:
        DR_FIELD(&Dependency::label) label;
        DR_POINTER(&Dependency::head, &Doc::tokens) head;
        DR_POINTER(&Dependency::dep, &Doc::tokens) dep;

        Schema(void);
        virtual ~Schema(void) { }
    };


    class Doc::Schema : public schwa::dr::Doc::Schema<Doc> {
    public:
        DR_STORE(&Doc::tokens) tokens;
        DR_STORE(&Doc::sentences) sentences;
        DR_STORE(&Doc::parse_nodes) parse_nodes;
        DR_STORE(&Doc::dependencies) dependencies;

        Schema(void);
        virtual ~Schema(void) { }
    };

}
