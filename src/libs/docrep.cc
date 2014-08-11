/* -*- Mode: C++; -*- */

#include "docrep.h"

namespace dr = schwa::dr;

using namespace model;

Token::Schema::Schema(void) :
    dr::Ann::Schema<Token>("Token", "The token class", "Token"),
    span(*this, "span", "the beginning and end byte offsets of the token", dr::FieldMode::READ_ONLY, "span"),
    raw(*this, "raw", "the raw token", dr::FieldMode::READ_ONLY, "raw"),
    norm(*this, "norm", "the normalised token", dr::FieldMode::READ_ONLY, "norm"),
    pos(*this, "pos", "the POS tag of the token", dr::FieldMode::READ_WRITE, "pos"),
    { }

Sentence::Schema::Schema(void) :
    dr::Ann::Schema<Sentence>("Sentence", "The sentence class", "Sentence"),
    span(*this, "span", "the span of the sentence", dr::FieldMode::READ_ONLY, "span"),
    parse(*this, "parse", "the C&C parse of the sentence", dr::FieldMode::READ_WRITE, "parse")
    { }

ParseNode::Schema::Schema(void) :
    dr::Ann::Schema<ParseNode>("ParseNode", "The parse node class", "ParseNode"),
    label(*this, "label", "the label", dr::FieldMode::READ_WRITE, "label"),
    children(*this, "children", "the children of the annotation", dr::FieldMode::READ_WRITE, "children"),
    span(*this, "span", "the span of the annotation if a leaf node", dr::FieldMode::READ_WRITE, "span")
    { }

Dependency::Schema::Schema(void) :
    dr::Ann::Schema<Dependency>("Dependency", "A dependency arc", "Dependency"),
    label(*this, "label", "the dependency label", dr::FieldMode::READ_WRITE, "label"),
    head(*this, "head", "the governing head", dr::FieldMode::READ_WRITE, "head"),
    dep(*this, "dep", "the dependent head", dr::FieldMode::READ_WRITE, "dep"),
    { }

Doc::Schema::Schema(void) :
    dr::Doc::Schema<Doc>("Doc", "The document class"),
    tokens(*this, "tokens", "The store for the tokens", dr::FieldMode::READ_WRITE, "tokens"),
    sentences(*this, "sentences", "The store for the sentences", dr::FieldMode::READ_WRITE, "sentences"),
    parse_nodes(*this, "parse_nodes", "The store for the parse nodes", dr::FieldMode::READ_WRITE, "parse_nodes"),
    dependencies(*this, "dependencies", "The store for the dependencies", dr::FieldMode::READ_WRITE, "dependencies")
    { }
