$(shell `if [ -d ./o ]; then mv ./o/*.o .; else mkdir o; fi`)

wi11-sim: Decoder.o Loader.o Main.o Memory.o ObjParser.o Register.o ResultCodes.o Wi11.o Word.o
	g++ -o wi11-sim Decoder.o Loader.o Main.o Memory.o ObjParser.o Register.o ResultCodes.o Wi11.o Word.o
	mv *.o ./o/

Decoder.o: Decoder.h iDecoder.h Word.h iWord.h ResultCodes.h
	g++ -c Decoder.cpp

Loader.o: Loader.cpp Loader.h iLoader.h ResultCodes.h iMemory.h Word.h iWord.h
	g++ -c Loader.cpp

Main.o: Wi11.h iWi11.h iWord.h iRegister.h iMemory.h ResultCodes.h iDecoder.h
	g++ -c Main.cpp

Memory.o: iMemory.h iWord.h ResultCodes.h Memory.h Word.h
	g++ -c Memory.cpp

ObjParser.o: ObjParser.h iObjParser.h ResultCodes.h Word.h iWord.h
	g++ -c ObjParser.cpp

Regiser.o: Register.h iRegister.h iWord.h Word.h
	g++ -c Register.cpp

ResultCodes.o: ResultCodes.h
	g++ -c ResultCodes.cpp

Wi11.o: Wi11.h iWi11.h iWord.h iRegister.h iMemory.h ResultCodes.h iDecoder.h Word.h Register.h Memory.h Decoder.h Loader.h iLoader.h
	g++ -c Wi11.cpp

Word.o: Word.h iWord.h
	g++ -c Word.cpp
