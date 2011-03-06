objects = cpp/Decoder.o cpp/Extractor.o cpp/Line.o cpp/Loader.o cpp/Main.o \
			cpp/Memory.o cpp/ObjParser.o cpp/Printer.o cpp/Register.o \
			cpp/ResultCodes.o cpp/SymbolTable.o cpp/Wi11.o cpp/Word.o cpp/itos.o

wi11: $(objects)
	g++ -o wi11 $(objects)

clean:
	rm -f wi11 $(objects)

# DO NOT DELETE

cpp/Decoder.o: h/Decoder.h h/iDecoder.h h/iWord.h h/Word.h
cpp/Extractor.o: h/Extractor.h h/iExtractor.h h/SymbolTable.h
cpp/Extractor.o: h/iSymbolTable.h h/Word.h h/iWord.h h/ResultCodes.h h/Word.h
cpp/Extractor.o: h/Line.h h/iLine.h h/SymbolTable.h h/ResultCodes.h h/itos.h
cpp/Line.o: h/Line.h h/iLine.h h/Word.h h/iWord.h h/ResultCodes.h h/Word.h
cpp/Line.o: h/ResultCodes.h
cpp/Loader.o: h/Loader.h h/iLoader.h h/ResultCodes.h h/iMemory.h h/iWord.h
cpp/Loader.o: h/Word.h h/Word.h h/ResultCodes.h h/iMemory.h h/iLoader.h
cpp/Loader.o: h/ObjParser.h h/iObjParser.h
cpp/Main.o: h/Wi11.h h/iWi11.h h/iWord.h h/iRegister.h h/iMemory.h
cpp/Main.o: h/ResultCodes.h h/iDecoder.h h/Word.h h/Register.h h/Memory.h
cpp/Main.o: h/Decoder.h h/Loader.h h/iLoader.h h/Extractor.h h/iExtractor.h
cpp/Main.o: h/SymbolTable.h h/iSymbolTable.h h/SymbolTable.h h/Printer.h
cpp/Main.o: h/iPrinter.h h/Line.h h/iLine.h h/ResultCodes.h
cpp/Memory.o: h/iMemory.h h/Memory.h h/iMemory.h h/iWord.h h/ResultCodes.h
cpp/Memory.o: h/Word.h h/ResultCodes.h
cpp/ObjParser.o: h/ObjParser.h h/iObjParser.h h/ResultCodes.h h/Word.h
cpp/ObjParser.o: h/iWord.h
cpp/Printer.o: h/Printer.h h/iPrinter.h h/SymbolTable.h h/iSymbolTable.h
cpp/Printer.o: h/Word.h h/iWord.h h/ResultCodes.h h/Line.h h/iLine.h h/Word.h
cpp/Printer.o: h/Line.h h/SymbolTable.h h/ResultCodes.h h/itos.h
cpp/Register.o: h/Register.h h/iRegister.h h/iWord.h h/Word.h
cpp/ResultCodes.o: h/ResultCodes.h
cpp/SymbolTable.o: h/SymbolTable.h h/Word.h h/iWord.h
cpp/Wi11.o: h/Wi11.h h/iWi11.h h/iWord.h h/iRegister.h h/iMemory.h
cpp/Wi11.o: h/ResultCodes.h h/iDecoder.h h/Word.h h/Register.h h/Memory.h
cpp/Wi11.o: h/Decoder.h h/Loader.h h/iLoader.h h/Register.h h/Memory.h
cpp/Wi11.o: h/iDecoder.h h/Decoder.h h/Loader.h h/ResultCodes.h
cpp/Word.o: h/Word.h h/iWord.h h/itos.h
cpp/itos.o: h/itos.h
