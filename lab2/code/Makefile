objects = Extractor.o Line.o Main.o Printer.o ResultCodes.o SymbolTable.o Word.o

wi11-asm: $(objects)
	g++ -o wi11-asm $(objects)

Extractor.o: Extractor.h iExtractor.h SymbolTable.h iSymbolTable.h Word.h
Extractor.o: iWord.h ResultCodes.h Line.h iLine.h
Line.o: Line.h iLine.h Word.h iWord.h ResultCodes.h
Main.o: Extractor.h iExtractor.h SymbolTable.h iSymbolTable.h Word.h iWord.h
Main.o: ResultCodes.h Printer.h iPrinter.h
Printer.o: Printer.h iPrinter.h SymbolTable.h iSymbolTable.h Word.h iWord.h
Printer.o: ResultCodes.h Line.h iLine.h
ResultCodes.o: ResultCodes.h
SymbolTable.o: SymbolTable.h iSymbolTable.h Word.h iWord.h
Word.o: Word.h iWord.h

clean:
	rm -f wi11-asm $(objects)
