# Tiller - Tiled TinyXML-2 parser

Tiller is a simple, small, efficient, C++ Tiled XML parser that can be easily integrated into other programs.

What it does.
In brief, Tiller parses an XML document, and builds from that a Document Object Model (DOM) that can be read, modified, and saved.

XML stands for "eXtensible Markup Language." It is a general purpose human and machine readable markup language to describe arbitrary data. All those random file formats created to store application data can all be replaced with XML. One parser for everything.

http://en.wikipedia.org/wiki/XML

There are different ways to access and interact with XML data. Tiller uses a Document Object Model (DOM), meaning the XML data is parsed into a C++ objects that can be browsed and manipulated, and then written to disk or another output stream. You can also construct an XML document from scratch with C++ objects and write this to disk or another output stream.

Simplify your life when using Tiled to create your own game maps. Tiller can utilise XML generated files & structure from Tiled make it easier
to load & parse it. Without need to worry about what the structure is, calculations of tile fields, loading images and parsing it into your own game.